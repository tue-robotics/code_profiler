//////////////////////////////////////////////////////////////////////////////
// Timer.cpp
// =========
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#include "tue/profiling/timer.h"
#include <stdlib.h>

namespace tue
{

#ifdef WIN32
inline long double timeCountsToLongDouble(const LARGE_INTEGER& counts, const LARGE_INTEGER& frequency)
{
    return counts.QuadPart * (1000000.0 / frequency.QuadPart);
}
#else
inline long double timevalToLongDouble(const timeval& time)
{
    return (time.tv_sec * 1000000.0) + time.tv_usec;
}
#endif

// ----------------------------------------------------------------------------------------------------

Timer::Timer() : running_(false)
{
#ifdef WIN32
    QueryPerformanceFrequency(&frequency_);
    startCount_.QuadPart = 0;
    endCount_.QuadPart = 0;
#else
    start_count_.tv_sec = start_count_.tv_usec = 0;
    end_count_.tv_sec = end_count_.tv_usec = 0;
#endif
}

Timer::~Timer()
{
}

void Timer::start()
{
    running_ = true;
#ifdef WIN32
    QueryPerformanceCounter(&startCount_);
#else
    gettimeofday(&start_count_, NULL);
#endif
}

void Timer::stop()
{
    running_ = false;

#ifdef WIN32
    QueryPerformanceCounter(&endCount_);
#else
    gettimeofday(&end_count_, NULL);
#endif
}

long double Timer::getElapsedTimeInMicroSec() const
{
#ifdef WIN32
    LARGE_INTEGER endCount;
    if(!running_)
        endCount = endCount_;
    else
        QueryPerformanceCounter(&endCount);

    long double startTimeInMicroSec = timeCountsToLongDouble(startCount_, frequency_);
    long double endTimeInMicroSec = timeCountsToLongDouble(endCount, frequency_);
#else
    timeval end_count;
    if (!running_)
        end_count = end_count_;
    else
        gettimeofday(&end_count, NULL);

    long double startTimeInMicroSec = timevalToLongDouble(start_count_);
    long double endTimeInMicroSec = timevalToLongDouble(end_count);
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}

long double Timer::getElapsedTimeInMilliSec() const
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

long double Timer::getElapsedTimeInSec() const
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

long double Timer::getElapsedTime() const
{
    return this->getElapsedTimeInSec();
}

void Timer::printLastElapsedTime(std::string m)
{
	std::cout << m << " (sec): " << getElapsedTimeInSec() << std::endl;
}


void Timer::printLastElapsedTimeMSec(std::string m)
{
	std::cout << m << " (msec): " << getElapsedTimeInMilliSec() << std::endl;
}

}

