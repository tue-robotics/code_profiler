#include "profiling/ProfileLog.h"
#include "profiling/Profiler.h"

#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

void ProfileLog::PrintLog(std::ostream& stream,
                          std::vector<std::map<std::string, Statistics>*>& stats,
                          double totalTime)
{
    int thread_num = 0;
    for (auto& stat : stats)
    {
        stream << std::fixed;
        stream << std::left;
        stream << "Thread info for thread " << ++thread_num << ":" << '\n';

        stream << std::setw(15) << "Name:" << std::setw(20) << "Total Time:" << std::setw(30) << "Average time"
               << std::setw(30) << "Relative Time:" << std::setw(15) << "Called #:" << '\n';
        stream << std::setfill('-') << std::setw(100) << "" << '\n';
        stream << std::setfill(' ');

        for (const auto& it2 : *stat)
        {
            const Statistics& function_stats = it2.second;
            if (!function_stats.running)
                stream << std::setw(15) << it2.first << std::setw(20) << function_stats.msec << std::setw(30)
                       << (function_stats.msec / function_stats.callAmount) << std::setw(30)
                       << function_stats.msec / totalTime << std::setw(15) << function_stats.callAmount << '\n';
            else
                stream << std::setw(15) << it2.first << "ERROR(" << it2.first << ") was never stopped!" << '\n';
        }
        stream << '\n';
    }
}

void ProfileLog::PrintLog(std::vector<std::map<std::string, Statistics>*>& stats, double totalTime)
{
    ProfileLog::PrintLog(std::cout, stats, totalTime);
}
