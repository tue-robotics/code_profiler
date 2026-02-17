//////////////////////////////////////////////////////////////////////////////
// Timer.cpp
// =========
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// using chrono STL (C++11) in both Windows, Linux and Unix system
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2024-04-17
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#include "profiling/Timer.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
Timer::Timer() : startPoint(std::chrono::high_resolution_clock::now()), endPoint(startPoint), stopped(false) {}

///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
Timer::~Timer() {}

///////////////////////////////////////////////////////////////////////////////
// start timer.
// startPoint will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::start()
{
    stopped = false; // reset stop flag
    startPoint = std::chrono::high_resolution_clock::now();
}

///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endPoint will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::stop()
{
    stopped = true; // set timer stopped flag
    endPoint = std::chrono::high_resolution_clock::now();
}

///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in nano-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInNanoSec() const
{
    if (!stopped)
        endPoint = std::chrono::high_resolution_clock::now();

    // time interval (nanosec) as double
    return std::chrono::duration<double, std::nano>{endPoint - startPoint}.count();
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInNanoSec by 1000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMicroSec() const
{
    return this->getElapsedTimeInNanoSec() * 0.001;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInNanoSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMilliSec() const
{
    return this->getElapsedTimeInNanoSec() * 0.000001;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInNanoSec by 1000000000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInSec() const
{
    return this->getElapsedTimeInNanoSec() * 0.000000001;
}

///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTime() const
{
    return this->getElapsedTimeInSec();
}

// TUe
void Timer::printLastElapsedTime(std::string m) const
{
    std::cout << m << " (sec): " << getElapsedTimeInSec() << std::endl;
}

void Timer::printLastElapsedTimeInMilliSec(std::string m) const
{
    std::cout << m << " (msec): " << getElapsedTimeInMilliSec() << std::endl;
}

void Timer::printLastElapsedTimeInMicroSec(std::string m) const
{
    std::cout << m << " (μsec): " << getElapsedTimeInMicroSec() << std::endl;
}

void Timer::printLastElapsedTimeInNanoSec(std::string m) const
{
    std::cout << m << " (nsec): " << getElapsedTimeInNanoSec() << std::endl;
}
