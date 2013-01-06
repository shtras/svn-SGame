#include "StdAfx.h"
#include "CompartmentButton.h"
#include "Renderer.h"
#include "Text.h"

CompartmentButton::CompartmentButton(Rect size, Compartment* comp, ShipView* shipView):Button(size, false),comp_(comp),shipView_(shipView),hoverWindow_(NULL)
{
  assert(comp);
  TextureParams params1 = {100, 152, 31, 31, 4, 4, 4, 4};
  TextureParams params2 = {132, 152, 31, 31, 4, 4, 4, 4};
  initRegular(params1);
  initHovered(params2);
  initPressed(params2);
  draggable_ = true;
  scrollTransparrent_ = true;

  double top = 0.01;
  double height = 0.1;
  hoverWindow_ = new Window(Rect(0.0, 0.0, 0.2, 0.2));
  Text* t = new Text(Rect(0.01, top, 0.98, height));
  t->setCaption(comp->getName());
  hoverWindow_->addWidget(t);
  top += height + 0.01;

  if (comp->getMinCrew() > 0 || comp->getMaxCrew() > 0) {
    t = new Text(Rect(0.01, top, 0.98, height));
    t->setCaption("Required crew: " + CString(comp->getMinCrew()) + "(" + CString(comp->getMaxCrew()) + ")");
    t->setAlign(Widget::LeftAlign);
    hoverWindow_->addWidget(t);
    top += height + 0.01;
  }

  if (comp->getPowerProduced() > 0) {
    t = new Text(Rect(0.01, top, 0.98, height));
    t->setCaption("Produced power: " + CString(comp->getPowerProduced()));
    t->setAlign(Widget::LeftAlign);
    hoverWindow_->addWidget(t);
    top += height + 0.01;
  }

  if (comp->getPowerRequired() > 0) {
    t = new Text(Rect(0.01, top, 0.98, height));
    t->setCaption("Required power: " + CString(comp->getPowerRequired()));
    t->setAlign(Widget::LeftAlign);
    hoverWindow_->addWidget(t);
    top += height + 0.01;
  }

  if (comp->getCrewCapacity() > 0) {
    t = new Text(Rect(0.01, top, 0.98, height));
    t->setCaption("Crew capacity: " + CString(comp->getCrewCapacity()));
    t->setAlign(Widget::LeftAlign);
    hoverWindow_->addWidget(t);
    top += height + 0.01;
  }
}

CompartmentButton::~CompartmentButton()
{
  delete hoverWindow_;
}

void CompartmentButton::render()
{
  Renderer& renderer = Renderer::getInstance();
  GLuint texID = renderer.getTilesTex();
  float aspect = renderer.getWidth() / (float)renderer.getHeight();
  float tileWidth = size_.width / (float)comp_->getWidth();
  float tileHeight = tileWidth * aspect;

  const list<Item*>& items = comp_->getItems();
  for (auto itemItr = items.begin(); itemItr != items.end(); ++itemItr) {
    Item* item = *itemItr;
    Rect pos(item->getX() * tileWidth + size_.left, item->getY()*tileHeight + size_.top, tileWidth, tileHeight);
    Rect texPos(item->getTexX() + item->getRotation()*item->getTexWidth(), item->getTexY(), item->getTexWidth(), item->getTexHeight());
    renderer.drawTexRect(pos, texID, texPos);
  }
}

void CompartmentButton::onDrag()
{
  shipView_->setHoveredDimensions(comp_->getWidth(), comp_->getHeight());
  shipView_->setDraggedComp(comp_);
}

void CompartmentButton::onStopDrag()
{
  shipView_->setHoveredDimensions(1,1);
  shipView_->setDraggedComp(NULL);
}

void CompartmentButton::onHoverEnter()
{
  Renderer::getInstance().setFloatingWidget(hoverWindow_);
}

void CompartmentButton::onHoverExit()
{
  Renderer::getInstance().setFloatingWidget(NULL);
}
