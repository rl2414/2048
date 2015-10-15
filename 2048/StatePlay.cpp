#include "StatePlay.hpp"
#include "View.hpp"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

StatePlay::StatePlay(Game* game) : isSpawning(false), isMoving(false) {
  this->game = game;
  srand(time(nullptr));
  memset(state, 0, sizeof(state));

  spawn(2);
}

void StatePlay::handleInput() {
  if (!isMoving && !isSpawning) {
    if (!check()) game->window.close();

    sf::Event event;
    if (game->window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Up:
            dir = UP;
            move();
            break;
          case sf::Keyboard::Down:
            dir = DOWN;
            move();
            break;
          case sf::Keyboard::Left:
            dir = LEFT;
            move();
            break;
          case sf::Keyboard::Right:
            dir = RIGHT;
            move();
            break;
          case sf::Keyboard::Escape:
            game->window.close();
            break;
          default:
            break;
        }
      } else if (event.type == sf::Event::Closed) {
          game->window.close();
      }
    }
  }
}

void StatePlay::update(float dt) {
  if (isSpawning) {
    updateSpawn(dt);
  } else if (isMoving) {
    updateMove(dt);
  }
}

void StatePlay::updateSpawn(float dt) {
  spawningTime += dt;

  if (spawningTime >= animTime) {
    for (SpawnMsg msg : spawnMsg) {
      sf::RectangleShape* block = game->view.blocks[msg.pos];
      sf::Color color = game->view.getColor(msg.num);
      color.a = 255;
      block->setFillColor(color);
      block->setScale(1.0f, 1.0f);

      sf::Text* text = game->view.texts[msg.pos];
      text->setColor(sf::Color(0, 0, 0, 255));
      text->setScale(1.0f, 1.0f);
    }

    for (CombMsg msg : combMsg) {
      game->view.newBlocks[msg.pos]->setScale(1.0f, 1.0f);
      game->view.newTexts[msg.pos]->setScale(1.0f, 1.0f);
      delete game->view.blocks[msg.pos];
      game->view.blocks[msg.pos] = game->view.newBlocks[msg.pos];
      game->view.newBlocks[msg.pos] = nullptr;
      delete game->view.texts[msg.pos];
      game->view.texts[msg.pos] = game->view.newTexts[msg.pos];
      game->view.newTexts[msg.pos] = nullptr;
    }

    isSpawning = false;
    spawnMsg.clear();
    combMsg.clear();
    sf::Event event;
    
    //clear events
    while (game->window.pollEvent(event)) ;
  } else {
    for (SpawnMsg msg : spawnMsg) {
      sf::RectangleShape* block = game->view.blocks[msg.pos];
      sf::Color color = game->view.getColor(msg.num);
      color.a = spawningTime / animTime * 255;
      block->setFillColor(color);
      float scale = spawningTime / animTime;
      block->setScale(scale, scale);

      sf::Text* text = game->view.texts[msg.pos];
      text->setColor(sf::Color(0, 0, 0, spawningTime / animTime * 255));
      text->setScale(scale, scale);
    }

    for (CombMsg msg : combMsg) {
      float scale = spawningTime / animTime;
      game->view.newBlocks[msg.pos]->setScale(scale, scale);
      game->view.newTexts[msg.pos]->setScale(scale, scale);
    }
  }
}

void StatePlay::updateMove(float dt) {
  movingTime += dt;

  if (movingTime >= animTime) {
    for (MoveMsg msg : moveMsg) {
      game->view.blocks[msg.pos]->setPosition(game->view.getPos(msg.newPos));
      if (game->view.blocks[msg.newPos])
        delete game->view.blocks[msg.newPos];
      game->view.blocks[msg.newPos] = game->view.blocks[msg.pos];
      game->view.blocks[msg.pos] = nullptr;

      game->view.texts[msg.pos]->setPosition(game->view.getPos(msg.newPos));
      if (game->view.texts[msg.newPos])
        delete game->view.texts[msg.newPos];
      game->view.texts[msg.newPos] = game->view.texts[msg.pos];
      game->view.texts[msg.pos] = nullptr;
    }
    isMoving = false;
    moveMsg.clear();
    spawn(1);
  } else
    for (MoveMsg msg: moveMsg) {
      sf::Vector2f step;
      step.x = (msg.newPos % 4 - msg.pos % 4) * (GRID_SIZE + SPACE_SIZE);
      step.y = (msg.newPos / 4 - msg.pos / 4) * (GRID_SIZE + SPACE_SIZE);
      sf::RectangleShape* block = game->view.blocks[msg.pos];
      block->setPosition(game->view.getPos(msg.pos) + step * (movingTime / animTime));
      sf::Text* text = game->view.texts[msg.pos];
      text->setPosition(game->view.getPos(msg.pos) + step * (movingTime / animTime));
    }
}

void StatePlay::render() {
  game->view.render(game->window);
}

void StatePlay::getSpawnMsg(int times) {
  while (times--) {
    int i;
    do {
      i = rand() % GRID_NUMBER;
    } while (state[i / 4][i % 4]);
    int n = rand() % 10 > 0 ? 2 : 4;
    state[i / 4][i % 4] = n;
    spawnMsg.push_back({i, n});
  }
}

