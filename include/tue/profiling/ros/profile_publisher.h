#ifndef TUE_PROFILING_ROS_PROFILE_PUBLISHER_H_
#define TUE_PROFILING_ROS_PROFILE_PUBLISHER_H_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <memory>

namespace tue
{

class Profiler;

class ProfilePublisher {

public:

    ProfilePublisher();

    ProfilePublisher(const Profiler& profiler, rclcpp::Node* node);

    ProfilePublisher(const Profiler* profiler, rclcpp::Node* node);

    virtual ~ProfilePublisher() = default;

    void initialize(const Profiler& profiler, rclcpp::Node* node);

    void initialize(const Profiler* profiler, rclcpp::Node* node);

    void publish() const;

protected:

    void initialize(rclcpp::Node* node);

    const Profiler* profiler_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_stats_;

    rclcpp::Node* node_;

};

}

#endif
