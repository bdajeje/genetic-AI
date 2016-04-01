#include "game.hpp"

#include <iostream>

#include "utils/file.hpp"

namespace models {

const std::string Game::_player_filepath {"player/highest"};

Game::Game(sf::RenderWindow& window, sf::Vector2u game_size, bool use_ai, bool allow_draw)
  : _window {window}
  , _map {game_size.x, game_size.y}
  , _hud {game_size.x, game_size.y}
  , _use_ai {use_ai}
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

  _hud.setHighestScore(_highest_score);
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
  const uint score = _hud.getScore();
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
  while(_window.pollEvent(event))
  {
    // Close program
    if( event.type == sf::Event::Closed )
    {
      saveScore();
      return false;
    }
    // If AI, don't handle other events
    else if(_use_ai)
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

void Game::updateModels()
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
    if(_use_ai)
      restart();
    else
    {
      _game_started = false;
      _hud.setDrawGameOver(true);
      _hud.setDrawStarting(true);
    }
  }
}

void Game::draw()
{
  static const sf::Color background_color {0, 0, 0};

  _window.clear(background_color);
  _window.draw(_map);
  _window.draw(_player);
  _window.draw(_hud);
  _window.display();
}

void Game::start()
{
  // If AI, start game directly
  if(_use_ai)
    restart();

  while(_window.isOpen())
  {
    if(!handleEvents())
      return;

    if(_game_started)
      updateModels();

    draw();
  }
}

}
