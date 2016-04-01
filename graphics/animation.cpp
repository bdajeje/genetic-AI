#include "animation.hpp"

#include "managers/texturemanager.hpp"

namespace graphics {

Animation::Animation(const std::string& textures_filename, unsigned int texture_width, unsigned int texture_height, unsigned int sprite_time)
  : _texture_width{texture_width}
  , _texture_height{texture_height}
  , _time_between_textures {sprite_time}
  , _texture {texture::TextureManager::get(textures_filename)}
{}

void Animation::setSprite(sf::Sprite* sprite)
{
  _sprite = sprite;
  if(!_sprite)
    return;

  _sprite->setTexture( _texture );
  updateSprite();
}

void Animation::update(const sf::Time& elapsed_time)
{
  // Do we need a sprite update
  bool need_update = false;
  if( elapsed_time.asMilliseconds() - _elapsed_time >= _time_between_textures )
    need_update = true;

  _elapsed_time += elapsed_time.asMilliseconds();

  if(need_update)
    updateSprite();
}

void Animation::updateSprite()
{
  if(!_sprite)
    return;

  int texture_nbr = _elapsed_time / _time_between_textures;
  uint x_offset = (texture_nbr * _texture_width);

  if(x_offset >= _sprite->getTexture()->getSize().x)
  {
    _elapsed_time = 0;
    x_offset = 0;
  }

  _sprite->setTextureRect(sf::IntRect(x_offset, 0, _texture_width, _texture_height));
}

}
