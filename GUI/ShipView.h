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
  void erase();
private:
  enum Action {BuildWalls, BuildFloor, Erase};
  int getWall(int x, int y);
  int hasWall(int x, int y);
  void setWall(int x, int y, int value);
  void plantWalls();
  void eraseArea();
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
  int* wallLayout_;
  Action action_;
};
