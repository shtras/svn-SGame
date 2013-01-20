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

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Entrance");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  entranceText_ = new FramedText(Rect(left+width, top, width1, height));
  entranceText_->setCaption("Missing");
  addWidget(entranceText_);
  entranceText_->setAlign(Widget::LeftAlign);
  top += height;

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Structure status");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  structureStatusText_ = new FramedText(Rect(left+width, top, width1, height));
  structureStatusText_->setCaption("OK");
  addWidget(structureStatusText_);
  structureStatusText_->setAlign(Widget::LeftAlign);
  top += height;

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Accessibility status");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  accessibilityStatusText_ = new FramedText(Rect(left+width, top, width1, height));
  accessibilityStatusText_->setCaption("OK");
  addWidget(accessibilityStatusText_);
  accessibilityStatusText_->setAlign(Widget::LeftAlign);
  top += height;

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("Connections status");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  connectionsStatusText_ = new FramedText(Rect(left+width, top, width1, height));
  connectionsStatusText_->setCaption("OK");
  addWidget(connectionsStatusText_);
  connectionsStatusText_->setAlign(Widget::LeftAlign);
  top += height;

  labelText = new FramedText(Rect(left, top, width, height));
  labelText->setCaption("General status");
  labelText->setAlign(Widget::LeftAlign);
  labelText->setColor(Vector4(50, 255, 10, 255));
  addWidget(labelText);
  generalStatusText_ = new FramedText(Rect(left+width, top, width1, height));
  generalStatusText_->setCaption("OK");
  addWidget(generalStatusText_);
  generalStatusText_->setAlign(Widget::LeftAlign);
  top += height;

  ta_ = new TextArea(Rect(left, top, width + width1, 1.0f - top - 0.01f));
  addWidget(ta_);
  ta_->setTextSize(0.9f);
}

BuildInfo::~BuildInfo()
{

}

void BuildInfo::updateValues()
{
  bool status = true;
  Ship* ship = shipView_->getShip();
  crewText_->setCaption(CString(ship->getMinCrew()) + "(" + CString(ship->getMaxCrew()) + ")/" + CString(ship->getCrewCapacity()));
  if (ship->getMinCrew() > ship->getCrewCapacity()) {
    crewText_->setColor(Vector4(255,100,100,255));
    status = false;
  } else if (ship->getMaxCrew() > ship->getCrewCapacity()) {
    crewText_->setColor(Vector4(172,204,34,255));
  } else {
    crewText_->setColor(Vector4(255,255,255,255));
  }
  powerText_->setCaption(CString(ship->getPowerRequired()) + "/" + CString(ship->getPowerProduced()));
  if (ship->getPowerRequired() > ship->getPowerProduced()) {
    powerText_->setColor(Vector4(255,100,100,255));
    status = false;
  } else if (ship->getPowerRequired() == ship->getPowerProduced()) {
    powerText_->setColor(Vector4(216,255,43,255));
  } else {
    powerText_->setColor(Vector4(255,255,255,255));
  }
  if (ship->getEntrance()) {
    entranceText_->setColor(Vector4(255,255,255,255));
    entranceText_->setCaption("Exists");
  } else {
    entranceText_->setColor(Vector4(255,100,100,255));
    entranceText_->setCaption("Missing");
    status = false;
  }
  if (ship->connectionsOK()) {
    connectionsStatusText_->setColor(Vector4(255,255,255,255));
    connectionsStatusText_->setCaption("OK");
  } else {
    connectionsStatusText_->setColor(Vector4(255,100,100,255));
    connectionsStatusText_->setCaption("Not OK");
    status = false;
  }
  if (ship->accessibilityOK()) {
    accessibilityStatusText_->setColor(Vector4(255,255,255,255));
    accessibilityStatusText_->setCaption("OK");
  } else {
    accessibilityStatusText_->setColor(Vector4(255,100,100,255));
    accessibilityStatusText_->setCaption("Not OK");
    status = false;
  }
  if (ship->structureOK()) {
    structureStatusText_->setColor(Vector4(255,255,255,255));
    structureStatusText_->setCaption("OK");
  } else {
    structureStatusText_->setColor(Vector4(255,100,100,255));
    structureStatusText_->setCaption("Not OK");
    status = false;
  }
  if (status) {
    generalStatusText_->setColor(Vector4(255,255,255,255));
    generalStatusText_->setCaption("OK");
  } else {
    generalStatusText_->setColor(Vector4(255,100,100,255));
    generalStatusText_->setCaption("Not OK");
  }
}

void BuildInfo::init( ShipView* shipView )
{
  shipView_ = shipView;
}

