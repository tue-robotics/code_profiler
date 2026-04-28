# code_profiler

[![CI](https://github.com/tue-robotics/code_profiler/actions/workflows/main.yml/badge.svg)](https://github.com/tue-robotics/code_profiler/actions/workflows/main.yml)

Various classes to profile code in different ways, including some additional loop statistics.

## ROS2 Compatibility

This package has been fully migrated to ROS2. It provides profiling utilities that can be used standalone or integrated with ROS2 nodes.

## Key Components

- **Profiler**: Core profiling class for timing code sections
- **LoopTimer**: Timer with Kahan summation for accurate loop timing
- **ProfilePublisher**: ROS2 publisher for profiling data (requires `rclcpp::Node*`)
- **StatsPublisher**: ROS2 publisher for statistics messages (requires `rclcpp::Node*`)
- **liveprofile**: Python script for live visualization of profiling data

## Usage with ROS2

The `ProfilePublisher` and `StatsPublisher` classes are designed to be usable by other libraries. They accept an `rclcpp::Node*` pointer during initialization:

```cpp
#include <tue/profiling/profiler.h>
#include <tue/profiling/ros/profile_publisher.h>

// In your node class
tue::Profiler profiler("my_profiler");
tue::ProfilePublisher publisher(profiler, this);  // 'this' is your rclcpp::Node*

// Later, publish profiling data
publisher.publish();
```
