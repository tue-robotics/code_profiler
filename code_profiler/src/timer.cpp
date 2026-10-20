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

#include "tue/profiling/timer.h"

#include <chrono>
#include <iostream>
#include <ratio>
#include <string>

namespace tue
{

namespace
{
// The elapsed time is kept as an exact integer number of ticks; it is converted to floating point only once, by a
// division that is correctly rounded. Subtracting two floating point time points instead would lose precision
// (catastrophic cancellation) and multiplying by an inexact constant such as 0.001 adds a second rounding error.
template <typename Period> long double toLongDouble(const Timer::Clock::duration& duration)
{
    return std::chrono::duration<long double, Period>(duration).count();
}

template <typename Period> long double nowSinceEpoch()
{
    // Epoch time requires the system clock; the steady clock has an unspecified epoch.
    return std::chrono::duration<long double, Period>(std::chrono::system_clock::now().time_since_epoch()).count();
}
} // namespace

// ----------------------------------------------------------------------------------------------------

Timer::Timer() : running_(false) {}

void Timer::start()
{
    running_ = true;
    start_point_ = Clock::now(); // last, so the overhead of this function is not measured
}

void Timer::stop()
{
    end_point_ = Clock::now(); // first, so the overhead of this function is not measured
    running_ = false;
}

Timer::Clock::duration Timer::getElapsedDuration() const
{
    // Use a local end point when running, so this const function is free of side effects and data races.
    return (running_ ? Clock::now() : end_point_) - start_point_;
}

long double Timer::getElapsedTimeInNanoSec() const
{
    return toLongDouble<std::nano>(getElapsedDuration());
}

long double Timer::getElapsedTimeInMicroSec() const
{
    return toLongDouble<std::micro>(getElapsedDuration());
}

long double Timer::getElapsedTimeInMilliSec() const
{
    return toLongDouble<std::milli>(getElapsedDuration());
}

long double Timer::getElapsedTimeInSec() const
{
    return toLongDouble<std::ratio<1>>(getElapsedDuration());
}

long double Timer::getElapsedTime() const
{
    return this->getElapsedTimeInSec();
}

void Timer::printLastElapsedTime(const std::string& m) const
{
    std::cout << m << " (sec): " << getElapsedTimeInSec() << '\n';
}

void Timer::printLastElapsedTimeMSec(const std::string& m) const
{
    printLastElapsedTimeInMilliSec(m);
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

long double Timer::nowMicroSec()
{
    return nowSinceEpoch<std::micro>();
}

long double Timer::nowMilliSec()
{
    return nowSinceEpoch<std::milli>();
}

long double Timer::now()
{
    return nowSinceEpoch<std::ratio<1>>();
}

} // namespace tue
