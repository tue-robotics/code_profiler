#include "tue/profiling/loop_timer.h"
#include <stdlib.h>

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

long double LoopTimer::getTotalTime()
{
    if (timer_.running())
        this->stop();

    return sum_;
}

long double LoopTimer::getAverageTime()
{
    if (timer_.running())
        this->stop();

    return sum_/counts_;
}

double LoopTimer::getLoopUsagePercentage()
{
    return getTotalTime()/total_timer_.getElapsedTime();
}

}

