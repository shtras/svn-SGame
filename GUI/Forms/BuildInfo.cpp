#include "StdAfx.h"
#include "BuildInfo.h"
#include "Ship.h"

BuildInfo::BuildInfo( Rect size ):Window(size),shipView_(NULL)
{
  double top = 0.01;
  double height = 0.03;
  double left = 0.02;
  double width = (1.0 - 2.0*left) * 0.75;
  double width1 = (1.0 - 2.0*left) * 0.25;

  FramedText* labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Crew min(max)/capacity");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  crewText_ = new FramedText(Rect(left+width, top, width1, height));
  crewText_->setCaption("0");
  addWidget(crewText_);
  crewText_->setAlign(Widget::LeftAlign);
  top += height;

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Power required/produced");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  powerText_ = new FramedText(Rect(left+width, top, width1, height));
  powerText_->setCaption("0/0");
  addWidget(powerText_);
  powerText_->setAlign(Widget::LeftAlign);
  top += height;

  ta_ = new TextArea(Rect(left, top, width + width1, 1.0f - top - 0.01f));
  addWidget(ta_);
}

BuildInfo::~BuildInfo()
{

}

void BuildInfo::updateValues()
{
  Ship* ship = shipView_->getShip();
  crewText_->setCaption(CString(ship->getMinCrew()) + "(" + CString(ship->getMaxCrew()) + ")/" + CString(ship->getCrewCapacity()));
  if (ship->getMinCrew() > ship->getCrewCapacity()) {
    crewText_->setColor(Vector4(255,100,100,255));
  } else if (ship->getMaxCrew() > ship->getCrewCapacity()) {
    crewText_->setColor(Vector4(172,204,34,255));
  } else {
    crewText_->setColor(Vector4(255,255,255,255));
  }
  powerText_->setCaption(CString(ship->getPowerRequired()) + "/" + CString(ship->getPowerProduced()));
  if (ship->getPowerRequired() > ship->getPowerProduced()) {
    powerText_->setColor(Vector4(255,100,100,255));
  } else if (ship->getPowerRequired() == ship->getPowerProduced()) {
    powerText_->setColor(Vector4(216,255,43,255));
  } else {
    powerText_->setColor(Vector4(255,255,255,255));
  }
}

void BuildInfo::init( ShipView* shipView )
{
  shipView_ = shipView;
}

