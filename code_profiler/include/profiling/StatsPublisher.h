#ifndef CODE_PROFILER_STATS_PUBLISHER_H_
#define CODE_PROFILER_STATS_PUBLISHER_H_

#include <code_profiler/msg/statistics.hpp>
#include <map>
#include <rclcpp/rclcpp.hpp>
#include <stack>
#include <string>

#include "profiling/Timer.h"

struct ScopeStat
{

    ScopeStat() {}

    ScopeStat(const std::string& name_) : name(name_) {}

    std::string name;
    Timer timer;
};

class StatsPublisher
{

public:
    StatsPublisher();

    virtual ~StatsPublisher();

    void initialize(rclcpp::Node* node);

    void startTimer(const std::string& label);

    void stopTimer(const std::string& label);

    void publish() const;

protected:
    rclcpp::Publisher<code_profiler::msg::Statistics>::SharedPtr pub_stats_;

    std::map<std::string, Timer> timers_;

    std::stack<ScopeStat> stack_;

    rclcpp::Node* node_;
};

#endif
