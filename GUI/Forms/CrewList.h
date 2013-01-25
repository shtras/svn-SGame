#pragma once
#include "Window.h"
#include "Ship.h"

class CrewList: public Window
{
public:
  CrewList(Rect size);
  ~CrewList();
  void init(Ship* ship);
private:
  Ship* ship_;
};
