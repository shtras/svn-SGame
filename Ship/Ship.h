#pragma once
#define SAVE_VERSION 2

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
  int getNumDecks() {return numDecks_;}
  void normalize();
  void shiftContents(int dx, int dy);
  void save(CString fileName);
  bool load(CString fileName);
  bool connectionsOK() {return connectionStatus_;}
  bool structureOK() {return structureStatus_;}
  bool accessibilityOK() {return accessibleStatus_;}
  void checkConnections();
private:
  void resetValues();
  void recalculateTilesRec(int deckIdx, int x, int y, bool accessible, bool connected);
  bool isEntranceValid();
  bool outAccessible(Tile* tile);
  vector<Deck*> decks_;
  int minCrew_;
  int maxCrew_;
  int powerRequired_;
  int powerProduced_;
  int crewCapacity_;
  Tile* entrance_;
  int width_;
  int height_;
  int numDecks_;
  int left_;
  int top_;
  int actualWidth_;
  int actualHeight_;
  bool connectionStatus_;
  bool accessibleStatus_;
  bool structureStatus_;
};

class Tile
{
public:
  Tile(int x, int y, int deck);
  ~Tile();
  enum TileType {Empty, Wall, Floor, Door, Stair};
  TileType getType() {return type_;}
  void setType(TileType type) {type_ = type;}
  void setEntrance(bool value) {entrance_ = value;}
  bool isEntrance() {return entrance_;}
  int getDeckIdx() {return deckIdx_;}
  bool isAccessible() {return accessible_;}
  void setAccessible(bool value) {accessible_ = value;}
  bool isConnected() {return connected_;}
  void setConnected(bool value) {connected_ = value;}
  bool isChecked() {return checked_;}
  void setChecked(bool value) {checked_ = value;}
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
  bool checked_; //For entrance validation
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
  void setStair(int x, int y);
  bool hasDoorsAround(int x, int y);
  void eraseDoorsAround(int x, int y);
  Compartment* getCompartment(int x, int y);
  int hasWall(int x, int y);
  vector<Compartment*>& getCompartments() {return compartments_;}
  void shiftContents(int dx, int dy);
  bool checkConnections();
private:
  Deck();

  int width_;
  int height_;
  Tile** tileLayout_;
  vector<Compartment*> compartments_;
  Ship* ship_;
  int idx_;
};

class Compartment
{
  friend class RoomParser;
  friend class ShipViewEditor;
  friend bool Ship::load(CString fileName);
public:
  enum Category {Common = 0, Navigation, Power, LifeSupport, Living, Weapons, LastCategory};
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
  void connectTo(Compartment* comp);
  void disconnectFrom(Compartment* comp);
  set<Compartment*>& getConnections() {return connections_;}
  bool isConnectedTo(Compartment* comp);
  set <CString>& getRequiredConnections() {return requiredConnections_;}
  bool isConnectedTo(CString compName);
  int numConnectionsTo(CString compName);
  bool requiredConnection(CString compName);
  void setName(CString name) {name_ = name;}
  void setRequiredConnections(set<CString> connections) {requiredConnections_ = connections;}
  int getMaxConnections() {return maxConnections_;}
  int getMaxSameConnections() {return maxSameConnections_;}
  Item* getItem(int x, int y);
  bool requiresAccess() {return requiresAccess_;}
  bool isInside(int x, int y);
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
  set<Compartment*> connections_;
  set<CString> requiredConnections_;
  int maxConnections_;
  int maxSameConnections_;
  bool requiresAccess_;
};

class Item
{
  friend class RoomParser;
  friend bool Ship::load(CString fileName);
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
  bool autorotate() {return autorotate_;}
  bool requiresVacuum() {return requiresVacuum_;}
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
  bool autorotate_;
  bool requiresVacuum_;
};
