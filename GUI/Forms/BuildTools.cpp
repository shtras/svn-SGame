#include "StdAfx.h"
#include "BuildTools.h"
#include "Renderer.h"
#include "ItemsDB.h"
#include "CompartmentButton.h"

BuildTools::BuildTools( Rect size ):Window(size),firstButton_(-1), currentCategory_((Compartment::Category)0)
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
  buildButton_->setToolTip("Build walls");

  floorButton_ = new Button(Rect(0.28, 0.02, 0.2, 0.05), false);
  TextureParams regular2 = {100, 76, 54, 37, 10, 6, 10, 6};
  TextureParams hovered2 = {155, 76, 54, 37, 10, 6, 10, 6};
  TextureParams pressed2 = {210, 76, 54, 37, 10, 6, 10, 6};
  floorButton_->init(regular2, hovered2, pressed2);
  floorButton_->sigClick.connect(this, &BuildTools::floorClick);
  addWidget(floorButton_);
  floorButton_->setToolTip("Build floor");

  doorButton_ = new Button(Rect(0.52, 0.02, 0.2, 0.05), false);
  TextureParams regular3 = {100, 114, 54, 37, 10, 6, 10, 6};
  TextureParams hovered3 = {155, 114, 54, 37, 10, 6, 10, 6};
  TextureParams pressed3 = {210, 114, 54, 37, 10, 6, 10, 6};
  doorButton_->init(regular3, hovered3, pressed3);
  doorButton_->sigClick.connect(this, &BuildTools::doorClick);
  addWidget(doorButton_);
  doorButton_->setToolTip("Build door");

  eraseButton_ = new Button(Rect(0.76, 0.02, 0.2, 0.05), false);
  TextureParams regular1 = {100, 38, 54, 37, 10, 6, 10, 6};
  TextureParams hovered1 = {155, 38, 54, 37, 10, 6, 10, 6};
  TextureParams pressed1 = {210, 38, 54, 37, 10, 6, 10, 6};
  eraseButton_->init(regular1, hovered1, pressed1);
  eraseButton_->sigClick.connect(this, &BuildTools::eraseClick);
  addWidget(eraseButton_);
  eraseButton_->setToolTip("Demolish");

  float lastX = 0.02f;
  float lastY = 0.08f;
  firstButton_ = 0;
  float width = 0.317f;
  for (int i=0; i<Compartment::LastCategory; ++i) {
    Rect size(lastX, lastY, width, 0.05f);
    Button* but = new Button(size);
    but->setCaption(Compartment::categoryName((Compartment::Category)i));
    but->setParam((void*)i);
    but->sig1Click.connect(this, &BuildTools::categoryClick);
    but->setColor(Vector4(255,0,0,255));
    categoryButtons_.push_back(but);
    addWidget(but);
    lastX += width + 0.001f;
    if (lastX >= 0.7f) {
      lastX = 0.02f;
      lastY += 0.05f;
    }

    list<Compartment*> comps = ItemsDB::getInstance().getCompartmentsByCategory((Compartment::Category)i);
    float lastTop = 0.2f;
    float aspect = size_.width / size_.height * Renderer::getInstance().getWidth() / (float)Renderer::getInstance().getHeight();
    int count = 0;
    compButtons_[i] = vector<CompartmentButton*>();
    for (auto itr = comps.begin(); itr != comps.end(); ++itr) {
      Compartment* comp = *itr;
      float width = 0.85f;
      if (comp->getWidth() < 5) {
        width = 0.85f / (5-(float)comp->getWidth());
      }
      float height = width * aspect * comp->getHeight() / (float)comp->getWidth();
      if (height > 0.25f) {
        width *= 0.5f;
        height *= 0.5f;
      }
      CompartmentButton* button = new CompartmentButton(Rect(0.02f, lastTop, width, height), comp, shipView_);
      addWidget(button);
      button->setToolTip(comp->getName());
      compButtons_[i].push_back(button);
      lastTop += height + 0.02f;
      if (lastTop >= 0.95f || i != currentCategory_) {
        button->setVisible(false);
      }
      ++count;
    }
  }
  categoryButtons_[currentCategory_]->setHighlighted(true);
}

void BuildTools::categoryClick(void* param)
{
  int iParam = (int)param;
  for (int i=0; i<Compartment::LastCategory; ++i) {
    Button* but = categoryButtons_[i];
    but->setHighlighted(i == iParam);
  }
  for (auto itr = compButtons_[currentCategory_].begin(); itr != compButtons_[currentCategory_].end(); ++itr) {
    (*itr)->setVisible(false);
  }
  currentCategory_ = (Compartment::Category)iParam;
  firstButton_ = 0;
  if (compButtons_[currentCategory_].size() == 0) {
    return;
  }
  float lastTop = compButtons_[currentCategory_][0]->getSize().top;
  for (auto itr = compButtons_[currentCategory_].begin(); itr != compButtons_[currentCategory_].end(); ++itr) {
    CompartmentButton* but = *itr;
    Rect size = but->getSize();
    size.top = lastTop;
    but->resize(size);
    but->setVisible(true);
    lastTop += size.height + 0.02f;
    if (lastTop >= 0.98f) {
      but->setVisible(false);
    }
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

void BuildTools::onMouseWheelScroll( int direction )
{
  if (compButtons_[currentCategory_].size() == 0) {
    return;
  }
  if (direction == -1 && compButtons_[currentCategory_][compButtons_[currentCategory_].size()-1]->isVisible()) {
    return;
  }
  firstButton_ -= direction;
  if (firstButton_ < 0) {
    firstButton_ = 0;
    return;
  }
  float lastTop = compButtons_[currentCategory_][0]->getSize().top;
  bool fitsScreen = true;
  for (int i=0; i<(int)compButtons_[currentCategory_].size(); ++i) {
    CompartmentButton* button = compButtons_[currentCategory_][i];
    Rect oldSize = button->getSize();
    if (i < firstButton_ || !fitsScreen) {
      button->setVisible(false);
    } else {
      button->setVisible(true);
      oldSize.top = lastTop;
      button->resize(oldSize);
      lastTop += oldSize.height + 0.02f;
      if (lastTop >= 0.95f) {
        fitsScreen = false;
        button->setVisible(false);
      }
    }
  }
}
