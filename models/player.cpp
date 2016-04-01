#include "player.hpp"

namespace models {

Player::Player()
  : _target_y_position {_default_y_pos}
  , _animation_up {"player.png", 60, 102, 200}
  , _animation_down {"player_crouch.png", 60, 102, 200}
{
  _sprite.setPosition( 50, _default_y_pos );
  standUp();
}

void Player::update(const sf::Time& elapsed_time)
{
  _current_animation->update(elapsed_time);

  // If player needs to reach a position
  const sf::Vector2f& position = _sprite.getPosition();
  if( position.y != _target_y_position )
  {
    float move = elapsed_time.asMilliseconds() * _speed;
    // Going up
    if( _sprite.getPosition().y > _target_y_position )
    {
      _sprite.setPosition(position.x, std::max(position.y - move, _target_y_position));

      // If we reach max high, now let's go down
      if( position.y == _target_y_position )
        _target_y_position = _default_y_pos;
    }
    // Going down
    else
      _sprite.setPosition(position.x, std::min(position.y + move, _target_y_position));
  }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_sprite, states);
}

bool Player::isJumping() const
{
  return _sprite.getPosition().y != _default_y_pos;
}

void Player::jump()
{
  // If player's still in the air, exit
  if( isJumping() )
    return;

  standUp();

  _target_y_position = _default_y_pos - _jump_height;
}

void Player::reset()
{
  _target_y_position = _default_y_pos;
  _sprite.setPosition( _sprite.getPosition().x, _target_y_position );
  standUp();
}

void Player::setAnimation(graphics::Animation& animation)
{
  if( _current_animation == &animation )
    return;

  _current_animation = &animation;
  _current_animation->setSprite(&_sprite);
}

void Player::standUp()
{
  setAnimation( _animation_up );
}

void Player::standDown()
{
  if(isJumping())
    return;

  setAnimation( _animation_down );
}

}
