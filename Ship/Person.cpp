#include "StdAfx.h"
#include "Person.h"
#include "Ship.h"
#include "Renderer.h"
#include "CrewMemberIcon.h"
#include "Timer.h"

Person::Person():rank_(Other), type_(Civilian), ship_(NULL), dir_(Left),pathStepProgress_(0.0f),speed_(0.01f),highlighted_(false),quarterComp_(NULL),idleCnt_(0)
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
  speed_ += (rand() % 10 - 10) * 0.0005f;
  headTexX_ = (64*(nameInt%8+1)) / (float)Renderer::getInstance().getHeadsTexSize().x;
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
      quarterComp_ = comp;
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
  if (actions_.size() == 0) {
    decideNextAction();
  }
  Action currentAction = actions_.front();
  int currentWatch = Timer::getTime().getWatch();
  if (currentAction == Walking) {
    pathStep();
    if (path_.size() == 0) {
      actions_.pop_front();
    }
  } else if (currentAction == Idle) {
    ++idleCnt_;
    if (idleCnt_ > 100) {
      idleCnt_ = 0;
      actions_.pop_front();
    }
  } else if (currentAction == Working) {
    //Check if not fired from watch or it didn't end
    if (watchIcon_[currentWatch]) {
      if (watchPos_[currentWatch] != pos_) {
        //Has been assigned to another place
        actions_.pop_front(); //Stop working
      }
    } else {
      actions_.pop_front();
      //Watch has been released
    }
  }
}

void Person::pathStep()
{
  if (path_.size() == 0) {
    return;
  }
  Direction dir = path_.front();
  dir_ = dir;
  pathStepProgress_ += speed_;

  Direction nextDir = path_.front();
  Position nextPos = movePosition(pos_, nextDir);
  Tile* nextTile = ship_->getDeck(nextPos.deckIdx)->getTile(nextPos.coord.x, nextPos.coord.y);
  if (nextTile->getType() == Tile::Door) {
    ship_->openDoor(nextTile);
  }
  nextTile = ship_->getDeck(pos_.deckIdx)->getTile(pos_.coord.x, pos_.coord.y);
  if (nextTile->getType() == Tile::Door) {
    ship_->openDoor(nextTile);
  }

  if (pathStepProgress_ > 1.0f) {
    pathStepProgress_ = 0.0f;
    path_.pop_front();
    Tile* prevTile = ship_->getDeck(pos_.deckIdx)->getTile(pos_.coord.x, pos_.coord.y);
    pos_ = movePosition(pos_, dir);
    if (prevTile->getType() == Tile::Door) {
      ship_->closeDoor(prevTile);
    }
    //if (path_.size() != 0) {
    //  Direction nextDir = path_.front();
    //  Position nextPos = movePosition(pos_, nextDir);
    //  Tile* nextTile = ship_->getDeck(nextPos.deckIdx)->getTile(nextPos.coord.x, nextPos.coord.y);
    //  if (nextTile->getType() == Tile::Door) {
    //    ship_->openDoor(nextTile);
    //  }
    //}
  }
  if (path_.size() == 0) {
    Compartment* comp = ship_->getDeck(pos_.deckIdx)->getCompartment(pos_.coord.x, pos_.coord.y);
    if (comp) {
      Item* item = comp->getItem(pos_.coord.x - comp->getX(), pos_.coord.y - comp->getY());
      if (item) {
        dir_ = item->getDirection();
      }
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
  Compartment* comp = NULL;
  if (icon) {
    comp = icon->getComp();
  }
  if (comp) {
    bool found = false;
    for (Item* item: comp->getItems()) {
      if (item->getType() != Item::Work) {
        continue;
      }
      if (!item->isWatchOccupied(watchNum)) {
        item->occupyWatch(watchNum);
        found = true;
        watchPos_[watchNum].coord.x = item->getX() + comp->getX();
        watchPos_[watchNum].coord.y = item->getY() + comp->getY();
        watchPos_[watchNum].deckIdx = comp->getDeckIdx();
        break;
      }
    }
    assert(found);
  } else {
    assert(watchIcon_[watchNum]);
    Compartment* prevComp = watchIcon_[watchNum]->getComp();
    assert(prevComp);
    int itemX = watchPos_[watchNum].coord.x - prevComp->getX();
    int itemY = watchPos_[watchNum].coord.y - prevComp->getY();
    Item* workItem = prevComp->getItem(itemX, itemY);
    assert(workItem);
    assert(workItem->isWatchOccupied(watchNum));
    workItem->vacateWatch(watchNum);
  }
  watchIcon_[watchNum] = icon;
}

void Person::decideNextAction()
{
  assert (actions_.size() == 0);
  int watch = Timer::getTime().getWatch();
  if (watchIcon_[watch]) {
    path_ = ship_->findPath(pos_, watchPos_[watch]);
    actions_.push_back(Walking);
    actions_.push_back(Working);
    return;
  }
  //Doesn't have to work now
  if (pos_ != quarter_) {
    //Return to quarter and rest
    path_ = ship_->findPath(pos_, quarter_);
    actions_.push_back(Walking);
    return;
  }

  actions_.push_back(Idle);
}

Position Person::movePosition( Position pos, Direction dir )
{
  Position res = pos;
  switch (dir)
  {
  case Left:
    res.coord.x--;
    break;
  case Up:
    res.coord.y--;
    break;
  case Right:
    res.coord.x++;
    break;
  case Down:
    res.coord.y++;
    break;
  case UpStairs:
    break;
  case DownStairs:
    break;
  default:
    break;
  }
  return res;
}
