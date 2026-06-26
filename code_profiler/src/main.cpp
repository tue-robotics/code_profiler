#include "profiling/Profiler.h"
#include "profiling/Timer.h"

#include <iostream>
#include <thread>

namespace
{
int testProf()
{ /*
     ThreadProfiler::Start("ProfileThis");
     int i  = sleep(1);
     ThreadProfiler::Start("ProfileThis2");
     sleep(2);
     ThreadProfiler::Stop("ProfileThis2");
     ThreadProfiler::Stop("ProfileThis");*/
    Timer t;
    t.start();
    ThreadProfiler::Start("Bla");
    ThreadProfiler::Stop("Bla");
    t.stop();
    std::cout << "Elapsed: " << t.getElapsedTimeInMilliSec() << '\n';
    return 0;
}

[[maybe_unused]]
void testProf2()
{
    std::thread t1(&testProf);
    t1.join();
    std::thread t2(&testProf);
    t2.join();
}
} // namespace

int main()
{
    return testProf();
}
