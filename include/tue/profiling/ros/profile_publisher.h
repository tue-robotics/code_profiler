#ifndef TUE_PROFILING_ROS_PROFILE_PUBLISHER_H_
#define TUE_PROFILING_ROS_PROFILE_PUBLISHER_H_

#include <ros/publisher.h>

namespace tue
{

class Profiler;

class ProfilePublisher {

public:

    ProfilePublisher();

    ProfilePublisher(const Profiler& profiler);

    ProfilePublisher(const Profiler* profiler);

    virtual ~ProfilePublisher();

    void initialize(const Profiler& profiler);

    void initialize(const Profiler* profiler);

    void publish() const;

protected:

    void initialize();

    const Profiler* profiler_;

    ros::Publisher pub_stats_;

};

}

#endif
