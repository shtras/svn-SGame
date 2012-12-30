#include "StdAfx.h"
#include "BuildTools.h"

BuildTools::BuildTools( Rect size ):Window(size)
{

}

BuildTools::~BuildTools()
{

}

void BuildTools::init(ShipView* shipView)
{
  shipView_ = shipView;
  buildButton_ = new Button(Rect(0.04, 0.05, 0.2, 0.05), false);
  TextureParams regular = {100, 0, 54, 37, 10, 6, 10, 6};
  TextureParams hovered = {155, 0, 54, 37, 10, 6, 10, 6};
  TextureParams pressed = {210, 0, 54, 37, 10, 6, 10, 6};
  buildButton_->init(regular, hovered, pressed);
  buildButton_->sigClick.connect(this, &BuildTools::buildClick);
  buildButton_->setHighlighted(true);
  addWidget(buildButton_);

  floorButton_ = new Button(Rect(0.28, 0.05, 0.2, 0.05), false);
  TextureParams regular2 = {100, 76, 54, 37, 10, 6, 10, 6};
  TextureParams hovered2 = {155, 76, 54, 37, 10, 6, 10, 6};
  TextureParams pressed2 = {210, 76, 54, 37, 10, 6, 10, 6};
  floorButton_->init(regular2, hovered2, pressed2);
  floorButton_->sigClick.connect(this, &BuildTools::floorClick);
  addWidget(floorButton_);

  doorButton_ = new Button(Rect(0.52, 0.05, 0.2, 0.05), false);
  TextureParams regular3 = {100, 114, 54, 37, 10, 6, 10, 6};
  TextureParams hovered3 = {155, 114, 54, 37, 10, 6, 10, 6};
  TextureParams pressed3 = {210, 114, 54, 37, 10, 6, 10, 6};
  doorButton_->init(regular3, hovered3, pressed3);
  doorButton_->sigClick.connect(this, &BuildTools::doorClick);
  addWidget(doorButton_);

  eraseButton_ = new Button(Rect(0.76, 0.05, 0.2, 0.05), false);
  TextureParams regular1 = {100, 38, 54, 37, 10, 6, 10, 6};
  TextureParams hovered1 = {155, 38, 54, 37, 10, 6, 10, 6};
  TextureParams pressed1 = {210, 38, 54, 37, 10, 6, 10, 6};
  eraseButton_->init(regular1, hovered1, pressed1);
  eraseButton_->sigClick.connect(this, &BuildTools::eraseClick);
  addWidget(eraseButton_);
}

void BuildTools::buildClick()
{
  shipView_->buildWalls();
  buildButton_->setHighlighted(true);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
}

void BuildTools::eraseClick()
{
  shipView_->erase();
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(true);
}

void BuildTools::floorClick()
{
  shipView_->buildFloor();
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(true);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
}

void BuildTools::doorClick()
{
  shipView_->buildDoor();
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(true);
  eraseButton_->setHighlighted(false);
}
