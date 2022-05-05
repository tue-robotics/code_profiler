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
     * @brief Get total elapsed time during the loops
     * @return Elapsed time in seconds
     */
    long double getTotalLoopTime();

    /**
     * @brief Get average time per loop iteration
     * @return Average time per loop iteration in seconds
     */
    long double getAverageLoopTime();

    /**
     * @brief Get average percentage of total time used by the loop
     * @return Percentage in range [0-1]
     */
    double getLoopUsagePercentage();
};

}

#endif // TUE_PROFILING_LOOP_TIMER_H_
