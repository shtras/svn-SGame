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
  Person* getPerson();
private:
  Text* nameText_;
  Text* livingText_;
  Text* watchText_[3];
  Person* pers_;
};
