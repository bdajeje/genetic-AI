#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

namespace models {

class HUD final : public sf::Drawable
{
  public:

    HUD(unsigned int window_width, unsigned int window_height);

    void update(const sf::Time& elapsed_time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    void setDrawStarting(bool value) { _draw_starting = value; }
    void setDrawGameOver(bool value) { _draw_game_over = value; }
    void setHighestScore(unsigned int value);

    unsigned int getScore() const;

  private:

    static std::string scoreString(const std::string& input);
    static bool isSuperior(unsigned int value, const sf::Text& text);

  private:

    bool _draw_game_over {false};
    bool _draw_starting {true};
    sf::Text _top;
    sf::Text _highest_score;
    sf::Text _score;
    sf::Text _game_over;
    sf::Text _starting;
    unsigned int _elapsed_time {0};
};

}

#endif // HUD_HPP
