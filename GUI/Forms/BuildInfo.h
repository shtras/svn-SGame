#pragma once
#include "Window.h"
#include "Text.h"
#include "TextArea.h"

class BuildInfo: public Window
{
public:
  BuildInfo(Rect size);
  ~BuildInfo();
private:
  FramedText* sizeText_;
  FramedText* minCrewRequiredText_;
  FramedText* maxCrewRequiredText_;
  FramedText* minPowerRequiredText_;
  FramedText* maxPowerRequiredText_;
  FramedText* powerProducedText_;
  FramedText* crewCapacityText_;
  TextArea* ta_;
};
