#include <tue/profiling/loop_timer.h>

class LoopTimerTest
{
    tue::LoopTimer lt1_, lt2_, lt3_;
    tue::Timer timer_;
    uint n_;
    double tot_;

public:
    LoopTimerTest(uint n): n_(n), tot_(0.0)
    {}

    void run( )
    {
        lt1_.start();

        for (uint i = 0; i < n_; ++i)
        {
            lt2_.start();

            for (uint j = 0; j < n_; ++j )
            {
                timer_.start();
                timer_.stop();
                tot_ += timer_.getElapsedTime();
                lt3_.start();
                lt3_.stop();
            }

            lt2_.stop();
        }

        lt3_.stop();
    }

    void printResults()
    {
        std::cout << "Total times using Kahan summation over " << n_*n_ << " loops: "
                  << "\n\ttimer1: " << lt1_.getTotalLoopTime()*1e3 << " ms"
                  << "\n\ttimer2: " << lt2_.getTotalLoopTime()*1e3 << " ms"
                  << "\n\ttimer3: " << lt3_.getTotalLoopTime()*1e3 << " ms"
                  << "\n" << std::endl;

        std::cout << "tue::timer total time: " << tot_*1e3 << " ms (without numerical error compensation)\n" << std::endl;

        std::cout << "Average times using Kahan summation over " << n_*n_ << " loops: "
                  << "\n\ttimer1: " << lt1_.getAverageLoopTime()*1e3 << " ms"
                  << "\n\ttimer2: " << lt2_.getAverageLoopTime()*1e3 << " ms"
                  << "\n\ttimer3: " << lt3_.getAverageLoopTime()*1e3 << " ms"
                  << "\n" << std::endl;

        std::cout << "tue::timer average time: " << tot_/(n_*n_)*1e3 << " ms (without numerical error compensation)\n" << std::endl;
    }
};

int main()
{
    LoopTimerTest ltt(1000);

    ltt.run();

    ltt.printResults();

    return 0;
}
