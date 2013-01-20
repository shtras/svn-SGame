#pragma once
#include "ShipView.h"
#include "BuildInfo.h"

class ShipViewEditor: public ShipView
{
public:
  enum Action {BuildWalls, BuildFloor, BuildDoor, BuildStairs, Erase, Select, ChooseEntrance, CreateConnection, RemoveConnection};

  ShipViewEditor(Rect size);
  ~ShipViewEditor();
  void onLMDown();
  void onLMUp();
  void onDrop(Widget* w);
  void setAction(Action action);
  void setDraggedComp(Compartment* comp) {draggedComp_ = comp;}
  void setHoveredDimensions(int width, int height);
  void onMouseWheelScroll(int direction);
  void onHoverExit();
  void onMouseMove();
  void setBuildInfo(BuildInfo* info);
  void saveShip(CString fileName);
  void loadShip(CString fileName);
private:
  void createConnection();
  void removeConnection();
  void addLogMessage(CString message);
  void plantWalls();
  void eraseArea();
  void setEntrance();
  int draggedCompartmentRoataion_;
  Compartment* draggedComp_;
  BuildInfo* buildInfo_;
  Action action_;
};
