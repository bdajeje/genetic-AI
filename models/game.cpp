#include "game.hpp"

namespace models {

const std::string Game::_player_filepath {"player/highest"};

Game::Game(sf::RenderWindow* window, sf::Vector2u game_size, bool use_ai, bool allow_draw)
  : _window {window}
  , _map {game_size.x, game_size.y, _player.getPosition().x + _player.getWidth()}
  , _hud {game_size.x, game_size.y}
  , _allow_draw {allow_draw}
{
  // Read highest score
  std::string highest_score_str;
  if( utils::files::read(_player_filepath, highest_score_str) )
  {
    try {
      _highest_score = std::stoul(highest_score_str);
    }
    catch(...)
    {
      std::cerr << "Wrong player file" << std::endl;
    }
  }

  // Set highest score
  _hud.setHighestScore(_highest_score);

  // Create AI
  if(use_ai)
    _ai = new AI::AI;
}

void Game::restart()
{
  saveScore();

  _map.reset();
  _player.reset();
  _hud.reset();

  _game_started = true;
  _hud.setDrawStarting(false);
  _timer.restart();
}

void Game::saveScore()
{
  const unsigned int score = _hud.getScore();
  if( score > _highest_score )
  {
    _highest_score = score;
    utils::files::create(_player_filepath, std::to_string(score), true);
  }
}

void Game::updatePlayerState()
{
  if(!_game_started)
    return;

  if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
    _player.standDown();
  else
    _player.standUp();
}

bool Game::handleEvents()
{
  sf::Event event;
  while(_window->pollEvent(event))
  {
    // Close program
    if( event.type == sf::Event::Closed )
    {
      saveScore();
      return false;
    }
    // If AI, don't handle other events
    else if(isAI())
      break;

    // Remove compiler warnings about some enumeration values not handled in switch
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch"
    switch(event.type)
    {
      case sf::Event::KeyReleased:
      {
        updatePlayerState();
        break;
      }
      case sf::Event::KeyPressed:
      {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
          // Game already started, player jump
          if(_game_started)
            _player.jump();
          // Start the game
          else
            restart();
        }
        else updatePlayerState();

        break;
      }
    }
  }

  return true;
}

int Game::updateModels()
{
  // Get elapsed time since last update
  const sf::Time elapsed_time = _timer.restart();

  // Update models
  _map.update(elapsed_time);
  _player.update(elapsed_time);
  _hud.update(elapsed_time);

  // Is there collision
  if(_map.isCollision(_player))
  {
    _game_started = false;
    _hud.setDrawGameOver(true);
    _hud.setDrawStarting(true);
  }
  return _hud.getScore(); 
}

void Game::draw()
{
  static const sf::Color background_color {0, 0, 0};

  _window->clear(background_color);
  _window->draw(_map);
  _window->draw(_player);
  _window->draw(_hud);
  _window->display();
}

void Game::start()
{
  while(_window->isOpen())
  {
    if(!handleEvents())
      return;

    if(_game_started)
      updateModels();

    draw();
  }
}

void Game::startAI()
{
  /*
   * Initialize NEAT environment
   */

  // Variables
  int genome_id{};
  std::string s_tmp{};
  std::string path_genes{ "ai/mario_genes" };
  std::string path_params{ "ai/mario_params.ne" };
  NEAT::Population *pop{};
  NEAT::Genome *start_genome{};

  // Load parameter file 
  if ( NEAT::load_neat_params( path_params.c_str(), true ) == false ) {
    std::cerr << "[- 1] (game) couldn't find file: " << path_params << std::endl; 
    return;
  }

  // Read initial genome
  std::ifstream f_genes( path_genes, std::ios::in );
  if ( f_genes.is_open() == false ) {
    std::cerr << "[- 1] (game) couldn't find file: " << path_genes << std::endl; 
    return;
  }
  f_genes >> s_tmp >> genome_id;
  start_genome = new NEAT::Genome( genome_id, f_genes );
  f_genes.close();

  /*
   * Start learning / playing
   */

  // Restart experiment multiple times
  for ( int i_runs{} ; i_runs < NEAT::num_runs ; ++i_runs ) {
    pop = new NEAT::Population( start_genome, NEAT::pop_size );
    pop->verify();

    // Evolve generations
    for ( int i_gens{ 1 } ; i_gens <= NEAT::num_gens ; ++i_gens ) {
      std::vector<NEAT::Organism*>::iterator it_orgs;
      //std::vector<NEAT::Species*>::iterator it_specs;

      // Try out each organism (neural network) 
      for ( it_orgs = ( pop->organisms ).begin() ; it_orgs != ( pop->organisms ).end() ; ++it_orgs ) {
        
        /*
         * Play game here
         */
        int fitness{};
        restart();

        // Game loop
			  while( _game_started ) {
			    handleAI( *it_orgs );
          fitness = updateModels();
			    draw();
			  }
        
        // Save organism's fitness (game score)
        ( *it_orgs )->fitness = fitness;

        // Keep checking if the user hasn't closed the game
        if ( _window->isOpen() == false ) {
          delete pop;
          return;
        }

      }

      // Output results every couple of generations
      //if ( i_gens % NEAT::print_every == 0 )
        //pop->print_to_file_by_species( "gen_" + std::to_string( i_gens ) );

      // Generate next generation
      pop->epoch( i_gens );
    }

    delete pop;
  }
 

}

void Game::handleAI( NEAT::Organism *org )
{
  const double threshold_jump{ 0.55 }, threshold_crouch{ 0.45 };
  const float limit           = _player.getPosition().x + _player.getWidth();
  const sf::Sprite& next_hole = _map.getNextHole(limit);
  const float bird_x_pos      = _map.getBirdXPos();

  double output;
  double inputs[] = {
    next_hole.getGlobalBounds().width,
    next_hole.getPosition().x - limit,
    bird_x_pos - limit
  };

  /*
   * Use neural network
   */

  // Send inputs to network
  org->net->load_sensors( inputs );

  // Activate network
  org->net->activate();

  // Fetch output
  output = ( *org->net->outputs.begin() )->activation;
  

  /*
   * Play next move
   */
  
  if( output > threshold_jump )
    _player.jump();
  else if( output < threshold_crouch )
    _player.standDown();
  else
    _player.standUp();
}

}
