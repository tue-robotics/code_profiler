#include <profiling/Profiler.h>
#include <tue/profiling/loop_timer.h>
#include <tue/profiling/profiler.h>
#include <tue/profiling/scoped_timer.h>
#include <tue/profiling/timer.h>

#include <gtest/gtest.h>

#include <chrono>
#include <cmath>
#include <ratio>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace
{

constexpr std::chrono::milliseconds SLEEP{20};

void sleep()
{
    std::this_thread::sleep_for(SLEEP);
}

// ----------------------------------------------------------------------------------------------------

TEST(Timer, NotRunningAfterConstruction)
{
    const tue::Timer timer;
    EXPECT_FALSE(timer.running());
    EXPECT_EQ(timer.getElapsedDuration().count(), 0);
    EXPECT_EQ(timer.getElapsedTime(), 0.0L);
}

TEST(Timer, IsMonotonic)
{
    EXPECT_TRUE(tue::Timer::Clock::is_steady);
}

TEST(Timer, MeasuresElapsedTime)
{
    tue::Timer timer;
    timer.start();
    EXPECT_TRUE(timer.running());
    sleep();
    timer.stop();
    EXPECT_FALSE(timer.running());

    EXPECT_GE(timer.getElapsedDuration(), SLEEP);
    // Stopped, so repeated reads are identical
    EXPECT_EQ(timer.getElapsedDuration(), timer.getElapsedDuration());
}

TEST(Timer, RunningTimerKeepsCounting)
{
    tue::Timer timer;
    timer.start();
    const auto first = timer.getElapsedDuration();
    sleep();
    EXPECT_GE(timer.getElapsedDuration() - first, SLEEP);
}

TEST(Timer, ElapsedDurationUntilTimePoint)
{
    tue::Timer timer;
    timer.start();
    const auto now = tue::Timer::Clock::now();
    const auto first = timer.getElapsedDuration(now);
    // Exact: the elapsed time grows with the given time point only
    EXPECT_EQ(timer.getElapsedDuration(now + SLEEP) - first, SLEEP);

    timer.stop();
    // Stopped, so the time point is not used
    const auto elapsed = timer.getElapsedDuration();
    EXPECT_EQ(timer.getElapsedDuration(tue::Timer::Clock::now() + SLEEP), elapsed);
}

TEST(Timer, UnitConversionsAreCorrectlyRounded)
{
    tue::Timer timer;
    timer.start();
    sleep();
    timer.stop();

    // Each unit is a single correctly rounded conversion of the exact integer tick count
    const long double ns = std::chrono::duration<long double, std::nano>(timer.getElapsedDuration()).count();
    EXPECT_EQ(timer.getElapsedTimeInNanoSec(), ns);
    EXPECT_EQ(timer.getElapsedTimeInMicroSec(), ns / 1e3L);
    EXPECT_EQ(timer.getElapsedTimeInMilliSec(), ns / 1e6L);
    EXPECT_EQ(timer.getElapsedTimeInSec(), ns / 1e9L);
    EXPECT_EQ(timer.getElapsedTime(), timer.getElapsedTimeInSec());
}

TEST(Timer, NowIsEpochTime)
{
    const long double now_sec = tue::Timer::now();
    const long double expected =
        std::chrono::duration<long double>(std::chrono::system_clock::now().time_since_epoch()).count();
    EXPECT_NEAR(now_sec, expected, 1.0L);
    EXPECT_NEAR(tue::Timer::nowMilliSec(), now_sec * 1e3L, 1e3L);
    EXPECT_NEAR(tue::Timer::nowMicroSec(), now_sec * 1e6L, 1e6L);
}

// ----------------------------------------------------------------------------------------------------

TEST(LoopTimer, EmptyTimerReturnsZeroInsteadOfNaN)
{
    tue::LoopTimer timer;
    const tue::LoopTimer& const_timer = timer;
    EXPECT_EQ(const_timer.getAverageLoopTime(), 0.0L);
    EXPECT_EQ(const_timer.getLoopUsagePercentage(), 0.0);
    EXPECT_EQ(timer.getAverageLoopTime(), 0.0L);
    EXPECT_EQ(timer.getLoopUsagePercentage(), 0.0);
    EXPECT_EQ(timer.getTotalLoopTime(), 0.0L);
    EXPECT_EQ(timer.getIterationCount(), 0U);
}

TEST(LoopTimer, StopIsIdempotent)
{
    tue::LoopTimer timer;
    timer.start();
    sleep();
    timer.stop();
    const long double total = timer.getTotalLoopTime();

    sleep();
    timer.stop();
    EXPECT_EQ(timer.getTotalLoopTime(), total);
}

TEST(LoopTimer, NoDoubleCountingAfterNonConstGetter)
{
    // The non-const getters stop the current iteration; the regular stop afterwards must not add that time again
    tue::LoopTimer timer;
    timer.start();
    sleep();
    const long double mid = timer.getTotalLoopTime();
    sleep();
    timer.stop();
    EXPECT_EQ(timer.getTotalLoopTime(), mid);
    EXPECT_LE(timer.getLoopUsagePercentage(), 1.0);
}

TEST(LoopTimer, ConstGettersExcludeCurrentIteration)
{
    tue::LoopTimer timer;
    timer.start();
    sleep();
    timer.stop();
    timer.start();
    sleep();

    const tue::LoopTimer& const_timer = timer;
    EXPECT_EQ(const_timer.getIterationCount(), 2U);
    const long double total = const_timer.getTotalLoopTime();
    EXPECT_EQ(const_timer.getAverageLoopTime(), total);
    EXPECT_LT(const_timer.getLoopUsagePercentage(), 1.0);
}

TEST(LoopTimer, TotalLoopDurationIncludesRunningIteration)
{
    tue::LoopTimer timer;
    timer.start();
    sleep();
    timer.stop();
    const auto completed = timer.getTotalLoopDuration(tue::Timer::Clock::now());
    EXPECT_GE(completed, SLEEP);
    EXPECT_EQ(std::chrono::duration<long double>(completed).count(), std::as_const(timer).getTotalLoopTime());

    timer.start();
    const auto now = tue::Timer::Clock::now();
    const auto running = timer.getTotalLoopDuration(now);
    EXPECT_GE(running, completed);
    // Exact: while running, the loop time grows with the given time point only
    EXPECT_EQ(timer.getTotalLoopDuration(now + SLEEP) - running, SLEEP);
    // Not stopped by the const getter
    EXPECT_EQ(std::as_const(timer).getTotalLoopTime(), std::chrono::duration<long double>(completed).count());
}

TEST(LoopTimer, LoopTimeDuringIntervalIsBoundedByInterval)
{
    // B(t) = getTotalLoopDuration(t) never decreases and grows at most as fast as t, so the loop time in any interval
    // [t0, t1] is within [0, t1 - t0]. Sampled around every start and stop, including a start while running.
    tue::LoopTimer timer;
    auto prev_time = tue::Timer::Clock::now();
    auto prev_loop = timer.getTotalLoopDuration(prev_time);
    const auto check = [&]
    {
        const auto time = tue::Timer::Clock::now();
        const auto loop = timer.getTotalLoopDuration(time);
        EXPECT_GE(loop, prev_loop);
        EXPECT_LE(loop - prev_loop, time - prev_time);
        prev_time = time;
        prev_loop = loop;
    };

    for (int i = 0; i < 100; ++i)
    {
        timer.start();
        check();
        timer.start(); // Ends the running iteration, so its time is kept
        check();
        timer.stop();
        check();
    }
    EXPECT_EQ(timer.getIterationCount(), 200U);
}

TEST(LoopTimer, StartWhileRunningKeepsIterationTime)
{
    tue::LoopTimer timer;
    timer.start();
    sleep();
    timer.start();
    sleep();
    timer.stop();
    EXPECT_EQ(timer.getIterationCount(), 2U);
    EXPECT_GE(timer.getTotalLoopDuration(tue::Timer::Clock::now()), 2 * SLEEP);
}

TEST(LoopTimer, LoopUsageAtTimePointIncludesRunningIteration)
{
    tue::LoopTimer timer;
    const auto now = tue::Timer::Clock::now();
    EXPECT_EQ(timer.getLoopUsagePercentage(now), 0.0);

    timer.start();
    sleep();
    timer.stop();
    sleep();
    const double idle = timer.getLoopUsagePercentage(tue::Timer::Clock::now());
    EXPECT_GT(idle, 0.0);
    EXPECT_LT(idle, 1.0);

    timer.start();
    const auto start = tue::Timer::Clock::now();
    // Running for an hour: nearly the whole time is used by the loop, but never more
    const double busy = timer.getLoopUsagePercentage(start + std::chrono::hours(1));
    EXPECT_GT(busy, 0.9999);
    EXPECT_LE(busy, 1.0);
    // The const overload without a time point excludes the running iteration
    EXPECT_LT(std::as_const(timer).getLoopUsagePercentage(), busy);
}

TEST(LoopTimer, SumIsExact)
{
    tue::LoopTimer timer;
    for (int i = 0; i < 1000; ++i)
    {
        timer.start();
        timer.stop();
    }
    // The total is a single conversion of an exact integer tick count, so converting it back yields an integer
    const long double total = std::as_const(timer).getTotalLoopTime();
    const long double ticks = total * tue::Timer::Clock::period::den / tue::Timer::Clock::period::num;
    EXPECT_NEAR(ticks, std::round(ticks), 1e-6L);
    EXPECT_EQ(timer.getIterationCount(), 1000U);
    EXPECT_NEAR(timer.getAverageLoopTime(), total / 1000, 1e-15L);
}

TEST(LoopTimer, ResetClearsEverything)
{
    tue::LoopTimer timer;
    timer.start();
    sleep();
    timer.stop();
    timer.reset();
    EXPECT_EQ(timer.getIterationCount(), 0U);
    EXPECT_EQ(timer.getTotalLoopTime(), 0.0L);
    EXPECT_EQ(timer.getAverageLoopTime(), 0.0L);
    EXPECT_EQ(timer.getTotalTime(), 0.0L);
}

TEST(LoopTimer, ResetOfUnusedTimer)
{
    tue::LoopTimer timer;
    timer.reset();
    EXPECT_EQ(timer.getTotalTime(), 0.0L);
}

// ----------------------------------------------------------------------------------------------------

TEST(Profiler, IsNotCopyableOrMovable)
{
    EXPECT_FALSE(std::is_copy_constructible_v<tue::Profiler>);
    EXPECT_FALSE(std::is_move_constructible_v<tue::Profiler>);
}

TEST(Profiler, NestedScopes)
{
    tue::Profiler profiler("test");
    for (int i = 0; i < 3; ++i)
    {
        const tue::ScopedTimer outer(profiler, "outer");
        {
            const tue::ScopedTimer inner(profiler, "inner");
        }
        const tue::ScopedTimer sibling(profiler, "sibling");
    }

    std::stringstream ss;
    ss << profiler;
    const std::string out = ss.str();
    EXPECT_NE(out.find("[test]"), std::string::npos);
    EXPECT_NE(out.find("\n  outer: "), std::string::npos);
    EXPECT_NE(out.find("\n    inner: "), std::string::npos);
    EXPECT_NE(out.find("\n    sibling: "), std::string::npos);
}

TEST(Profiler, UnbalancedStopIsHarmless)
{
    tue::Profiler profiler("test");
    profiler.stopTimer();
    profiler.startTimer("a");
    profiler.stopTimer();
    profiler.stopTimer();
    profiler.startTimer("b");
    profiler.stopTimer();

    std::stringstream ss;
    ss << profiler;
    EXPECT_NE(ss.str().find("\n  a: "), std::string::npos);
    EXPECT_NE(ss.str().find("\n  b: "), std::string::npos);
}

// ----------------------------------------------------------------------------------------------------

TEST(ThreadProfiler, DuplicateStopIsIgnored)
{
    ThreadProfiler::Start("dup");
    sleep();
    ThreadProfiler::Stop("dup");
    sleep();
    ThreadProfiler::Stop("dup");

    const Statistics& stats = ThreadProfiler::ReturnCurrentStatistics()->at("dup");
    EXPECT_EQ(stats.callAmount, 1);
    EXPECT_GE(stats.total, SLEEP);
    EXPECT_LT(stats.total, 2 * SLEEP);
}

TEST(ThreadProfiler, ParentIsTheEnclosingActiveScope)
{
    // "f" is first called from "a", later from "b"; a new scope opened in "b" after "f" must get parent "b"
    ThreadProfiler::Start("ctx_a");
    ThreadProfiler::Start("ctx_f");
    ThreadProfiler::Stop("ctx_f");
    ThreadProfiler::Stop("ctx_a");

    ThreadProfiler::Start("ctx_b");
    ThreadProfiler::Start("ctx_f");
    ThreadProfiler::Stop("ctx_f");
    ThreadProfiler::Start("ctx_g");
    ThreadProfiler::Stop("ctx_g");
    ThreadProfiler::Stop("ctx_b");

    const FunctionStatsPtr stats = ThreadProfiler::ReturnCurrentStatistics();
    EXPECT_EQ(stats->at("ctx_f").parent, "ctx_a");
    EXPECT_EQ(stats->at("ctx_g").parent, "ctx_b");
    EXPECT_EQ(stats->at("ctx_b").parent, "");
}

TEST(ThreadProfiler, InterleavedStop)
{
    ThreadProfiler::Start("il_x");
    ThreadProfiler::Start("il_y");
    ThreadProfiler::Stop("il_x");
    ThreadProfiler::Start("il_z"); // "il_y" is still active
    ThreadProfiler::Stop("il_z");
    ThreadProfiler::Stop("il_y");

    EXPECT_EQ(ThreadProfiler::ReturnCurrentStatistics()->at("il_z").parent, "il_y");
}

} // namespace
