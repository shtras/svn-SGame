#pragma once
#include "TexturedWidget.h"
#include "Ship.h"
#include "PersonHoverInfo.h"

class CompartmentCrewIcons: public Widget
{
public:
  CompartmentCrewIcons(Rect size);
  ~CompartmentCrewIcons();
  void init(Compartment* comp, PersonHoverInfo* hoverInfo);
  void onHoverEnter();
  void onHoverExit();
private:
  void initWorking();
  void initLiving();
  Compartment* comp_;
  PersonHoverInfo* hoverInfo_;
};
