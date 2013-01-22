#pragma once
#include "Ship.h"

class GameWorld
{
public:
  GameWorld();
  ~GameWorld();
  bool init();
  void step();
  Ship* getPlayerShip() {return playerShip_;}
private:
  Ship* playerShip_;
};
