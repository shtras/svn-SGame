#include "StdAfx.h"
#include "Window.h"


Window::Window(Rect size):
  GlobalWindow(size)
{
  TextureParams params = {0, 0, 40, 40, 8, 8, 8, 8};
  initRegular(params);
  TextureParams paramsHovered = {0, 0, 40, 40, 8, 8, 8, 8};
  initHovered(paramsHovered);
}

Window::~Window()
{

}
