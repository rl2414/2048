#include "Game.hpp"
#include "StatePlay.hpp"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "ResourcePath.hpp"

Game::Game() {
  window.create(sf::VideoMode(330, 330), "2048");
  window.setFramerateLimit(60);

  font.loadFromFile(resPath() + "Arial.ttf");
}

Game::~Game() {
  while (!states.empty()) popState();
}

void Game::pushState(State* state) {
  states.push(state);
}

void Game::popState() {
  delete topState();
  states.pop();
}

State* Game::topState() {
  return states.top();
}

void Game::gameLoop() {
  sf::Clock clock;

  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    float dt = elapsed.asSeconds();

    State* curState = topState();
    curState->handleInput();
    curState->update(dt);
    curState->render();
  }
}