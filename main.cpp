#include <iostream>

#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/game.hpp"

int main(int argc, char** argv)
{

  /*
   * Handle command line arguments
   */

  bool use_ai {false}, nop_gene {false}, no_log {false};

  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  po::variables_map args;

  desc.add_options()
    ( "help,h",     "Print help message" )
    ( "ai",         "Let NEAT AI play the game" )
    ( "nop_gene",   "Use ai/mario_genes genome as is (no population creation)" )
    ( "no_log",     "Do not log current session" );

  po::store( po::parse_command_line( argc, argv, desc ), args );

  if ( args.count( "help" ) ) {
    std::cout << desc;
    return EXIT_SUCCESS;
  }
  if ( args.count( "ai" ) )
    use_ai = true;
  if ( args.count( "nop_gene" ) )
    nop_gene = true;
  if ( args.count( "no_log" ) )
    no_log = true;


  // Create window
  sf::RenderWindow window(sf::VideoMode(1024, 512), "NEAT Ninja Mario");
  window.setFramerateLimit(60);

  // Init random
  srand(time(NULL));

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Game loop
  models::Game game { &window, window.getSize(), use_ai, true };

  std::cerr << "[+ 0] (main) Starting game with options ai: " << use_ai << ", nop_gene: " << nop_gene << ", no_log: " << no_log << std::endl;
  if ( use_ai )
    game.startAI( no_log, nop_gene );
  else
    game.start();

  return EXIT_SUCCESS;
}
