#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "View.hpp"

class State;

class Game {
  public:
    Game();
    ~Game();

    void pushState(State*);
    void popState();
    State* topState();

    void gameLoop();

    sf::RenderWindow window;
    sf::Font font;
    View view;

  private:
    std::stack<State*> states;
};

#endif 