#pragma once

class Deck;
class Compartment;
class Item;
class Tile;

class Ship
{
public:
  Ship(int width, int height);
  ~Ship();
  Deck* getDeck(int i);
  void updateParameters(int dMinCrew, int dMaxCrew, int dPowerProduced, int dPowerRequired, int dCrewCapacity);
  int getMinCrew() {return minCrew_;}
  int getMaxCrew() {return maxCrew_;}
  int getPowerRequired() {return powerRequired_;}
  int getPowerProduced() {return powerProduced_;}
  int getCrewCapacity() {return crewCapacity_;}
  void setEntrance(Tile* tile) {entrance_ = tile;}
  Tile* getEntrance() {return entrance_;}
  void recalculateTiles();
private:
  void recalculateTilesRec(int deckIdx, int x, int y, bool accessible, bool connected);
  bool recalculateTile(Tile* tile, bool& accessible);
  vector<Deck*> decks_;
  int minCrew_;
  int maxCrew_;
  int powerRequired_;
  int powerProduced_;
  int crewCapacity_;
  Tile* entrance_;
  int width_;
  int height_;
};

class Tile
{
public:
  Tile(int x, int y, int deck);
  ~Tile();
  enum TileType {Empty, Wall, Floor, Door};
  TileType getType() {return type_;}
  void setType(TileType type) {type_ = type;}
  void setEntrance(bool value) {entrance_ = value;}
  bool isEntrance() {return entrance_;}
  int getDeckIdx() {return deckIdx_;}
  bool isAccessible() {return accessible_;}
  void setAccessible(bool value) {accessible_ = value;}
  bool isConnected() {return connected_;}
  void setConnected(bool value) {connected_ = value;}
  int getX() {return x_;}
  int getY() {return y_;}
private:
  TileType type_;
  int x_;
  int y_;
  int deckIdx_;
  bool accessible_;
  bool connected_;
  bool entrance_;
};

class Deck
{
public:
  Deck(Ship* ship, int width, int height, int idx);
  ~Deck();
  Tile* getTile(int x, int y);
  void setTileType(int x, int y, Tile::TileType tile);
  Tile::TileType getTileType(int x, int y);
  void addCompartment(Compartment* comp);
  void removeCompartment(Compartment* comp);

  int getWallCode(int i, int j);
  void setDoor(int x, int y);
  bool hasDoorsAround(int x, int y);
  void eraseDoorsAround(int x, int y);
  Compartment* getCompartment(int x, int y);
  int hasWall(int x, int y);
  list<Compartment*>& getCompartments() {return compartments_;}
private:
  Deck();

  int width_;
  int height_;
  Tile** tileLayout_;
  list<Compartment*> compartments_;
  Ship* ship_;
  int idx_;
};

class Compartment
{
  friend class RoomParser;
  friend class ShipView;
public:
  enum Category {Common = 0, Navigation, Power, LifeSupport, Living, LastCategory};
  Compartment();
  Compartment(const Compartment& other);
  ~Compartment();
  void addItem(Item* item) {items_.push_back(item);}
  Category getCategory() {return category_;}
  const list<Item*>& getItems() {return items_;}
  int getX() {return left_;}
  int getY() {return top_;}
  void setX(int x) {left_ = x;}
  void setY(int y) {top_ = y;}
  int getWidth() {return width_;}
  int getHeight() {return height_;}
  int getRotation() {return rotation_;}
  int getMinCrew() {return minCrew_;}
  int getMaxCrew() {return maxCrew_;}
  int getPowerRequired() {return powerRequired_;}
  int getPowerProduced() {return powerProduced_;}
  int getCrewCapacity() {return crewCapacity_;}
  CString getName() {return name_;}
  static CString categoryName(Category cat);
private:
  Category category_;
  int left_;
  int top_;
  int width_;
  int height_;
  int rotation_;
  CString name_;
  list<Item*> items_;
  int minCrew_;
  int maxCrew_;
  int powerRequired_;
  int powerProduced_;
  int crewCapacity_;
};

class Item
{
  friend class RoomParser;
public:
  Item();
  Item(const Item& other);
  ~Item();
  void setTexX(int x);
  void setTexY(int y);
  void setTexWidth(int width);
  void setTexHeight(int height);
  int getID() {return id_;}
  int getX() {return x_;}
  int getY() {return y_;}
  int getRotation() {return rotation_;}
  float getTexX() {return texX_;}
  float getTexY() {return texY_;}
  float getTexWidth() {return texWidth_;}
  float getTexHeight() {return texHeight_;}
  void setRotation(int value) {rotation_ = value;}
  void setX(int value) {x_ = value;}
  void setY(int value) {y_ = value;}
private:
  int id_;
  int x_;
  int y_;
  int rotation_;
  CString name_;
  float texX_;
  float texY_;
  float texWidth_;
  float texHeight_;
};
