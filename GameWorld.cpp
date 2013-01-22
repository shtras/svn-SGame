#include "StdAfx.h"
#include "GameWorld.h"

GameWorld::GameWorld():playerShip_(NULL)
{

}

GameWorld::~GameWorld()
{

}

bool GameWorld::init()
{
  playerShip_ = new Ship(1, 1);
  bool res = playerShip_->load("saves/test2.sgs", true);
  if (!playerShip_->generalStatusOK()) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Ship design incomplete");
    return false;
  }
  return res;
}

void GameWorld::step()
{

}
