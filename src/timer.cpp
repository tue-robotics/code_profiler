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

Timer::Timer() : running_(false)
{
#ifdef WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
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
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&start_count_, NULL);
#endif
}

void Timer::stop()
{
    running_ = false;

#ifdef WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&end_count_, NULL);
#endif
}

double Timer::getElapsedTimeInMicroSec() const
{
#ifdef WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    double startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    double endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    timeval end_count;
    if (!running_) {
        end_count = end_count_;
    } else {
        gettimeofday(&end_count, NULL);
    }

    double startTimeInMicroSec = (start_count_.tv_sec * 1000000.0) + start_count_.tv_usec;
    double endTimeInMicroSec = (end_count.tv_sec * 1000000.0) + end_count.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}

double Timer::getElapsedTimeInMilliSec() const
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double Timer::getElapsedTimeInSec() const
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double Timer::getElapsedTime() const
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

