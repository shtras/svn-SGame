#pragma once
#include "TexturedWidget.h"
#include "Person.h"

class CrewMemberIcon: public Widget
{
public:
  CrewMemberIcon(Rect size);
  ~CrewMemberIcon();
  void render();
  void setPerson(Person* pers);
private:
  Person* person_;
};
