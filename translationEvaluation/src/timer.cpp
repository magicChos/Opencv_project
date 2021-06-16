
// #include "timer.hpp"
#include "include/timer.hpp"
#include "include/logger.h"
#include <ratio>
#include <type_traits>

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    start_ = std::chrono::high_resolution_clock::now();
}

double Timer::elapsed() const
{
    const auto end_ = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end_ - start_).count();
}

double Timer::elapsedMs() const
{
    const auto end_ = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count();
}

std::ostream& operator << (std::ostream& str, const Timer& t)
{
    return str << t.elapsed() << " s elapsed";
}

