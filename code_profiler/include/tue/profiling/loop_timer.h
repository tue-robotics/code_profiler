#ifndef TUE_PROFILING_LOOP_TIMER_H_
#define TUE_PROFILING_LOOP_TIMER_H_

#include <iostream> // IWYU pragma: export - kept for backwards compatibility of code relying on it
#include <tue/profiling/timer.h>

namespace tue
{

class LoopTimer
{
private:
    tue::Timer timer_;
    tue::Timer total_timer_;

    /**
     * @brief loop counter
     */
    long unsigned int counts_;

    /**
     * @brief Sum so far. Integer clock ticks, so the summation is exact and needs no (Kahan) error compensation.
     * Overflows only after 292 years with a nano-second clock.
     */
    Timer::Clock::duration sum_;

public:
    LoopTimer();
    ~LoopTimer() = default;

    /**
     * @brief Start a new iteration. A running iteration is stopped first, so its time is still counted.
     */
    void start();

    /**
     * @brief Stop the current iteration. Does nothing when no iteration is running, so it never counts time twice.
     */
    void stop();

    /**
     * @brief Stop the timer and reset all counters to zero
     */
    void reset();

    /**
     * @brief Get total number of loop iterations. When running, the current iteration is included.
     * @return Number of loop iterations
     */
    inline long unsigned int getIterationCount() const { return counts_; }

    /**
     * @brief Get total elapsed time during the loops. Stops the current iteration when running.
     * @return Elapsed time in seconds
     */
    ///@{
    long double getTotalLoopTime();
    /// const version which excludes the current loop iteration when running
    long double getTotalLoopTime() const;
    ///@}

    /**
     * @brief Get the exact total time of the loop iterations until the given time point, including the running
     * iteration until then. Changes nothing.
     *
     * As a function of the time point, this never decreases and grows at most as fast as the time point. So the loop
     * time during an interval [t0, t1], getTotalLoopDuration(t1) - getTotalLoopDuration(t0), is within [0, t1 - t0],
     * also when an iteration only partly overlaps the interval.
     * @param now Time point of Timer::Clock, not before the last call to start() or stop()
     * @return Total loop time as an integer number of clock ticks
     */
    Timer::Clock::duration getTotalLoopDuration(Timer::Clock::time_point now) const;

    /**
     * @brief Get average time per loop iteration. Stops the current iteration when running.
     * @return Average time per loop iteration in seconds, 0 when no iteration has completed
     */
    ///@{
    long double getAverageLoopTime();
    /// const version which excludes the current loop iteration when running
    long double getAverageLoopTime() const;
    ///@}

    /**
     * @brief Get the total elapsed time since the first start.
     * @return Total time since start in seconds
     */
    long double getTotalTime() const;

    /**
     * @brief Get average percentage of total time used by the loop. Stops the current iteration when running.
     * @return Percentage in range [0-1], 0 when the timer has not been started
     */
    ///@{
    double getLoopUsagePercentage();
    /// const version which excludes the current loop iteration when running
    double getLoopUsagePercentage() const;
    ///@}

    /**
     * @brief Get average percentage of total time used by the loop until the given time point, including the running
     * iteration until then. Changes nothing.
     * @param now Time point of Timer::Clock, not before the last call to start() or stop()
     * @return Percentage in range [0-1], 0 when the timer has not been started
     */
    double getLoopUsagePercentage(Timer::Clock::time_point now) const;
};

} // namespace tue

#endif // TUE_PROFILING_LOOP_TIMER_H_
