#include "hud.hpp"

#include "managers/fontmanager.hpp"
#include "utils/graphics.hpp"

namespace models {

HUD::HUD( unsigned int window_width, unsigned int window_height )
{
  const sf::Font& font = font::FontManager::get("consolas.ttf");
  const sf::Color color {0, 0, 0};

  // Top string
  _top.setFont(font);
  _top.setCharacterSize(22);
  _top.setColor(color);
  _top.setString( "Top" );
  _top.setPosition( window_width - 130, 15 );

  // Highest score
  _highest_score.setFont(font);
  _highest_score.setCharacterSize(22);
  _highest_score.setColor(color);
  setHighestScore(0);
  _highest_score.setPosition( _top.getPosition().x + _top.getGlobalBounds().width + 15, _top.getPosition().y );

  // Current score
  _score.setFont(font);
  _score.setCharacterSize(22);
  _score.setColor(color);
  _score.setString( scoreString("0") );
  _score.setPosition( _highest_score.getPosition().x, _highest_score.getPosition().y + 20 );

  // Starting text
  _starting.setFont(font);
  _starting.setCharacterSize(50);
  _starting.setColor(color);
  _starting.setString("Press 'space' to start");
  utils::graphics::centerPosition(_starting, window_width, window_height);

  // Game over text
  _game_over.setFont(font);
  _game_over.setCharacterSize(55);
  _game_over.setColor(color);
  _game_over.setString("Game Over");
  utils::graphics::centerPosition(_game_over, window_width, 320);
}

void HUD::setHighestScore(uint value)
{
  _highest_score.setString( scoreString(std::to_string(value)) );
}

void HUD::update(const sf::Time& elapsed_time)
{
  _elapsed_time += elapsed_time.asMilliseconds();

  // Update score text if needed
  const uint new_score = getScore();
  if( isSuperior(new_score, _score) )
  {
    _score.setString( scoreString(std::to_string(new_score)) );

    if( isSuperior(new_score, _highest_score) )
      _highest_score.setString( scoreString(std::to_string(new_score)) );
  }
}

uint HUD::getScore() const
{
  return _elapsed_time / 350;
}

bool HUD::isSuperior(uint value, const sf::Text& text)
{
  return value > std::stoul(text.getString().toAnsiString());
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_top, states);
  target.draw(_highest_score, states);
  target.draw(_score, states);

  if(_draw_game_over)
    target.draw(_game_over, states);

  if(_draw_starting)
    target.draw(_starting, states);
}

std::string HUD::scoreString(const std::string& input)
{
  static const std::string prepend {"0000"};
  if(input.length() > prepend.length())
    return input;

  return prepend.substr(0, 1 + prepend.length() - input.length()) + input;
}

void HUD::reset()
{
  setDrawGameOver(false);
  setDrawStarting(true);
  _elapsed_time = 0;
  _score.setString( scoreString("0") );
}

}
