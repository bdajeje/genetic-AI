#ifndef UTILS_GRAPHICS_HPP
#define UTILS_GRAPHICS_HPP

#include <SFML/Graphics.hpp>

namespace utils {
namespace graphics{

/*! Horizontally center a text */
void centerPosition(sf::Text& text, unsigned int total_width, unsigned int total_height);

/*! Resize a sprite */
void resize(sf::Sprite& sprite, float x, float y);

}
}

#endif // UTILS_GRAPHICS_HPP

