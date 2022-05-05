//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
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

#ifndef TUE_PROFILING_TIMER_H_
#define TUE_PROFILING_TIMER_H_

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

#include <iostream>
#include <string>

namespace tue
{

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();

    /**
     * @brief Get running status of the timer
     * @return running or not running
     */
    inline bool running() const { return running_; }

    /**
     * @brief Alias of Timer::getElapsedTimeInSec
     * @return elapsed time in seconds
     */
    long double getElapsedTime() const;

    /**
     * @brief Get elasped time in seconds
     * @return elapsed time in seconds
     */
    long double getElapsedTimeInSec() const;

    /**
     * @brief Get elasped time in milli-seconds
     * @return elapsed time in milli-seconds
     */
    long double getElapsedTimeInMilliSec() const;

    /**
     * @brief Get elasped time in micro-seconds
     * @return elapsed time in micro-seconds
     */
    long double getElapsedTimeInMicroSec() const;

    void printLastElapsedTime(std::string);
    void printLastElapsedTimeMSec(std::string);


private:
    /**
     * @brief Running flag
     */
    bool running_;

#ifdef WIN32
    /**
     * @brief ticks per second
     */
    LARGE_INTEGER frequency_;

    /**
     * @brief startCount
     */
    LARGE_INTEGER startCount_;

    /**
     * @brief endCount
     */
    LARGE_INTEGER endCount_;
#else
    /**
     * @brief Start counter
     */
    timeval start_count_;

    /**
     * @brief End counter
     */
    timeval end_count_;
#endif
};

}

#define TIMER_START Timer t; t.start();
#define TIMER_STOP(x) (t. printLastElapsedTime(x))
#define TIMER_STOP_M(x) (t.printLastElapsedTimeMSec(x))

#endif // TUE_PROFILING_TIMER_H_
