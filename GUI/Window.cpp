#include "StdAfx.h"
#include "Window.h"


Window::Window(Rect size):
  GlobalWindow(size)
{
  TextureParams params = {0, 31, 94, 30, 8, 8, 8, 8};
  initRegular(params);
  TextureParams paramsHovered = {0, 0, 30, 30, 8, 8, 8, 8};
  initHovered(paramsHovered);
  draggable_ = true;
}

Window::~Window()
{

}
