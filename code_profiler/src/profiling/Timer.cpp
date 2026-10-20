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

#include <chrono>
#include <iostream>
#include <ratio>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
Timer::Timer() : startPoint(Clock::now()), endPoint(startPoint), stopped(false) {}

///////////////////////////////////////////////////////////////////////////////
// start timer.
// startPoint will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::start()
{
    stopped = false; // reset stop flag
    startPoint = Clock::now(); // TUe: last, so the overhead of this function is not measured
}

///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endPoint will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::stop()
{
    endPoint = Clock::now(); // TUe: first, so the overhead of this function is not measured
    stopped = true; // set timer stopped flag
}

///////////////////////////////////////////////////////////////////////////////
// TUe: compute the exact elapsed time as integer clock ticks.
// other getElapsedTime will call this first, then convert to correspond resolution.
// A local end point is used, so this const function is free of side effects and data races.
///////////////////////////////////////////////////////////////////////////////
Timer::Clock::duration Timer::getElapsedDuration() const
{
    return (stopped ? endPoint : Clock::now()) - startPoint;
}

///////////////////////////////////////////////////////////////////////////////
// time interval (nanosec) as double
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInNanoSec() const
{
    return std::chrono::duration<double, std::nano>{this->getElapsedDuration()}.count();
}

///////////////////////////////////////////////////////////////////////////////
// TUe: convert the integer ticks directly, a single correctly rounded division,
// instead of multiplying the nano-seconds by an inexact constant (0.001)
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMicroSec() const
{
    return std::chrono::duration<double, std::micro>{this->getElapsedDuration()}.count();
}

///////////////////////////////////////////////////////////////////////////////
// TUe: see getElapsedTimeInMicroSec
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMilliSec() const
{
    return std::chrono::duration<double, std::milli>{this->getElapsedDuration()}.count();
}

///////////////////////////////////////////////////////////////////////////////
// TUe: see getElapsedTimeInMicroSec
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInSec() const
{
    return std::chrono::duration<double>{this->getElapsedDuration()}.count();
}

///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTime() const
{
    return this->getElapsedTimeInSec();
}

// TUe
void Timer::printLastElapsedTime(const std::string& m) const
{
    std::cout << m << " (sec): " << getElapsedTimeInSec() << '\n';
}

void Timer::printLastElapsedTimeInMilliSec(const std::string& m) const
{
    std::cout << m << " (msec): " << getElapsedTimeInMilliSec() << '\n';
}

void Timer::printLastElapsedTimeInMicroSec(const std::string& m) const
{
    std::cout << m << " (μsec): " << getElapsedTimeInMicroSec() << '\n';
}

void Timer::printLastElapsedTimeInNanoSec(const std::string& m) const
{
    std::cout << m << " (nsec): " << getElapsedTimeInNanoSec() << '\n';
}
