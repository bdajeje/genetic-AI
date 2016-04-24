#include <iostream>

#include <SFML/Graphics.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/game.hpp"

/*! Parse command line arguments
 * \returns first arg  - use AI or not
 *          second arg - use graphic mode or not
 */
std::tuple<bool, bool> parseArgs(int argc, char** argv)
{
  const bool use_ai       = ( argc > 1 && std::string(argv[1]) == "1" );
  const bool use_graphics = ( argc > 2 && std::string(argv[2]) == "1" );

  return std::make_tuple(use_ai, use_graphics);
}

int main(int argc, char** argv)
{
  const auto args = parseArgs(argc, argv);

  // Create window
  sf::RenderWindow window(sf::VideoMode(1024, 512), "AI Chrome Easter Game");
  window.setFramerateLimit(60);

  // Init random
  srand(time(NULL));

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Game loop
  models::Game game {&window, window.getSize(), std::get<0>(args), std::get<1>(args)};

  if ( std::get<0>( args ) )
    game.startAI();
  else
    game.start();

  return EXIT_SUCCESS;
}
