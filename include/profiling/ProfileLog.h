#ifndef PROFILING_PROFILELOG_H_
#define PROFILING_PROFILELOG_H_
#ifdef PROFILEAPP
#include <iostream>
#include <vector>
#include <map>

struct Statistics;
class ProfileLog
{
public:
    static void PrintLog(std::ostream& stream, std::vector< std::map< std::string, Statistics >* >&  stats, double totalTime);
    static void PrintLog(std::vector< std::map< std::string, Statistics >* >& stats, double totalTime);

};
#endif

#endif // PROFILING_PROFILELOG_H_
