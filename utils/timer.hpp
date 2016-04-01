#ifndef TIMER_HPP
#define TIMER_HPP

#include <SFML/System/Clock.hpp>

namespace utils {
namespace time {

class Timer final
{
  public:

    Timer() = default;

    /*! Start the timer */
    void start();

    /*! Restart the timer
     *  \returns time since started
     */
    sf::Time restart();

    /*! Pause the timer
     *  \returns current elapsed time
     */
    sf::Time pause();

    /*! Returns time since started */
    sf::Time getElapsedTime() const;

  private:

    sf::Clock _clock;
    sf::Time _elapsed_time {sf::Time::Zero};
    bool _paused {true};
};

} // time
} // utils

#endif // TIMER_HPP
