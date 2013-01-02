#pragma once
#include "Button.h"
#include "Ship.h"
#include "ShipView.h"

class CompartmentButton: public Button
{
public:
  CompartmentButton(Rect size, Compartment* comp, ShipView* shipView);
  ~CompartmentButton();
  void render();
  void onDrag();
  void onStopDrag();
  Compartment* getCompartment() {return comp_;}
private:
  Compartment* comp_;
  ShipView* shipView_;
};
