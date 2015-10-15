#include "View.hpp"
#include <iostream>

View::View() {
  frame = new sf::RectangleShape(sf::Vector2f(FRAME_SIZE, FRAME_SIZE));
  frame->setOrigin(FRAME_SIZE / 2.0f, FRAME_SIZE / 2.0f);
  frame->setPosition(FRAME_POS + FRAME_SIZE / 2.0f, FRAME_POS + FRAME_SIZE / 2.0f);
  frame->setFillColor(FRAME_COLOR);

  for (int i = 0; i < GRID_NUMBER; i++) {
    grids[i] = new sf::RectangleShape(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    grids[i]->setOrigin(GRID_SIZE / 2.0f, GRID_SIZE / 2.0f);
    grids[i]->setPosition(getPos(i));
    grids[i]->setFillColor(GRID_COLOR);
    
    blocks[i] = newBlocks[i] = nullptr;
    texts[i] = newTexts[i] = nullptr;
  }
}

View::~View() {
  delete frame;
  for (int i = 0; i < GRID_NUMBER; i++) {
    delete grids[i];
    if (blocks[i]) delete blocks[i];
    if (newBlocks[i]) delete newBlocks[i];
    if (texts[i]) delete texts[i];
    if (newTexts[i]) delete newTexts[i];
  }
}

void View::render(sf::RenderWindow& window) {
  window.clear(sf::Color::White);

  window.draw(*frame);
  for (int i = 0; i < GRID_NUMBER; i++)
    window.draw(*grids[i]);

  for (int i = 0; i < GRID_NUMBER; i++) {
    if (blocks[i]) window.draw(*blocks[i]);
    if (texts[i]) window.draw(*texts[i]);
    if (newBlocks[i]) window.draw(*newBlocks[i]);
    if (newTexts[i]) window.draw(*newTexts[i]);
  }

  window.display();
}

sf::Vector2f View::getPos(int i) {
  float x = (i % 4) * GRID_SIZE + (i % 4 + 1) * SPACE_SIZE + FRAME_POS + GRID_SIZE / 2.0f;
  float y = (i / 4) * GRID_SIZE + (i / 4 + 1) * SPACE_SIZE + FRAME_POS + GRID_SIZE / 2.0f;

  return sf::Vector2f(x, y);
}

sf::Color View::getColor(int x) {
  switch (x) {
    case 2:
      return sf::Color(238, 228, 217);
    case 4:
      return sf::Color(237, 224, 197);
    case 8:
      return sf::Color(240, 178, 112);
    case 16:
      return sf::Color(241, 150, 90);
    case 32:
      return sf::Color(242, 125, 89);
    case 64:
      return sf::Color(241, 96, 48);
    default:
      return sf::Color(236, 208, 98);
  }
}



















