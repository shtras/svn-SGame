#include "StdAfx.h"
#include "CrewMemberIcon.h"
#include "Renderer.h"

CrewMemberIcon::CrewMemberIcon( Rect size, Compartment* comp ):Widget(size),person_(NULL),comp_(comp)
{
  clickable_ = true;
}

CrewMemberIcon::~CrewMemberIcon()
{

}

void CrewMemberIcon::render()
{
  Renderer& renderer = Renderer::getInstance();
  Rect texPos(0.0f, 64/(float)renderer.getHeadsTexSize().y, 64/(float)renderer.getHeadsTexSize().x, 64/(float)renderer.getHeadsTexSize().y);
  if (person_) {
    texPos.left = person_->getHeadTexX();
  }
  renderer.drawTexRect(size_, renderer.getHeadsTex(), texPos);
}

void CrewMemberIcon::setPerson( Person* pers )
{
  person_ = pers;
  if (pers) {
    draggable_ = true;
  } else {
    draggable_ = false;
  }
}

void CrewMemberIcon::onHoverEnter()
{
  if (person_) {
    person_->setHighLighted(true);
  }
  if (hoverInfo_) {
    hoverInfo_->setPerson(person_);
    Renderer::getInstance().setFloatingWidget(hoverInfo_);
  }
}

void CrewMemberIcon::onHoverExit()
{
  if (person_) {
    person_->setHighLighted(false);
  }
  PersonHoverInfo* currHoverInfo = dynamic_cast<PersonHoverInfo*>(Renderer::getInstance().getFloatingWidget());
  if (currHoverInfo && currHoverInfo->getPerson() != person_) {
    return;
  }
  Renderer::getInstance().setFloatingWidget(NULL);
}

void CrewMemberIcon::setHoverInfo( PersonHoverInfo* hoverInfo )
{
  hoverInfo_ = hoverInfo;
}

CrewMemberIconLiving::CrewMemberIconLiving( Rect size, Compartment* comp ):CrewMemberIcon(size, comp)
{

}

CrewMemberIconLiving::~CrewMemberIconLiving()
{

}

CrewMemberIconGeneral::CrewMemberIconGeneral( Rect size, Compartment* comp ):CrewMemberIcon(size, comp)
{

}

CrewMemberIconGeneral::~CrewMemberIconGeneral()
{

}

CrewMemberIconWatch::CrewMemberIconWatch( Rect size, Compartment* comp, int watchNum ):CrewMemberIcon(size, comp),watchNum_(watchNum)
{

}

CrewMemberIconWatch::~CrewMemberIconWatch()
{

}

void CrewMemberIconWatch::onDrop( Widget* w )
{
  CrewMemberIcon* generalIcon = dynamic_cast<CrewMemberIcon*>(w);
  if (!w) {
    return;
  }
  if (person_ == generalIcon->getPerson()) {
    //Trying to assign the same person
    return;
  }
  if (person_) {
    person_->setWatchIcon(watchNum_, NULL);
  }
  Person* newPerson = generalIcon->getPerson();
  CrewMemberIcon* oldIcon = newPerson->getWatchIcon(watchNum_);
  //Compartment* oldComp = newPerson->getWatchCompartment(watchNum_);
  if (oldIcon) {
    oldIcon->rightClick();
  }
  //newPerson->setWatchCompartment(watchNum_, comp_);
  newPerson->setWatchIcon(watchNum_, this);
  setPerson(newPerson);
  hoverInfo_->setPerson(newPerson);
}

void CrewMemberIconWatch::onRightClick()
{
  if (!person_) {
    return;
  }
  //person_->setWatchCompartment(watchNum_, NULL);
  person_->setWatchIcon(watchNum_, NULL);
  setPerson(NULL);
  onHoverEnter();
}
