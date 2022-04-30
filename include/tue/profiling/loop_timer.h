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
    void reset();

    double getTotalTime();
    double getAverageTime();
};

}

#endif // TUE_PROFILING_LOOP_TIMER_H_
