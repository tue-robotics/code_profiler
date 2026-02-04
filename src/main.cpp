#include "profiling/Timer.h"
#include "profiling/Profiler.h"

#include <iostream>
#include <thread>

Timer t;
int testProf()
{/*
    ThreadProfiler::Start("ProfileThis");
    int i  = sleep(1);
    ThreadProfiler::Start("ProfileThis2");
    sleep(2);
    ThreadProfiler::Stop("ProfileThis2");
    ThreadProfiler::Stop("ProfileThis");*/
    t.start();
    ThreadProfiler::Start("Bla");
    ThreadProfiler::Stop("Bla");
    t.stop();
    std::cout << "Elapsed: " << t.getElapsedTimeInMilliSec() << std::endl;
    return 0;
}

void testProf2()
{
    std::thread t1(&testProf);
    t1.join();
    std::thread t2(&testProf);
    t2.join();
}

int main()
{
    return testProf();
}
