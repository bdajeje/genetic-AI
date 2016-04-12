#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "graphics/animation.hpp"

namespace models {

class Player final : public sf::Drawable
{
  public:

    Player();

    void update(const sf::Time& elapsed_time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    void jump();
    void standUp();
    void standDown();

    bool isCrouched() const { return _current_animation == &_animation_down; }
    bool isJumping() const;

    const sf::Vector2f& getPosition() const { return _sprite.getPosition(); }
    unsigned int getWidth() const { return _sprite.getGlobalBounds().width; }

  private:

    void setAnimation(graphics::Animation& animation);    

  private:

    // Target position to reach
    float _target_y_position;

    // Player sprite
    sf::Sprite _sprite;

    // Player stands up animation
    graphics::Animation _animation_up;

    // Player stands down animation
    graphics::Animation _animation_down;

    // Current animation
    graphics::Animation* _current_animation {nullptr};

    // Pixels / ms
    static constexpr float _speed {0.35};

    // Player default and starting position
    static constexpr float _default_y_pos {340};

    // Jump height
    static constexpr float _jump_height {150};
};

}

#endif // PLAYER_HPP
