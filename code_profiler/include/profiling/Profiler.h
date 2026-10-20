#ifndef PROFILER_H
#define PROFILER_H

#include "profiling/ProfileLog.h"
#include "Timer.h"

#include <algorithm>
#include <chrono>
#include <iterator>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

inline const std::string LOGGING_LOCATION = "~/profile_logs";

struct Statistics
{
    Timer timer;
    double msec; // derived from total, kept for backwards compatibility
    int callAmount;
    Timer::Clock::duration total{}; // exact sum of all calls, free of accumulated rounding errors

    std::string parent;
    bool running;

    Statistics() : msec(0), callAmount(0), parent(""), running(true) {}
    Statistics(std::string p) : msec(0), callAmount(0), parent(p), running(true) {}
};

typedef std::map<std::string, Statistics> FunctionStats;
typedef std::map<std::string, Statistics>* FunctionStatsPtr;

class ThreadProfiler
{

public:
    ThreadProfiler()
    {
        threadId = std::this_thread::get_id();
        currentlyProfiling = "";
        stats = new FunctionStats();
    }
    virtual ~ThreadProfiler();

    static void Start(const std::string& name);
    static void Stop(const std::string& name);
    static FunctionStatsPtr ReturnCurrentStatistics();

    FunctionStatsPtr stats;

private:
    std::thread::id threadId;
    std::string currentlyProfiling;
    std::vector<std::string> activeScopes; // innermost last; currentlyProfiling is the innermost or empty
};

class ThreadProfilerManager
{
public:
    ThreadProfilerManager();

    inline ThreadProfiler& Profiler()
    {
        if (!profiler)
        {
            profiler = std::make_unique<ThreadProfiler>();
            std::lock_guard<std::mutex> lock(mutex);
            threadStats.push_back(profiler.get()->stats);
        }

        return *profiler.get();
    }

    ~ThreadProfilerManager();

private:
    static inline thread_local std::unique_ptr<ThreadProfiler> profiler = nullptr;
    std::mutex mutex;
    std::vector<FunctionStatsPtr> threadStats;
    Timer timer;
};

// Global threadProfiler, a single instance for the whole program (instead of one per translation unit)
inline ThreadProfilerManager manager;

// Implementation of ThreadProfiler methods
inline void ThreadProfiler::Start(const std::string& name)
{
    ThreadProfiler& instance = manager.Profiler();
    // Single lookup; the parent is only recorded on the first call
    auto mapIt = instance.stats->find(name);
    if (mapIt == instance.stats->end())
    {
        mapIt = instance.stats->emplace(name, Statistics(instance.currentlyProfiling)).first;
    }
    mapIt->second.running = true;
    mapIt->second.timer.start();
    instance.activeScopes.push_back(name);
    instance.currentlyProfiling = name;
}

inline void ThreadProfiler::Stop(const std::string& name)
{
    ThreadProfiler& instance = manager.Profiler();
    FunctionStats::iterator mapIt = instance.stats->find(name);

    // Ignore unknown and already stopped scopes, so an interval is never counted twice
    if (mapIt == instance.stats->end() || !mapIt->second.running)
        return;

    mapIt->second.timer.stop();
    mapIt->second.total += mapIt->second.timer.getElapsedDuration();
    mapIt->second.msec = std::chrono::duration<double, std::milli>(mapIt->second.total).count();
    mapIt->second.callAmount++;
    mapIt->second.running = false;

    // Return to the enclosing active scope. The recorded parent can't be used: it is from the first call only, which
    // may have come from another caller. Scopes may also be stopped out of order.
    auto scopeIt = std::find(instance.activeScopes.rbegin(), instance.activeScopes.rend(), name);
    if (scopeIt != instance.activeScopes.rend())
        instance.activeScopes.erase(std::next(scopeIt).base());
    instance.currentlyProfiling = instance.activeScopes.empty() ? "" : instance.activeScopes.back();
}

inline FunctionStatsPtr ThreadProfiler::ReturnCurrentStatistics()
{
    ThreadProfiler& instance = manager.Profiler();
    return instance.stats;
}

inline ThreadProfiler::~ThreadProfiler() {}

inline ThreadProfilerManager::ThreadProfilerManager()
{
    timer.start();
}

inline ThreadProfilerManager::~ThreadProfilerManager()
{
    timer.stop();
    ProfileLog::PrintLog(threadStats, timer.getElapsedTimeInMilliSec());
    for (FunctionStatsPtr stats : threadStats)
        delete stats;
}

#endif
