#include "timer.hpp"

namespace utils {
namespace time {

sf::Time Timer::restart()
{
  const auto previous_elapsed_time = pause();

  // Reset elapsed time and clock
  _elapsed_time = sf::Time::Zero;
  start();

  return previous_elapsed_time;
}

sf::Time Timer::pause()
{
  if( !_paused )
  {
    _elapsed_time += _clock.getElapsedTime();
    _paused = true;
  }

  return getElapsedTime();
}

void Timer::start()
{
  if(_paused)
  {
    _paused = false;
    _clock.restart();
  }
}

sf::Time Timer::getElapsedTime() const
{
  return ( _paused ) ? _elapsed_time : _elapsed_time + _clock.getElapsedTime();
}

}
}
