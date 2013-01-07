#include "StdAfx.h"
#include "Ship.h"
#include "Renderer.h"

Ship::Ship(int width, int height):minCrew_(0),maxCrew_(0),powerRequired_(0),powerProduced_(0),crewCapacity_(0)
{
  for (int i=0; i<3; ++i) {
    Deck* deck = new Deck(this, width, height, i);
    decks_.push_back(deck);
  }
}

Ship::~Ship()
{
  for (auto itr = decks_.begin(); itr != decks_.end(); ++itr) {
    Deck* deck = *itr;
    delete deck;
  }
}

Deck* Ship::getDeck( int i )
{
  if (i < 0 || i >= (int)decks_.size()) {
    return NULL;
  }
  return decks_[i];
}

void Ship::updateParameters( int dMinCrew, int dMaxCrew, int dPowerProduced, int dPowerRequired, int dCrewCapacity )
{
  minCrew_ += dMinCrew;
  maxCrew_ += dMaxCrew;
  powerRequired_ += dPowerProduced;
  powerProduced_ += dPowerRequired;
  crewCapacity_ += dCrewCapacity;
}

Deck::Deck(Ship* ship, int width, int height,int idx):
  ship_(ship), width_(width), height_(height), idx_(idx)
{
  tileLayout_ = new Tile*[width_ * height_];
  for (int i=0; i<width_*height_; ++i) {
    tileLayout_[i] = new Tile(i%width_, i/height_, idx_);
  }
}

Deck::~Deck()
{
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    delete comp;
  }
  for (int i=0; i<width_*height_; ++i) {
    delete tileLayout_[i];
  }
  delete[] tileLayout_;
}

void Deck::addCompartment(Compartment* comp)
{
  compartments_.push_back(comp);
  ship_->updateParameters(comp->getMinCrew(), comp->getMaxCrew(), comp->getPowerRequired(), comp->getPowerProduced(), comp->getCrewCapacity());
}

void Deck::removeCompartment( Compartment* comp )
{
  compartments_.remove(comp);
  ship_->updateParameters(-comp->getMinCrew(), -comp->getMaxCrew(), -comp->getPowerRequired(), -comp->getPowerProduced(), -comp->getCrewCapacity());
}

Tile* Deck::getTile(int x, int y)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return NULL;
  }
  return tileLayout_[y*width_ + x];
}

void Deck::setTileType(int x, int y, Tile::TileType value)
{
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    assert(0);
  }
  if (value == Tile::Wall || value == Tile::Floor || value == Tile::Door) {
    if (hasDoorsAround(x, y) && value != Tile::Floor) {
      //Can't create doors or walls around a door
      return;
    }
    if (getTileType(x, y) == Tile::Door) {
      //Can only erase door, not replace
      return;
    }
  }
  if ((value == Tile::Wall || value == Tile::Door) && getCompartment(x,y)) {
    //Can't build over compartment
    return;
  }
  Tile* tile = tileLayout_[y*width_ + x];
  if (value != Tile::Door) {
    tile->setEntrance(false);
  }
  tile->setType(value);
}

Tile::TileType Deck::getTileType(int x, int y)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return Tile::Empty;
  }
  Tile* tile = tileLayout_[y*width_ + x];
  if (!tile) {
    return Tile::Empty;
  }
  return tile->getType();
}

int Deck::getWallCode( int i, int j )
{
  int wallCode = 0;
  wallCode |= hasWall(i, j+1);
  wallCode |= (hasWall(i+1, j) << 1);
  wallCode |= (hasWall(i, j-1) << 2);
  wallCode |= (hasWall(i-1, j) << 3);
  assert (wallCode >= 0 && wallCode < 16);
  return wallCode;
}

