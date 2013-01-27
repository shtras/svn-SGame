#pragma once
#define SAVE_VERSION 3
#include "Person.h"

class Deck;
class Compartment;
class Item;
class Tile;

class Ship
{
public:
  friend class Deck;
  Ship(int width, int height);
  ~Ship();
  list<Direction> findPath(Position from, Position to);
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
  bool load(CString fileName, bool adjustSize=false);
  bool connectionsOK() {return connectionStatus_;}
  bool structureOK() {return structureStatus_;}
  bool accessibilityOK() {return accessibleStatus_;}
  bool generalStatusOK();
  void checkConnections();
  void addCrewMember(Person* person);
  const list<Person*>& getCrew() const {return crew_;}
  void step();
  const list<Compartment*>& getCompartments() const {return compartments_;}
  int getSuffixIndex(CString str);
  void openDoor(Tile* door);
  void closeDoor(Tile* door);
private:
  void findPathRec(Tile* tile, int value);
  void fillPath(Tile* from, Tile* to);
  void resetPathFindValues();
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
  list<Person*> crew_;
  list<Compartment*> compartments_;
  list<Direction> path_;
  map<CString, int> suffixIndex_;
  set<Tile*> doorsInProgress_;
};

class Tile
{
public:
  Tile(int x, int y, int deck);
  ~Tile();
  enum TileType {Empty, Wall, Floor, Door, Stair};
  enum DoorState {Closed, Open, Opening, Closing};
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
  void open();
  void close();
  void update();
  int getOpenState();
  DoorState getState() {return doorState_;}
private:
  TileType type_;
  int x_;
  int y_;
  int deckIdx_;
  bool accessible_;
  bool connected_;
  bool entrance_;
  bool checked_; //For entrance validation
  DoorState doorState_;
  int openProgress_;
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
  void resetPathFindValues();
  int getPathFindValue(int x, int y);
  void setPathFindValue(int x, int y, int value);
private:
  Deck();

  int width_;
  int height_;
  Tile** tileLayout_;
  int* pathFindValues_;
  vector<Compartment*> compartments_;
  Ship* ship_;
  int idx_;
};

class Compartment
{
  friend class RoomParser;
  friend class ShipViewEditor;
  friend bool Ship::load(CString fileName, bool adjustSize);
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
  CString getSuffix() {return suffix_;}
  CString getFullName() {return name_+" "+suffix_ + " " + suffixIndex_;}
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
  int getDeckIdx() {return deckIdx_;}
  void setDeckIdx(int deckIdx) {deckIdx_ = deckIdx;}
  void setHighLighted(bool value) {highlighted_ = value;}
  bool isHighLighted() {return highlighted_;}
  void setSuffixIndex(CString str) {suffixIndex_ = str;}
private:
  Category category_;
  int left_;
  int top_;
  int width_;
  int height_;
  int rotation_;
  int deckIdx_;
  CString name_;
  CString suffix_;
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
  bool highlighted_;
  CString suffixIndex_;
};

class Item
{
  friend class RoomParser;
  friend bool Ship::load(CString fileName, bool adjustSize);
  friend void Ship::save(CString fileName);
public:
  enum Type {General, Sleep, Work, Relax, Eat};
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
  bool isGeneralOccupied();
  bool isWatchOccupied(int watch);
  void occupyGeneral();
  void occupyWatch(int watch);
  void vacateGeneral();
  void vacateWatch(int watch);
  Type getType() {return type_;}
  Direction getDirection();
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
  int occupied_; //4 bits (general, 3rd watch, 2nd watch, 1st watch)
  Type type_;
};
