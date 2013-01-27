#include "StdAfx.h"
#include "GameWorld.h"
#include "Person.h"

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
  if (!res) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Could not find saves/test2.sgs");
    return false;
  }
  if (!playerShip_->generalStatusOK()) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Ship design incomplete");
    return false;
  }
  int crewCount = playerShip_->getCrewCapacity();
  Tile* entrance = playerShip_->getEntrance();
  for (int i=0; i<crewCount; ++i) {
    Person* pers = new Person();
    Position pos = {DCoord(entrance->getX(), entrance->getY()), entrance->getDeckIdx()};
    res &= pers->init(playerShip_, pos);
    if (!res) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Crew member initialization failed");
      delete pers;
      break;
    }
    playerShip_->addCrewMember(pers);
  }
  return res;
}

void GameWorld::step()
{
  playerShip_->step();
}
