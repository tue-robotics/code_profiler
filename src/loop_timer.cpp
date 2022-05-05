#include "tue/profiling/loop_timer.h"

#include <stdlib.h>
#include <exception>

namespace tue
{

LoopTimer::LoopTimer(): counts_(0), sum_(0.0), c_(0.0)
{
}

void LoopTimer::start()
{
    ++counts_;
    if (!total_timer_.running())
        total_timer_.start();
    timer_.start();
}

void LoopTimer::stop()
{
    timer_.stop();
    long double time = timer_.getElapsedTime();
    long double y = time - c_;
    long double x = sum_ + y;
    c_ = ( x - sum_ ) - y;
    sum_ = x;
}

void LoopTimer::reset()
{
    total_timer_.stop();
    timer_.stop();
    counts_ = 0;
    sum_ = 0.0;
    c_ = 0.0;
}

long double LoopTimer::getTotalLoopTime()
{
    if (timer_.running())
        this->stop();

    return sum_;
}

long double LoopTimer::getTotalLoopTime() const
{
    if (timer_.running())
        return sum_;

    return sum_;
}

long double LoopTimer::getAverageLoopTime()
{
    if (timer_.running())
        this->stop();

    return sum_/counts_;
}

long double LoopTimer::getAverageLoopTime() const
{
    if (timer_.running())
        return sum_/std::max<int>(counts_ - 1, 1);

    return sum_/counts_;
}


long double LoopTimer::getTotalTime() const
{
    return total_timer_.getElapsedTime();
}

double LoopTimer::getLoopUsagePercentage()
{
    return getTotalLoopTime()/getTotalTime();
}

double LoopTimer::getLoopUsagePercentage() const
{
    return getTotalLoopTime()/getTotalTime();
}

}

