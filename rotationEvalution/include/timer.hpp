
#ifndef OPENMVG_SYSTEM_TIMER_HPP
#define OPENMVG_SYSTEM_TIMER_HPP

#include <chrono>
#include <iostream>

/**
* @brief Timer class with microsecond accuracy.
*/
class Timer
{
public:
    
    /**
    * @brief Default constructor
    */
    Timer();
    
    /**
    * @brief Reset the timer to zero.
    */
    void reset();
    
    /**
    * @brief Get the elapsed time in seconds.
    * @return Time elapsed in second
    */
    double elapsed() const;
    
    
    /**
    * @brief Get the elapsed time in milliseconds.
    * @return Time elapsed in milliseconds
    */
    double elapsedMs() const;
private:
    std::chrono::high_resolution_clock::time_point start_;
};

/**
* print the elapsed time
* @brief out Stream in which time is written
* @param tim Timer to output
* @return stream after write operation
*/
std::ostream& operator << ( std::ostream& out , const Timer& tim );

#endif // OPENMVG_SYSTEM_TIMER_HPP
