#include <iostream>

#include <SFML/Graphics.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/game.hpp"

/*! Parse command line arguments
 * \returns first is use AI or not
 *          second is game speed modifier factor
 */
std::tuple<bool, float> parseArgs(int argc, char** argv)
{
  const bool use_ai = ( argc > 1 && std::string(argv[2]) == "1" );

  float game_speed = 1.0;
  if( argc > 2 )
  {
    try{ game_speed = std::stof(argv[3]); }
    catch(...) { /* Nothing to do except ignore */ }
  }

  return std::make_tuple(use_ai, game_speed);
}

int main(int argc, char** argv)
{
  const std::tuple<bool, float> args = parseArgs(argc, argv);

  // Create window
  sf::RenderWindow window(sf::VideoMode(1024, 512), "AI Chrome Easter Game");
  window.setFramerateLimit(30);

  // Init random
  srand(time(NULL));

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Game loop
  models::Game game {window, window.getSize(), std::get<0>(args), std::get<1>(args)};
  game.start();

  return EXIT_SUCCESS;
}
