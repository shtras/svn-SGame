#pragma once
#include "Window.h"
#include "Ship.h"
#include "Text.h"
#include "TextArea.h"

class CompHoverInfo: public Window
{
public:
  CompHoverInfo(Rect size);
  ~CompHoverInfo();
  void setCompartment(Compartment* comp);
private:
  Text* name_;
  TextArea* requiredConnectionsTA_;
};
