#ifndef AI_HPP
#define AI_HPP

#include <utility>

namespace AI {

class AI final
{
  public:

    AI() = default;

    /*! Handle inputs to return outputs
     *  \param hole_width    - width of the hole to come
     *  \param hole_distance - distance with the hole to come
     *  \param bird_distance - distance with the bird to come
     *  \return first : press jump or not
     *          second: press arrow down or not
     */
    std::pair<bool, bool> inputs(float hole_width, float hole_distance, float bird_distance);

    /*! Restart a run
     *  \param score - of the just finised run
     */
    void restart(unsigned int score);
};

}

#endif // AI_HPP
