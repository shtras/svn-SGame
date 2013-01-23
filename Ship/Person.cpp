#include "StdAfx.h"
#include "Person.h"
#include "Ship.h"
#include "Renderer.h"

Person::Person():rank_(Other), type_(Civilian), ship_(NULL), dir_(Left),pathStepProgress_(0.0f),speed_(0.05f)
{
  actions_.push_back(Idle);
}

Person::~Person()
{

}

bool Person::init( Ship* ship, Position pos )
{
  pos_ = pos;
  headTexX_ = (64 + 64*(rand()%6)) / (float)Renderer::getInstance().getHeadsTexSize().x;
  ship_ = ship;
  bool found = false;
  for (Compartment* comp: ship_->getCompartments()) {
    if (comp->getName() != "CrewQuarters") {
      continue;
    }
    for (Item* item: comp->getItems()) {
      if (item->getType() != Item::Sleep) {
        continue;
      }
      if (item->isGeneralOccupied()) {
        continue;
      }
      //Found sleeping place for our guy
      item->occupyGeneral();
      quarter_.coord = DCoord(item->getX() + comp->getX(), item->getY() + comp->getY());
      quarter_.deckIdx = comp->getDeckIdx();
      found = true;
      break;
    }
    if (found) {
      break;
    }
  }
  if (!found) {
    return false;
  }
  path_ = ship_->findPath(pos_, quarter_);
  actions_.push_front(Walking);
  return true;
}

void Person::step()
{
  Action currentAction = actions_.front();
  if (currentAction == Walking) {
    pathStep();
  }
}

void Person::pathStep()
{
  if (path_.size() == 0) {
    return;
  }
  Direction nextDir = path_.front();
  dir_ = nextDir;
  pathStepProgress_ += speed_;
  if (pathStepProgress_ > 1.0f) {
    pathStepProgress_ = 0.0f;
    path_.pop_front();
    switch (nextDir)
    {
    case Left:
      pos_.coord.x--;
      break;
    case Up:
      pos_.coord.y--;
      break;
    case Right:
      pos_.coord.x++;
      break;
    case Down:
      pos_.coord.y++;
      break;
    case UpStairs:
      break;
    case DownStairs:
      break;
    default:
      break;
    }
  }
}
