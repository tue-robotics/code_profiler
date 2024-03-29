#ifndef TUE_PROFILING_LOOP_TIMER_H_
#define TUE_PROFILING_LOOP_TIMER_H_


#include <iostream>
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
     * @brief Sum so far
     */
    long double sum_;

    /**
     * @brief Compensation for lost low-order bits
     */
    long double c_;

public:
    LoopTimer();
    ~LoopTimer() = default;

    void start();
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
     * @brief Get average time per loop iteration. Stops the current iteration when running.
     * @return Average time per loop iteration in seconds
     */
    ///@{
    long double getAverageLoopTime();
    /// const version which excludes the current loop iteration when running
    long double getAverageLoopTime() const;
    ///@}

    /**
     * @brief Get the total elapsed time since the first start.Stops the current iteration when running.
     * @return Total time since start in seconds
     */
    long double getTotalTime() const;

    /**
     * @brief Get average percentage of total time used by the loop
     * @return Percentage in range [0-1]
     */
    ///@{
    double getLoopUsagePercentage();
    /// const version which excludes the current loop iteration when running
    double getLoopUsagePercentage() const;
    ///@}
};

}

#endif // TUE_PROFILING_LOOP_TIMER_H_
