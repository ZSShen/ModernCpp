#pragma once

#include <chrono>
#include <iostream>


using MicroSeconds = std::chrono::microseconds;
using MilliSeconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;


auto makeStamp()
{
    return std::chrono::steady_clock::now();
}

template<typename T = MilliSeconds>
auto makeDuration(decltype(makeStamp()) time)
{
    return std::chrono::duration_cast<T>(
        std::chrono::steady_clock::now() - time).count();
}
