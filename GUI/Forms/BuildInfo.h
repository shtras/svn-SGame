#pragma once
#include "Window.h"
#include "Text.h"
#include "TextArea.h"
#include "Ship.h"

class BuildInfo: public Window
{
public:
  BuildInfo(Rect size);
  ~BuildInfo();
  void updateValues(Ship* ship);
private:
  FramedText* crewText_;
  FramedText* powerText_;
  TextArea* ta_;
};
