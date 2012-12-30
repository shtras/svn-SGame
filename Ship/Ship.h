#pragma once

class Deck;
class Compartment;
class Item;
class Tile;

class Ship
{
public:
  Ship();
  ~Ship();
private:
  vector<Deck*> decks_;
};

class Tile
{
public:
  Tile();
  ~Tile();
  enum TileType {Empty, Wall, Floor, Door};
  TileType getType() {return type_;}
private:
  TileType type_;
  int x_;
  int y_;
};

class Deck
{
public:
  Deck(int height, int width);
  ~Deck();
  Tile* getTile(int x, int y);
  void setTile(int x, int y, Tile* tile);
  Tile::TileType getTileType(int x, int y);
  void addCompartment(Compartment* comp);
private:
  Deck();

  int width_;
  int height_;
  Tile** tileLayout_;
  list<Compartment*> compartments_;
};

class Compartment
{
public:
  Compartment();
  Compartment(const Compartment& other);
  ~Compartment();
private:
  int left_;
  int top_;
  int width_;
  int height_;
  list<Item*> items_;
};

class Item
{
public:
  Item();
  Item(const Item& other);
  ~Item();
private:
  int x_;
  int y_;
  CString name_;
  float texX_;
  float texY_;
  float texWidth_;
  float texHeight_;
};