void Deck::setDoor( int x, int y )
{
  Tile::TileType type = getTileType(x, y);
  if (type != Tile::Wall) {
    return;
  }
  int wallCode = getWallCode(x, y);
  if (wallCode != 10 && wallCode != 5) {
    //Not a horizontal or vertical straight wall
    return;
  }
  setTileType(x, y, Tile::Door);
}

bool Deck::hasDoorsAround( int x, int y )
{
  return getTileType(x-1,y) == Tile::Door || getTileType(x+1, y) == Tile::Door || getTileType(x, y-1) == Tile::Door || getTileType(x, y+1) == Tile::Door;
}

void Deck::eraseDoorsAround( int x, int y )
{
  if (getTileType(x-1,y) == Tile::Door) {
    setTileType(x-1, y, Tile::Empty);
  }
  if (getTileType(x+1,y) == Tile::Door) {
    setTileType(x+1, y, Tile::Empty);
  }
  if (getTileType(x,y-1) == Tile::Door) {
    setTileType(x, y-1, Tile::Empty);
  }
  if (getTileType(x,y+1) == Tile::Door) {
    setTileType(x, y+1, Tile::Empty);
  }
}

Compartment* Deck::getCompartment( int x, int y )
{
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    if (x >= comp->getX() && x < comp->getX() + comp->getWidth() && y >= comp->getY() && y < comp->getY() + comp->getHeight()) {
      return comp;
    }
  }
  return NULL;
}

int Deck::hasWall( int x, int y )
{
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    return 0;
  }
  return (getTileType(x, y)==Tile::Wall || getTileType(x, y)==Tile::Door)?1:0;
}

Compartment::Compartment():rotation_(0)
{
}

Compartment::Compartment(const Compartment& other):left_(other.left_), top_(other.top_), width_(other.width_), height_(other.height_), name_(other.name_),
  category_(other.category_),minCrew_(other.minCrew_),maxCrew_(other.maxCrew_),powerRequired_(other.powerRequired_),powerProduced_(other.powerProduced_),
  crewCapacity_(other.crewCapacity_),/*Maybe this one is not needed -->*/rotation_(other.rotation_)
{
  for (auto itr = other.items_.begin(); itr != other.items_.end(); ++itr) {
    items_.push_back(new Item(**itr));
  }
}

Compartment::~Compartment()
{
  for (auto itr = items_.begin(); itr != items_.end(); ++itr) {
    delete *itr;
  }
}

CString Compartment::categoryName(Category cat)
{
  switch (cat) {
  case Common:
    return "Common";
  case Navigation:
    return "Navigation";
  case Power:
    return "Power";
  case LifeSupport:
    return "Life support";
  case Living:
    return "Living";
  case LastCategory:
    assert(0);
    Logger::getInstance().log(ERROR_LOG_NAME, "Attempted to get last category name");
    break;
  default:
    assert(0);
    Logger::getInstance().log(ERROR_LOG_NAME, "Unknown category");
    break;
  }
  return "";
}

Item::Item()
{
}

Item::Item(const Item& other):x_(other.x_), y_(other.y_),name_(other.name_),texX_(other.texX_),texY_(other.texY_), texWidth_(other.texWidth_),texHeight_(other.texHeight_),
  rotation_(other.rotation_),id_(other.id_)
{
}

Item::~Item()
{
}

void Item::setTexX(int x)
{
  texX_ = x / (float)Renderer::getInstance().getTilesTexWidth();
}

void Item::setTexY(int y)
{
  texY_ = y / (float)Renderer::getInstance().getTilesTexHeight();
}

void Item::setTexWidth(int width)
{
  texWidth_ = width / (float)Renderer::getInstance().getTilesTexWidth();
}

void Item::setTexHeight(int height)
{
  texHeight_ = height / (float)Renderer::getInstance().getTilesTexHeight();
}

Tile::Tile(int x, int y, int deck):type_(Empty), x_(x), y_(y), accessible_(false), connected_(false), deckIdx_(deck), entrance_(false)
{
}

Tile::~Tile()
{
}
