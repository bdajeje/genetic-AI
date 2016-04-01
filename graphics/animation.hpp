#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>

#include <SFML/Graphics.hpp>

namespace graphics {

/*! Link an animation to a sprite */
class Animation final
{
  public:

    Animation(const std::string& textures_filename, unsigned int texture_width, unsigned int texture_height, unsigned int sprite_time);

    void update(const sf::Time& elapsed_time);

    void setSprite(sf::Sprite* sprite);

  private:

    void updateSprite();

  private:

    unsigned int _texture_width;
    unsigned int _texture_height;
    unsigned int _elapsed_time {0}; // milliseconds
    unsigned int _time_between_textures; // milliseconds
    const sf::Texture& _texture;
    sf::Sprite* _sprite;
};

}

#endif // ANIMATION_HPP
