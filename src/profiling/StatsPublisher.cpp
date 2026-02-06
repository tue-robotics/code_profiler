#include "profiling/StatsPublisher.h"

#include <rclcpp/rclcpp.hpp>
#include <code_profiler/msg/statistics.hpp>
#include <iostream>

// ----------------------------------------------------------------------------------------------------

StatsPublisher::StatsPublisher() : node_(nullptr)
{
}

// ----------------------------------------------------------------------------------------------------

StatsPublisher::~StatsPublisher()
{
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::initialize(rclcpp::Node * node)
{
    if (!node)
    {
        std::cerr << "code_profiler: StatsPublisher::initialize() - Node is a nullptr." << std::endl;
        return;
    }

    node_ = node;
    pub_stats_ = node_->create_publisher<code_profiler::msg::Statistics>("profiler_stats", 1);
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::startTimer(const std::string & label)
{
    timers_[label].start();
    stack_.push(ScopeStat(label));
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::stopTimer(const std::string & label)
{
    std::map<std::string, Timer>::iterator it = timers_.find(label);
    if (it == timers_.end())
    {
        std::cout << "code_profiler: no timer found for label '" << label << "'." << std::endl;
    }
    else
    {
        it->second.stop();
    }

    if (!stack_.empty())
    {
        stack_.top().timer.stop();
        stack_.pop();
    }
    else
    {
        std::cout << "code_profiler: stopTimer() called, but no timer is active." << std::endl;
    }
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::publish() const
{
    if (!pub_stats_)
    {
        std::cout << "code_profiler: StatsPublisher not initialized." << std::endl;
        return;
    }

    code_profiler::msg::Statistics msg;
    for (std::map<std::string, Timer>::const_iterator it = timers_.begin(); it != timers_.end(); ++it)
    {
        msg.labels.push_back(it->first);
        msg.time_secs.push_back(it->second.getElapsedTimeInSec());
    }

    pub_stats_->publish(msg);
}
