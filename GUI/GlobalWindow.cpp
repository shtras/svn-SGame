#include "StdAfx.h"
#include "GlobalWindow.h"
#include "Window.h"
#include "Renderer.h"

GlobalWindow::GlobalWindow( Rect size ):Widget(size)
{

}

GlobalWindow::~GlobalWindow()
{

}

void GlobalWindow::onDrop( Widget* w )
{
  if (!dynamic_cast<Window*>(w)) {
    return;
  }
  if (std::find(children_.begin(), children_.end(), w) == children_.end()) {
    return;
  }
  Rect newSize = w->getSize();
  float offsetX = Renderer::getInstance().getOffsetX();
  float offsetY = Renderer::getInstance().getOffsetY();
  newSize.left -= offsetX;
  newSize.top -= offsetY;
  w->resize(newSize);
}
