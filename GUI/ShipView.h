#pragma once
#include "TexturedWidget.h"
#include "Ship.h"
#include "CompHoverInfo.h"

class BuildInfo;

class ShipView: public Widget, public has_slots<>
{
public:
  enum Action {BuildWalls, BuildFloor, BuildDoor, BuildStairs, Erase, Select, ChooseEntrance, CreateConnection, RemoveConnection};
  enum OverviewType {Construction, Accessibility};
  ShipView(Rect size);
  ~ShipView();
  void render();
  void onRMDown();
  void onRMUp();
  void onHoverExit();
  void onMouseMove();
  void onMouseWheelScroll(int direction);
  void onLMDown();
  void onLMUp();
  void onDrop(Widget* w);
  void setAction(Action action);
  void setHoveredDimensions(int width, int height);
  void deckUp();
  void deckDown();
  void ghostDeckUp();
  void ghostDeckDown();
  int getActiveDeck() {return activeDeckIdx_;}
  int getGhostDeck() {return ghostDeckIdx_;}
  void setBuildInfo(BuildInfo* info);
  void setDraggedComp(Compartment* comp) {draggedComp_ = comp;}
  Ship* getShip() {return ship_;}
  void constructionOverview();
  void accessibilityOverview();
  void saveShip();
  void loadShip();
private:
  void drawCompartments();
  void plantWalls();
  void eraseArea();
  void setEntrance();
  void renderFloorSection(Deck* deck, int i, int j, Rect& tilePos, Rect& texPos);
  void structureChanged();
  void createConnection();
  void removeConnection();
  void addLogMessage(CString message);
  bool isFloorOrStair(Deck* deck, int x, int y);
  Ship* ship_;
  int layoutWidth_;
  int layoutHeight_;
  float zoom_;
  float desiredZoom_;
  float zoomStep_;
  float offsetX_;
  float offsetY_;
  bool scrolling_;
  float lastMouseX_;
  float lastMouseY_;
  int hoveredLeft_;
  int hoveredTop_;
  int hoverWidth_;
  int hoverHeight_;
  Deck* activeDeck_;
  //vector<DeckView*> decks_;
  int activeDeckIdx_;
  int ghostDeckIdx_;
  float tileWidth_;
  float tileHeight_;
  bool drawing_;
  int drawingStartX_;
  int drawingStartY_;
  Action action_;
  int tilesTexWidth_;
  int tilesTexHeight_;
  Compartment* hoveredComp_;
  BuildInfo* buildInfo_;
  CompHoverInfo* hoveredCompInfo_;
  int draggedCompartmentRoataion_;
  Compartment* draggedComp_;
  OverviewType overviewType_;
  Compartment* selectedComp_;
};
//
//class DeckView
//{
//public:
//  enum TileType {Empty = -1, Floor = 0, Wall = 1, Door = 2};
//  DeckView(int width, int height);
//  ~DeckView();
//  int getWallCode(int i, int j);
//  void setDoor(int x, int y);
//  bool hasDoorsAround(int x, int y);
//  void eraseDoorsAround(int x, int y);
//  Compartment* getCompartment(int x, int y);
//  TileType getWall(int x, int y);
//  int hasWall(int x, int y);
//  void setWall(int x, int y, TileType value);
//  void addCompartment(Compartment* comp);
//  void removeCompartment(Compartment* comp);
//  list<Compartment*>& getCompartments() {return compartments_;}
//private:
//  TileType* wallLayout_;
//  list<Compartment*> compartments_;
//  int width_;
//  int height_;
//};
