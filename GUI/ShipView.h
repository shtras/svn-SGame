#pragma once
#include "TexturedWidget.h"
#include "Ship.h"
#include "BuildInfo.h"

class DeckView;

class ShipView: public Widget, public has_slots<>
{
public:
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
  void buildWalls();
  void buildFloor();
  void buildDoor();
  void erase();
  void setHoveredDimensions(int width, int height);
  void deckUp();
  void deckDown();
  int getActiveDeck() {return activeDeckIdx_;}
  void setBuildInfo(BuildInfo* info);
private:
  enum Action {BuildWalls, BuildFloor, BuildDoor, Erase};
  void drawCompartments();
  void plantWalls();
  void eraseArea();
  void renderFloorSection(Deck* deck, int i, int j, Rect& tilePos, Rect& texPos);
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
