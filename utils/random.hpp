#ifndef UTILS_RANDOM_HPP
#define UTILS_RANDOM_HPP

namespace utils {

int random(int min, int max)
{
  return rand() % max + min;
}

}

#endif // UTILS_RANDOM_HPP
