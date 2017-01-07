#include "WallClockTimer.hpp"
#include <iostream>


int main()
{
    auto start = makeStamp();

    // Run the task.

    auto cost = makeDuration<MilliSeconds>(start);
    std::cout << "Wall clock time: " << cost << " milliseconds" << std::endl;

    return 0;
}
