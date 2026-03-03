#include "profiling/StatsPublisher.h"

#include <code_profiler_interfaces/msg/statistics.hpp>
#include <iostream>
#include <rclcpp/rclcpp.hpp>

// ----------------------------------------------------------------------------------------------------

StatsPublisher::StatsPublisher() : node_(nullptr) {}

// ----------------------------------------------------------------------------------------------------

StatsPublisher::~StatsPublisher() = default;

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::initialize(rclcpp::Node* node)
{
    if (!node)
    {
        std::cerr << "code_profiler_interfaces: StatsPublisher::initialize() - Node is a nullptr." << std::endl;
        return;
    }

    node_ = node;
    pub_stats_ = node_->create_publisher<code_profiler_interfaces::msg::Statistics>("profiler_stats", 1);
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::startTimer(const std::string& label)
{
    timers_[label].start();
    stack_.push(ScopeStat(label));
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::stopTimer(const std::string& label)
{
    auto it = timers_.find(label);
    if (it == timers_.end())
    {
        std::cout << "code_profiler_interfaces: no timer found for label '" << label << "'." << std::endl;
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
        std::cout << "code_profiler_interfaces: stopTimer() called, but no timer is active." << std::endl;
    }
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::publish() const
{
    if (!pub_stats_)
    {
        std::cout << "code_profiler_interfaces: StatsPublisher not initialized." << std::endl;
        return;
    }

    code_profiler_interfaces::msg::Statistics msg;
    for (const auto& timer : timers_)
    {
        msg.labels.push_back(timer.first);
        msg.time_secs.push_back(timer.second.getElapsedTimeInSec());
    }

    pub_stats_->publish(msg);
}
