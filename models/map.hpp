#ifndef MAP_HPP
#define MAP_HPP

#include <list>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "models/player.hpp"

namespace models {

class Map final : public sf::Drawable
{
  public:

    /*! Constructor
     *  \param player_front_x - X position of the front of the player (player x pos + player width)
     */
    Map(unsigned int width, unsigned int height, float player_front_x);

    void update(const sf::Time& elapsed_time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    bool isCollision(const Player& player) const;

    /*! Get next hole after a x distance
     * \param limit - find first hole after this X limit
     * \returns hole sprite
     */
    const sf::Sprite& getNextHole(float limit) const;

    /*! Get bird x position
     * \returns bird x position
     */
    float getBirdXPos() const { return _bird.getPosition().x; }

  private:

    void updateHoles(float move);
    void generateHole();
    void placeBird();
    bool isBirdCollision(const Player& player) const;
    bool isHoleCollision(const Player& player) const;

    static bool isCollide(float a_left, float a_right, float b_left, float b_right);

  private:

    int _width; // we need an int here to be able to get minus _width
    unsigned int _height;
    float _player_x;
    std::array<sf::Sprite, 2> _backgrounds;
    std::list<sf::Sprite> _holes;
    sf::Sprite _bird;
    graphics::Animation _bird_animation;

    // Pixels / ms
    static constexpr float _speed {0.3};

    // Bird has to move faster than map, this multiplicator is added to its movement
    static constexpr float _bird_speed_modificator {2.5};

    // Minimum X distance between two holes
    static constexpr float _min_distance_between_holes {600};
};

}

#endif // MAP_HPP
