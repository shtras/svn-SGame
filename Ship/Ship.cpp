#include "StdAfx.h"
#include "Ship.h"

Ship::Ship()
{

}

Ship::~Ship()
{
  for (auto itr = decks_.begin(); itr != decks_.end(); ++itr) {
    Deck* deck = *itr;
    delete deck;
  }
}

Deck::Deck(int height, int width):
  width_(width), height_(height)
{
  tileLayout_ = new Tile*[width_ * height_];
  for (int i=0; i<width_*height_; ++i) {
    tileLayout_[i] = new Tile();
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
}

Tile* Deck::getTile(int x, int y)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return NULL;
  }
  return tileLayout_[y*width_ + x];
}

void Deck::setTile(int x, int y, Tile* tile)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return;
  }
  if (tile == tileLayout_[y*width_ + x]) {
    return;
  }
  delete tileLayout_[y*width_ + x];
  tileLayout_[y*width_ + x] = tile;
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

Compartment::Compartment()
{
}

Compartment::Compartment(const Compartment& other):left_(other.left_), top_(other.top_), width_(other.width_), height_(other.height_)
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

Item::Item()
{
}

Item::Item(const Item& other):x_(other.x_), y_(other.y_),name_(other.name_),texX_(other.texX_),texY_(other.texY_), texWidth_(other.texWidth_),texHeight_(other.texHeight_)
{
}

Item::~Item()
{
}

Tile::Tile():type_(Empty)
{
}

Tile::~Tile()
{
}
