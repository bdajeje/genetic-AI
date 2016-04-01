#include "game.hpp"

#include <iostream>

#include "utils/file.hpp"

namespace models {

const std::string Game::_player_filepath {"player/highest"};

Game::Game(sf::RenderWindow& window, sf::Vector2u game_size, bool use_ai, float game_speed)
  : _window {window}
  , _map {game_size.x, game_size.y}
  , _hud {game_size.x, game_size.y}
  , _use_ai {use_ai}
  , _game_speed {game_speed}
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

void Game::start()
{
  const sf::Color background_color {247, 247, 247};

  while(_window.isOpen())
  {
    sf::Event event;
    while(_window.pollEvent(event))
    {
      // Remove compiler warnings about some enumeration values not handled in switch
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wswitch"
      switch(event.type)
      {
        case sf::Event::Closed:
        {
          saveScore();
          return;
        }
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
            {
              restart();
              _hud.setDrawStarting(false);
            }
          }
          else updatePlayerState();

          break;
        }
      }
    }

    if(_game_started)
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
    }

    // Draw
    _window.clear(background_color);
    _window.draw(_map);
    _window.draw(_player);
    _window.draw(_hud);
    _window.display();
  }
}

}
