#pragma once
#include "TexturedWidget.h"
#include "Ship.h"

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
  void buildWalls();
  void buildFloor();
  void buildDoor();
  void erase();
private:
  enum Action {BuildWalls, BuildFloor, BuildDoor, Erase};
  enum TileType {Empty = -1, Floor = 0, Wall = 1, Door = 2};
  ShipView::TileType getWall(int x, int y);
  int hasWall(int x, int y);
  void setWall(int x, int y, TileType value);
  void plantWalls();
  void eraseArea();
  int getWallCode(int i, int j);
  void setDoor(int x, int y);
  bool hasDoorsAround(int x, int y);
  void eraseDoorsAround(int x, int y);
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

  float tileWidth_;
  float tileHeight_;
  bool drawing_;
  int drawingStartX_;
  int drawingStartY_;
  TileType* wallLayout_;
  Action action_;
  int tilesTexWidth_;
  int tilesTexHeight_;
};
