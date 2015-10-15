#include "Game.hpp"
#include "StatePlay.hpp"
#include <SFML/System.hpp>

int main() {
  Game* game = new Game();
  game->pushState(new StatePlay(game));
  game->gameLoop();
  delete game;
  return 0;
}