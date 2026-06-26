#include "tue/profiling/ros/profile_publisher.h"
#include "tue/profiling/profiler.h"

#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp> // IWYU pragma: keep
#include <std_msgs/msg/string.hpp> // IWYU pragma: keep

#include <iostream>
#include <sstream>

namespace tue
{

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher() : profiler_(nullptr), node_(nullptr) {}

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher(const Profiler& profiler, rclcpp::Node* node) : profiler_(&profiler), node_(node)
{
    initialize(node);
}

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher(const Profiler* profiler, rclcpp::Node* node) : profiler_(profiler), node_(node)
{
    initialize(node);
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize(const Profiler& profiler, rclcpp::Node* node)
{
    profiler_ = &profiler;
    node_ = node;
    initialize(node);
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize(const Profiler* profiler, rclcpp::Node* node)
{
    profiler_ = profiler;
    node_ = node;
    initialize(node);
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize(rclcpp::Node* node)
{
    if (!profiler_)
    {
        std::cerr << "[tue::Profiler] ProfilePublisher: Profiler is a nullptr" << '\n';
        return;
    }

    if (!node)
    {
        std::cerr << "[tue::Profiler] ProfilePublisher: Node is a nullptr" << '\n';
        return;
    }

    node_ = node;
    // NOLINTNEXTLINE(misc-include-cleaner) - String is provided by the generated <std_msgs/msg/string.hpp> kept above
    pub_stats_ = node_->create_publisher<std_msgs::msg::String>("profile/" + profiler_->getName(), 1);
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::publish() const
{
    if (!pub_stats_)
    {
        std::cout << "[tue::Profiler] ProfilePublisher not initialized." << '\n';
        return;
    }

    std::stringstream s;
    s << *profiler_;

    std_msgs::msg::String msg;
    msg.data = s.str();

    pub_stats_->publish(msg);
}

} // namespace tue
