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
    Timer();      // default constructor
    ~Timer();       // default destructor

    void   start();      // start timer
    void   stop();      // stop the timer
    double getElapsedTime() const;       // get elapsed time in second
    double getElapsedTimeInSec() const;       // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec() const;       // get elapsed time in milli-second (10^-3)
    double getElapsedTimeInMicroSec() const;       // get elapsed time in micro-second (10^-6)
    double getElapsedTimeInNanoSec() const;       // get elapsed time in nano-second (10^-9)
    void   printLastElapsedTime(std::string) const;          // TUe
    void   printLastElapsedTimeInMilliSec(std::string) const;          // TUe
    void   printLastElapsedTimeInMicroSec(std::string) const;          // TUe
    void   printLastElapsedTimeInNanoSec(std::string) const;          // TUe

protected:

private:
    std::chrono::high_resolution_clock::time_point startPoint;
    mutable std::chrono::high_resolution_clock::time_point endPoint;
    bool stopped;      // stop flag
};

#endif // TIMER_H_DEF
