#ifndef STATE_PLAY_HPP
#define STATE_PLAY_HPP

#include "State.hpp"
#include "Game.hpp"
#include <vector>

struct SpawnMsg {
  int pos, num;
};

enum Move {
  UP, DOWN, LEFT, RIGHT
};

struct MoveMsg {
  int pos, newPos;
};

struct CombMsg {
  int pos, num;
};

class StatePlay: public State {
  public:
    StatePlay(Game*);

    virtual void handleInput();
    virtual void update(float);
    virtual void render();

  private:
    bool isSpawning, isMoving;
    float spawningTime, movingTime;
    Move dir;
    void updateSpawn(float);
    void updateMove(float);

    int state[4][4];
    bool flag[4][4];

    void getSpawnMsg(int);
    void spawn(int);
    std::vector<SpawnMsg> spawnMsg;

    void getMoveAndCombMsg();
    void move();
    std::vector<MoveMsg> moveMsg;
    std::vector<CombMsg> combMsg;

    bool check();
};

const float animTime = 0.15f;

#endif