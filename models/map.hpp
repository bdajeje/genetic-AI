#ifndef MAP_HPP
#define MAP_HPP

#include <list>

#include <SFML/Graphics.hpp>

#include "models/player.hpp"

namespace models {

class Map final : public sf::Drawable
{
  public:

    Map(unsigned int width, unsigned int height);

    void update(const sf::Time& elapsed_time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    bool isCollision(const Player& player) const;

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
    std::array<sf::Sprite, 2> _backgrounds;
    std::list<sf::Sprite> _holes;
    sf::Sprite _bird;

    // Pixels / ms
    float _speed {0.3};

    // Bird has to move faster than map, this multiplicator is added to its movement
    static constexpr float _bird_speed_modificator {2};
};

}

#endif // MAP_HPP
