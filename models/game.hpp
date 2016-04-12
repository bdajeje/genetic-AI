#ifndef GAME_HPP
#define GAME_HPP

#include "ai/ai.hpp"
#include "models/map.hpp"
#include "models/hud.hpp"
#include "models/player.hpp"
#include "utils/timer.hpp"

namespace models {

class Game final
{
  public:

    Game(sf::RenderWindow& window, sf::Vector2u game_size, bool use_ai, bool allow_draw);

    void start();

  private:

    bool isAI() const { return _ai != nullptr; }
    void restart();
    void saveScore();
    void updatePlayerState();
    bool handleEvents();
    void draw();
    void updateModels();
    void handleAI();

  private:

    sf::RenderWindow& _window;

    Player _player; // Player has to be initialed before map (map needs player x position)
    Map _map;
    HUD _hud;

    unsigned int _highest_score {0};
    bool _game_started {false};
    utils::time::Timer _timer;
    AI::AI* _ai {nullptr};
    bool _allow_draw;

    static const std::string _player_filepath;
};

}

#endif // GAME_HPP
