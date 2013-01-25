#pragma once
#include "Window.h"
#include "Person.h"
#include "Text.h"

class PersonHoverInfo: public Window
{
public:
  PersonHoverInfo(Rect size);
  ~PersonHoverInfo();
  void setPerson(Person* pers);
private:
  Text* nameText_;
};
