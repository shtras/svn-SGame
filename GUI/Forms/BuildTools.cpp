#include "StdAfx.h"
#include "BuildTools.h"
#include "Renderer.h"
#include "ItemsDB.h"
#include "CompartmentButton.h"

BuildTools::BuildTools( Rect size ):Window(size)
{
  draggable_ = false;
  clickable_ = true;
}

BuildTools::~BuildTools()
{

}

void BuildTools::init(ShipView* shipView)
{
  shipView_ = shipView;
  buildButton_ = new Button(Rect(0.04, 0.02, 0.2, 0.05), false);
  TextureParams regular = {100, 0, 54, 37, 10, 6, 10, 6};
  TextureParams hovered = {155, 0, 54, 37, 10, 6, 10, 6};
  TextureParams pressed = {210, 0, 54, 37, 10, 6, 10, 6};
  buildButton_->init(regular, hovered, pressed);
  buildButton_->sigClick.connect(this, &BuildTools::buildClick);
  buildButton_->setHighlighted(true);
  addWidget(buildButton_);

  floorButton_ = new Button(Rect(0.28, 0.02, 0.2, 0.05), false);
  TextureParams regular2 = {100, 76, 54, 37, 10, 6, 10, 6};
  TextureParams hovered2 = {155, 76, 54, 37, 10, 6, 10, 6};
  TextureParams pressed2 = {210, 76, 54, 37, 10, 6, 10, 6};
  floorButton_->init(regular2, hovered2, pressed2);
  floorButton_->sigClick.connect(this, &BuildTools::floorClick);
  addWidget(floorButton_);

  doorButton_ = new Button(Rect(0.52, 0.02, 0.2, 0.05), false);
  TextureParams regular3 = {100, 114, 54, 37, 10, 6, 10, 6};
  TextureParams hovered3 = {155, 114, 54, 37, 10, 6, 10, 6};
  TextureParams pressed3 = {210, 114, 54, 37, 10, 6, 10, 6};
  doorButton_->init(regular3, hovered3, pressed3);
  doorButton_->sigClick.connect(this, &BuildTools::doorClick);
  addWidget(doorButton_);

  eraseButton_ = new Button(Rect(0.76, 0.02, 0.2, 0.05), false);
  TextureParams regular1 = {100, 38, 54, 37, 10, 6, 10, 6};
  TextureParams hovered1 = {155, 38, 54, 37, 10, 6, 10, 6};
  TextureParams pressed1 = {210, 38, 54, 37, 10, 6, 10, 6};
  eraseButton_->init(regular1, hovered1, pressed1);
  eraseButton_->sigClick.connect(this, &BuildTools::eraseClick);
  addWidget(eraseButton_);

  list<Compartment*> comps = ItemsDB::getInstance().getCompartmentsByCategory(Compartment::Navigation);
  float lastTop = 0.1f;
  float aspect = size_.width / size_.height * Renderer::getInstance().getWidth() / (float)Renderer::getInstance().getHeight();
  for (auto itr = comps.begin(); itr != comps.end(); ++itr) {
    Compartment* comp = *itr;
    float width = 0.8f;
    float height = width * aspect * comp->getHeight() / (float)comp->getWidth();
    CompartmentButton* button = new CompartmentButton(Rect(0.1f, lastTop, width, height), comp, shipView_);
    addWidget(button);
    lastTop += height + 0.02f;
  }
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

void BuildTools::onClick()
{
  cout << "aaa" << endl;
}

void BuildTools::render()
{
  //Renderer& renderer = Renderer::getInstance();
  //GLuint texID = renderer.getTilesTex();
  //float lastTop = 0.3f;
  //float aspect = renderer.getWidth() / (float)renderer.getHeight();
  //list<Compartment*> compartments = ItemsDB::getInstance().getCompartmentsByCategory(Compartment::Navigation);
  //for (auto itr = compartments.begin(); itr != compartments.end(); ++itr) {
  //  Compartment* comp = *itr;

  //  float tileWidth = size_.width / (float)comp->getWidth() * 0.8f;
  //  float tileHeight = tileWidth * aspect;

  //  const list<Item*>& items = comp->getItems();
  //  for (auto itemItr = items.begin(); itemItr != items.end(); ++itemItr) {
  //    Item* item = *itemItr;
  //    Rect pos(item->getX() * tileWidth, lastTop + item->getY()*tileHeight, tileWidth, tileHeight);
  //    Rect texPos(item->getTexX() + item->getRotation()*item->getTexWidth(), item->getTexY(), item->getTexWidth(), item->getTexHeight());
  //    renderer.drawTexRect(pos, texID, texPos);
  //  }
  //  lastTop += tileHeight * comp->getHeight() * 1.2f;
  //}
}
