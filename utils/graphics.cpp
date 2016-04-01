#include "utils/graphics.hpp"

namespace utils {
namespace graphics{

void centerPosition(sf::Text& text, uint total_width, uint total_height)
{
  const sf::FloatRect bounds = text.getGlobalBounds();
  text.setPosition( (total_width - bounds.width) / 2, (total_height - bounds.height) / 2 );
}

void resize(sf::Sprite& sprite, float x, float y)
{
  sprite.setScale( x / sprite.getGlobalBounds().width,
                   y / sprite.getGlobalBounds().height );
}

}
}
