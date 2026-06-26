#include "profiling/StatsPublisher.h"

#include <code_profiler_interfaces/msg/statistics.hpp> // IWYU pragma: keep
#include <iostream>
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp> // IWYU pragma: keep

// ----------------------------------------------------------------------------------------------------

StatsPublisher::StatsPublisher() : node_(nullptr) {}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::initialize(rclcpp::Node* node)
{
    if (!node)
    {
        std::cerr << "code_profiler: StatsPublisher::initialize() - Node is a nullptr." << '\n';
        return;
    }

    node_ = node;
    // NOLINTNEXTLINE(misc-include-cleaner) - Statistics is provided by the generated <...statistics.hpp> kept above
    pub_stats_ = node_->create_publisher<code_profiler_interfaces::msg::Statistics>("profiler_stats", 1);
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::startTimer(const std::string& label)
{
    timers_[label].start();
    stack_.emplace(label);
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::stopTimer(const std::string& label)
{
    auto it = timers_.find(label);
    if (it == timers_.end())
    {
        std::cout << "code_profiler: no timer found for label '" << label << "'." << '\n';
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
        std::cout << "code_profiler: stopTimer() called, but no timer is active." << '\n';
    }
}

// ----------------------------------------------------------------------------------------------------

void StatsPublisher::publish() const
{
    if (!pub_stats_)
    {
        std::cout << "code_profiler: StatsPublisher not initialized." << '\n';
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
