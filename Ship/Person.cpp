#include "StdAfx.h"
#include "Person.h"
#include "Ship.h"
#include "Renderer.h"

Person::Person():rank_(Other), type_(Civilian), ship_(NULL), dir_(Left),pathStepProgress_(0.0f),speed_(0.05f),highlighted_(false)
{
  actions_.push_back(Idle);
  for (int i=0; i<3; ++i) {
    //watchComp_[i] = NULL;
    watchIcon_[i] = NULL;
  }
}

Person::~Person()
{

}

bool Person::init( Ship* ship, Position pos )
{
  static int nameInt = 0;
  name_ = "Crew Member " + CString(nameInt++);
  pos_ = pos;
  headTexX_ = (64*(nameInt)) / (float)Renderer::getInstance().getHeadsTexSize().x;
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

//void Person::setWatchCompartment( int watchNum, Compartment* comp )
//{
//  assert (watchNum >= 0 && watchNum < 3);
//  watchComp_[watchNum] = comp;
//}
//
//Compartment* Person::getWatchCompartment( int watchNum )
//{
//  return watchComp_[watchNum];
//}

CrewMemberIcon* Person::getWatchIcon( int watchNum )
{
  return watchIcon_[watchNum];
}

void Person::setWatchIcon( int watchNum, CrewMemberIcon* icon )
{
  watchIcon_[watchNum] = icon;
}