void StatePlay::spawn(int times) {
  isSpawning = true;
  spawningTime = 0.0f;
  getSpawnMsg(times);

  for (SpawnMsg msg : spawnMsg) {
    game->view.blocks[msg.pos] = new sf::RectangleShape(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    sf::RectangleShape* block = game->view.blocks[msg.pos];
    sf::Color color = game->view.getColor(msg.num);
    color.a = 0;
    block->setFillColor(color);
    block->setOrigin(GRID_SIZE / 2.0f, GRID_SIZE / 2.0f);
    block->setPosition(game->view.getPos(msg.pos));
    block->setScale(0.0f, 0.0f);

    game->view.texts[msg.pos] = new sf::Text(std::to_string(msg.num), game->font, FONT_SIZE);
    sf::Text* text = game->view.texts[msg.pos];
    text->setColor(sf::Color(0, 0, 0, 0));
    sf::FloatRect rect = text->getLocalBounds();
    text->setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    text->setPosition(game->view.getPos(msg.pos));
    text->setScale(0.0f, 0.0f);
  }

  for (CombMsg msg : combMsg) {
    game->view.newBlocks[msg.pos] = new sf::RectangleShape(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    sf::RectangleShape* block = game->view.newBlocks[msg.pos];
    sf::Color color = game->view.getColor(msg.num);
    block->setFillColor(color);
    block->setOrigin(GRID_SIZE / 2.0f, GRID_SIZE / 2.0f);
    block->setPosition(game->view.getPos(msg.pos));
    block->setScale(0.0f, 0.0f);

    game->view.newTexts[msg.pos] = new sf::Text(std::to_string(msg.num), game->font, FONT_SIZE);
    sf::Text* text = game->view.newTexts[msg.pos];
    text->setColor(sf::Color(0, 0, 0, 255));
    sf::FloatRect rect = text->getLocalBounds();
    text->setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    text->setPosition(game->view.getPos(msg.pos));
    text->setScale(0.0f, 0.0f);
  }
}

void StatePlay::getMoveAndCombMsg() {
  memset(flag, false, sizeof(flag));

  switch (dir) {
    case UP:
      for (int j = 0; j < 4; j++)
        for (int i = 0, i2 = i; i < 4; i++, i2 = i)
          if (state[i][j]) {
            while (i2 > 0 && !flag[i2 - 1][j] && (!state[i2 - 1][j] || state[i2 - 1][j] == state[i][j])) i2--;
            if (i == i2) continue;
            moveMsg.push_back({i * 4 + j, i2 * 4 + j});
            if (state[i2][j] == state[i][j]) {
              combMsg.push_back({i2 * 4 + j, state[i2][j] * 2});
              flag[i2][j] = true;
            }
            state[i2][j] = state[i2][j] + state[i][j];
            state[i][j] = 0;
          }
      break;
    case DOWN:
      for (int j = 0; j < 4; j++)
        for (int i = 3, i2 = i; i >= 0; i--, i2 = i)
          if (state[i][j]) {
            while (i2 < 3 && !flag[i2 + 1][j] && (!state[i2 + 1][j] || state[i2 + 1][j] == state[i][j])) i2++;
            if (i == i2) continue;
            moveMsg.push_back({i * 4 + j, i2 * 4 + j});
            if (state[i2][j] == state[i][j]) {
              combMsg.push_back({i2 * 4 + j, state[i2][j] * 2});
              flag[i2][j] = true;
            }
            state[i2][j] = state[i2][j] + state[i][j];
            state[i][j] = 0;
          }
      break;
    case LEFT:
      for (int i = 0; i < 4; i++)
        for (int j = 0, j2 = j; j < 4; j++, j2 = j)
          if (state[i][j]) {
            while (j2 > 0 && !flag[i][j2 - 1] && (!state[i][j2 - 1] || state[i][j2 - 1] == state[i][j])) j2--;
            if (j == j2) continue;
            moveMsg.push_back({i * 4 + j, i * 4 + j2});
            if (state[i][j2] == state[i][j]) {
              combMsg.push_back({i * 4 + j2, state[i][j2] * 2});
              flag[i][j2] = true;
            }
            state[i][j2] = state[i][j2] + state[i][j];
            state[i][j] = 0;
          }
      break;
    case RIGHT:
      for (int i = 0; i < 4; i++)
        for (int j = 3, j2 = j; j >= 0; j--, j2 = j)
          if (state[i][j]) {
            while (j2 < 3 && !flag[i][j2 + 1] && (!state[i][j2 + 1] || state[i][j2 + 1] == state[i][j]))
              j2++;
            if (j == j2) continue;
            moveMsg.push_back({i * 4 + j, i * 4 + j2});
            if (state[i][j2] == state[i][j]) {
              combMsg.push_back({i * 4 + j2, state[i][j2] * 2});
              flag[i][j2] = true;
            }
            state[i][j2] = state[i][j2] + state[i][j];
            state[i][j] = 0;
          }
      break;
  }

  if (moveMsg.empty()) isMoving = false;
}

void StatePlay::move() {
  isMoving = true;
  movingTime = 0.0f;
  getMoveAndCombMsg();
}

bool StatePlay::check() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (!state[i][j]) return true;
      if (i > 0 && state[i][j] == state[i - 1][j]) return true;
      if (i < 3 && state[i][j] == state[i + 1][j]) return true;
      if (j > 0 && state[i][j] == state[i][j - 1]) return true;
      if (j < 3 && state[i][j] == state[i][j + 1]) return true;
    }
  return false;
}















