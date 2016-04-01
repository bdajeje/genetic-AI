#include "map.hpp"

#include "managers/texturemanager.hpp"
#include "utils/graphics.hpp"
#include "utils/random.hpp"

namespace models {

Map::Map(unsigned int width, unsigned int height)
  : _width {static_cast<int>(width)}
  , _height {height}
  , _bird_animation {"bird.png", 4, 100}
{
  // Set backgrounds
  for(size_t i = 0; i < _backgrounds.size(); ++i)
  {
    sf::Sprite& background = _backgrounds[i];
    background.setTexture( texture::TextureManager::get("background.png") );
    const sf::FloatRect bounds = background.getGlobalBounds();
    background.setPosition(i * bounds.width, height - bounds.height);
  }

  // Set bird sprite
  _bird.setPosition(0, 310);
  _bird_animation.setSprite(&_bird);

  // Reset map data
  reset();
}

void Map::update(const sf::Time& elapsed_time)
{
  // How many pixels of moving
  float move = elapsed_time.asMilliseconds() * _speed;

  // Move backgrounds
  for(sf::Sprite& background : _backgrounds)
  {
    background.move(-move, 0);
    if(background.getPosition().x < -_width )
      background.move(_width * 2, 0);
  }

  // Move bird
  _bird_animation.update(elapsed_time);
  _bird.move(-move * _bird_speed_modificator, 0);

  // Bird goes out of screen, randomly re-place it
  const static float bird_screen_limit = - _bird.getGlobalBounds().width;
  if( _bird.getPosition().x < bird_screen_limit )
    placeBird();

  // Take care of holes (move and generate)
  updateHoles(move);
}

void Map::placeBird()
{
  _bird.move(utils::random(_width, _width * 4), 0);
}

void Map::updateHoles(float move)
{
  // Move holes
  for( sf::Sprite& hole : _holes )
    hole.move(-move, 0);

  // Check if most left hole is out of screen, if yes, remove it, and generate a new one
  sf::Sprite& hole = _holes.front();
  if(hole.getPosition().x < -hole.getGlobalBounds().width)
  {
    _holes.pop_front();
    generateHole();
  }
}

void Map::generateHole()
{
  // Starting x position
  const int x_margin = _holes.empty() ? _width : _holes.front().getPosition().x;

  // Create new hole
  _holes.emplace_back( texture::TextureManager::get("hole.png") );
  sf::Sprite& hole = _holes.back();
  const sf::FloatRect& bounds = hole.getGlobalBounds();
  hole.setPosition(std::max(x_margin + utils::random(_min_distance_between_holes, _width), _width), _height - bounds.height);

  // Resize hole width randomly
  const float width_modificator = utils::random(10, 18) / 10;
  utils::graphics::resize(hole, bounds.width * width_modificator, bounds.height);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for(const sf::Sprite& background : _backgrounds)
    target.draw(background, states);

  for( const sf::Sprite& hole : _holes )
    target.draw(hole, states);

  target.draw(_bird, states);
}

bool Map::isCollision(const Player& player) const
{
  return isHoleCollision(player) || isBirdCollision(player);
}

bool Map::isCollide(float a_left, float a_right, float b_left, float b_right)
{
  if( a_right < b_left || a_left > b_right )
    return false;

  return true;
}

bool Map::isHoleCollision(const Player& player) const
{
  if(player.isJumping())
    return false;

  // Only check first hole
  const sf::Sprite& hole           = _holes.front();
  const sf::Vector2f& hole_pos     = hole.getPosition();
  const sf::Vector2f& player_pos   = player.getPosition();
  const sf::FloatRect& hole_bounds = hole.getGlobalBounds();

  // We apply a modificator to player position and width so player's
  // sprite may enter a little bit the hole without falling
  static const float margin_modifier = 20.0; // in pixels

  return isCollide(player_pos.x + margin_modifier, player_pos.x + player.getWidth() - margin_modifier,
                   hole_pos.x, hole_pos.x + hole_bounds.width);
}

bool Map::isBirdCollision(const Player& player) const
{
  if(player.isCrouched())
    return false;

  const sf::Vector2f player_pos = player.getPosition();
  const sf::Vector2f bird_pos   = _bird.getPosition();

  // Because collision is tested against bounds, apply a
  // modifier so collision with bird is less restricted
  static const float margin_modifier = 10.0; // in pixels

  return isCollide( player_pos.x + margin_modifier, player_pos.x + player.getWidth(),
                    bird_pos.x, bird_pos.x + _bird.getGlobalBounds().width );
}

void Map::reset()
{
  // Remove all holes
  _holes.clear();

  // Generates two holes by default
  generateHole();
  generateHole();

  // Re-place bird
  placeBird();
}

const sf::Sprite& Map::getNextHole(float limit) const
{
  // Since we only have two holes in this game,
  // check if first hole respects the given limit,
  // otherwise returns the second hole.

  const sf::Sprite& first_hole = _holes.front();
  if( first_hole.getPosition().x > limit )
    return first_hole;

  return _holes.back();
}

}
