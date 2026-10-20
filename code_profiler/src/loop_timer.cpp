#include "tue/profiling/loop_timer.h"
#include "tue/profiling/timer.h"

#include <chrono>
#include <cstdint>

namespace tue
{

namespace
{
long double toSec(const Timer::Clock::duration& duration)
{
    return std::chrono::duration<long double>(duration).count();
}

long double average(const Timer::Clock::duration& sum, std::uint64_t counts)
{
    return counts == 0 ? 0.0L : toSec(sum) / static_cast<long double>(counts);
}

double usage(const Timer::Clock::duration& loop_time, const Timer::Clock::duration& total_time)
{
    // Ratio of the exact integer durations; the unit conversion cancels out, so it is not applied.
    if (total_time.count() <= 0)
        return 0.0;
    return static_cast<double>(static_cast<long double>(loop_time.count()) /
                               static_cast<long double>(total_time.count()));
}
} // namespace

LoopTimer::LoopTimer() : counts_(0), sum_(Timer::Clock::duration::zero()) {}

void LoopTimer::start()
{
    // Restarting the running iteration would discard its time, which makes the total loop time decrease
    this->stop();
    ++counts_;
    if (!total_timer_.running())
        total_timer_.start();
    timer_.start();
}

void LoopTimer::stop()
{
    if (!timer_.running())
        return;

    timer_.stop();
    sum_ += timer_.getElapsedDuration();
}

void LoopTimer::reset()
{
    // Fresh timers, so the elapsed times are zero too (a stopped timer keeps its last interval)
    total_timer_ = Timer();
    timer_ = Timer();
    counts_ = 0;
    sum_ = Timer::Clock::duration::zero();
}

long double LoopTimer::getTotalLoopTime()
{
    this->stop();
    return toSec(sum_);
}

long double LoopTimer::getTotalLoopTime() const
{
    return toSec(sum_);
}

Timer::Clock::duration LoopTimer::getTotalLoopDuration(Timer::Clock::time_point now) const
{
    return timer_.running() ? sum_ + timer_.getElapsedDuration(now) : sum_;
}

long double LoopTimer::getAverageLoopTime()
{
    this->stop();
    return average(sum_, counts_);
}

long double LoopTimer::getAverageLoopTime() const
{
    return average(sum_, timer_.running() ? counts_ - 1 : counts_);
}

long double LoopTimer::getTotalTime() const
{
    return total_timer_.getElapsedTime();
}

double LoopTimer::getLoopUsagePercentage()
{
    this->stop();
    return usage(sum_, total_timer_.getElapsedDuration());
}

double LoopTimer::getLoopUsagePercentage() const
{
    return usage(sum_, total_timer_.getElapsedDuration());
}

double LoopTimer::getLoopUsagePercentage(Timer::Clock::time_point now) const
{
    // The iterations don't overlap and lie within the total time, so the loop time never exceeds it; as rounding is
    // monotonic, the ratio is never above 1.
    return usage(getTotalLoopDuration(now), total_timer_.getElapsedDuration(now));
}

} // namespace tue
