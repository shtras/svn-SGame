#pragma once
#include "TexturedWidget.h"
#include "Person.h"
#include "PersonHoverInfo.h"

//Base icon. For all crew list
class CrewMemberIcon: public Widget
{
public:
  CrewMemberIcon(Rect size, Compartment* comp);
  virtual ~CrewMemberIcon();
  void render();
  void setPerson(Person* pers);
  Person* getPerson() {return person_;}
  Compartment* getComp() {return comp_;}
  void onHoverEnter();
  void onHoverExit();
  void setHoverInfo(PersonHoverInfo* hoverInfo);
protected:
  Person* person_;
  Compartment* comp_;
  PersonHoverInfo* hoverInfo_;
};

//For living quarters
class CrewMemberIconLiving: public CrewMemberIcon
{
public:
  CrewMemberIconLiving(Rect size, Compartment* comp);
  ~CrewMemberIconLiving();
private:
};

//For work places for those not working in watches (senior officers)
class CrewMemberIconGeneral: public CrewMemberIcon
{
public:
  CrewMemberIconGeneral(Rect size, Compartment* comp);
  ~CrewMemberIconGeneral();
private:
};

//For work places by watches
class CrewMemberIconWatch: public CrewMemberIcon
{
public:
  CrewMemberIconWatch(Rect size, Compartment* comp, int watchNum);
  ~CrewMemberIconWatch();
  void onDrop(Widget* w);
  void onRightClick();
private:
  int watchNum_;
};
