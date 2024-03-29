#include "tue/profiling/ros/profile_publisher.h"
#include "tue/profiling/profiler.h"

#include <std_msgs/String.h>
#include <ros/node_handle.h>

#include <sstream>

namespace tue
{

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher() : profiler_(nullptr)
{
}

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher(const Profiler& profiler) : profiler_(&profiler)
{
    initialize();
}

// ----------------------------------------------------------------------------------------------------

ProfilePublisher::ProfilePublisher(const Profiler* profiler) : profiler_(profiler)
{
    initialize();
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize(const Profiler& profiler)
{
    profiler_ = &profiler;
    initialize();
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize(const Profiler* profiler)
{
    profiler_ = profiler;
    initialize();
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::initialize()
{
    assert((void("Profiler is a nullptr"), profiler_));
    ros::NodeHandle nh("~");
    pub_stats_ = nh.advertise<std_msgs::String>("profile/" + profiler_->getName(), 1);
}

// ----------------------------------------------------------------------------------------------------

void ProfilePublisher::publish() const
{
    if (pub_stats_.getTopic() == "")
    {
        std::cout << "[tue::Profiler] ProfilePublisher not initialized." << std::endl;
        return;
    }

    std::stringstream s;
    s << *profiler_;

    std_msgs::String msg;
    msg.data = s.str();

    pub_stats_.publish(msg);
}

}
