#pragma once
#include "Window.h"
#include "Ship.h"
#include "PersonHoverInfo.h"

class CrewList: public Window
{
public:
  CrewList(Rect size);
  ~CrewList();
  void init(Ship* ship);
private:
  Ship* ship_;
  PersonHoverInfo* hoverInfo_;
};
