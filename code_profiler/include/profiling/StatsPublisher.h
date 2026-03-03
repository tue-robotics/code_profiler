#ifndef code_profiler_interfaces_STATS_PUBLISHER_H_
#define code_profiler_interfaces_STATS_PUBLISHER_H_

#include <code_profiler_interfaces_interfaces/msg/statistics.hpp>
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
    rclcpp::Publisher<code_profiler_interfaces_interfaces::msg::Statistics>::SharedPtr pub_stats_;

    std::map<std::string, Timer> timers_;

    std::stack<ScopeStat> stack_;

    rclcpp::Node* node_;
};

#endif
