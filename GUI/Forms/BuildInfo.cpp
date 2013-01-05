#include "StdAfx.h"
#include "BuildInfo.h"
#include "Button.h"

BuildInfo::BuildInfo( Rect size ):Window(size)
{
  double top = 0.01;
  double height = 0.03;
  double left = 0.02;
  double width = 1.0 - 2.0*left;
  sizeText_ = new FramedText(Rect(left, top, width, height));
  sizeText_->setCaption("Size");
  addWidget(sizeText_);
  sizeText_->setAlign(Widget::LeftAlign);
  sizeText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  minCrewRequiredText_ = new FramedText(Rect(left, top, width, height));
  minCrewRequiredText_->setCaption("Minimum crew to operate");
  addWidget(minCrewRequiredText_);
  minCrewRequiredText_->setAlign(Widget::LeftAlign);
  minCrewRequiredText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  maxCrewRequiredText_ = new FramedText(Rect(left, top, width, height));
  maxCrewRequiredText_->setCaption("Maximum crew required");
  addWidget(maxCrewRequiredText_);
  maxCrewRequiredText_->setAlign(Widget::LeftAlign);
  maxCrewRequiredText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  minPowerRequiredText_ = new FramedText(Rect(left, top, width, height));
  minPowerRequiredText_->setCaption("Minimum power to operate");
  addWidget(minPowerRequiredText_);
  minPowerRequiredText_->setAlign(Widget::LeftAlign);
  minPowerRequiredText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  maxPowerRequiredText_ = new FramedText(Rect(left, top, width, height));
  maxPowerRequiredText_->setCaption("Maximum power required");
  addWidget(maxPowerRequiredText_);
  maxPowerRequiredText_->setAlign(Widget::LeftAlign);
  maxPowerRequiredText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  powerProducedText_ = new FramedText(Rect(left, top, width, height));
  powerProducedText_->setCaption("Power produced");
  addWidget(powerProducedText_);
  powerProducedText_->setAlign(Widget::LeftAlign);
  powerProducedText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  crewCapacityText_ = new FramedText(Rect(left, top, width, height));
  crewCapacityText_->setCaption("Crew capacity");
  addWidget(crewCapacityText_);
  crewCapacityText_->setAlign(Widget::LeftAlign);
  crewCapacityText_->setColor(Vector4(50, 255, 10, 255));
  top += height;

  ta_ = new TextArea(Rect(left, top, width, 1.0f - top - 0.01f));
  addWidget(ta_);
}

BuildInfo::~BuildInfo()
{

}

