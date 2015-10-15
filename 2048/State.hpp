#ifndef STATE_HPP
#define STATE_HPP

#include "Game.hpp"

class State
{
  public:
    virtual ~State() { }
    virtual void handleInput() = 0;
    virtual void update(float) = 0;
    virtual void render() = 0;

  protected:
    Game* game;
};

#endif