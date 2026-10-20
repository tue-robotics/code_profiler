//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
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

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#include <chrono>
#include <string>

class Timer
{
public:
    // TUe: monotonic clock; std::chrono::high_resolution_clock is the adjustable system_clock in libstdc++
    using Clock = std::chrono::steady_clock;

    Timer();
    ~Timer() = default;

    void start(); // start timer
    void stop(); // stop the timer
    Clock::duration getElapsedDuration() const; // TUe: get exact elapsed time in clock ticks
    double getElapsedTime() const; // get elapsed time in second
    double getElapsedTimeInSec() const; // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec() const; // get elapsed time in milli-second (10^-3)
    double getElapsedTimeInMicroSec() const; // get elapsed time in micro-second (10^-6)
    double getElapsedTimeInNanoSec() const; // get elapsed time in nano-second (10^-9)
    void printLastElapsedTime(const std::string&) const; // TUe
    void printLastElapsedTimeInMilliSec(const std::string&) const; // TUe
    void printLastElapsedTimeInMicroSec(const std::string&) const; // TUe
    void printLastElapsedTimeInNanoSec(const std::string&) const; // TUe

protected:
private:
    Clock::time_point startPoint;
    Clock::time_point endPoint;
    bool stopped; // stop flag
};

#endif // TIMER_H_DEF
