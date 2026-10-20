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

#ifndef TUE_PROFILING_TIMER_H_
#define TUE_PROFILING_TIMER_H_

#include <chrono>
#include <iostream> // IWYU pragma: export - kept for backwards compatibility of code relying on it
#include <string>

namespace tue
{

class Timer
{
public:
    /**
     * @brief Clock used for measuring intervals. Monotonic, unlike std::chrono::high_resolution_clock, which is an
     * alias of the adjustable std::chrono::system_clock in libstdc++.
     */
    using Clock = std::chrono::steady_clock;

    Timer();
    ~Timer() = default;

    void start();
    void stop();

    /**
     * @brief Get running status of the timer
     * @return running or not running
     */
    inline bool running() const { return running_; }

    /**
     * @brief Get the exact elapsed time as an integer number of clock ticks. When running, the time until now is used.
     * @return elapsed time
     */
    Clock::duration getElapsedDuration() const;

    /**
     * @brief Get the exact elapsed time as an integer number of clock ticks. When running, the time until the given
     * time point is used, so multiple timers can be read at the same instant.
     * @param now Time point of Clock, not before the start of the timer
     * @return elapsed time
     */
    Clock::duration getElapsedDuration(Clock::time_point now) const;

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

    /**
     * @brief Get elasped time in nano-seconds
     * @return elapsed time in nano-seconds
     */
    long double getElapsedTimeInNanoSec() const;

    void printLastElapsedTime(const std::string&) const;
    void printLastElapsedTimeMSec(const std::string&) const;
    void printLastElapsedTimeInMilliSec(const std::string&) const;
    void printLastElapsedTimeInMicroSec(const std::string&) const;
    void printLastElapsedTimeInNanoSec(const std::string&) const;

    /**
     * @brief Get the current time in micro-seconds since epoch
     * @return Epoch time in micro-seconds
     */
    static long double nowMicroSec();

    /**
     * @brief Get the current time in milli-seconds since epoch
     * @return Epoch time in milli-seconds
     */
    static long double nowMilliSec();

    /**
     * @brief Get the current time in seconds since epoch
     * @return Epoch time in seconds
     */
    static long double now();

private:
    /**
     * @brief Start time point
     */
    Clock::time_point start_point_;

    /**
     * @brief End time point
     */
    Clock::time_point end_point_;

    /**
     * @brief Running flag
     */
    bool running_;
};

} // namespace tue

#define TIMER_START                                                                                                    \
    Timer t;                                                                                                           \
    t.start();
#define TIMER_STOP(x)   (t.printLastElapsedTime(x))
#define TIMER_STOP_M(x) (t.printLastElapsedTimeMSec(x))

#endif // TUE_PROFILING_TIMER_H_
