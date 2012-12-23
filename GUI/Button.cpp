#include "StdAfx.h"
#include "Button.h"
#include "Renderer.h"

Button::Button(Rect size, bool defaultTexture/* = true*/):Text(size)
{
  if (defaultTexture) {
    TextureParams params = {1, 63, 32, 15, 5, 4, 5, 4};
    initRegular(params);
    TextureParams paramsHovered = {34, 63, 32, 15, 5, 4, 5, 4};
    initHovered(paramsHovered);
    TextureParams paramsPressed = {67, 63, 32, 15, 5, 4, 5, 4};
    initPressed(paramsPressed);
  }
  clickable_ = true;
}

Button::~Button()
{

}

void Button::onClick()
{
  sigClick.emit();
}

void Button::init( TextureParams regular, TextureParams hovered, TextureParams pressed )
{
  assert (!regularTexture_ && !hoveredTexture_ && !pressedTexture_);
  initRegular(regular);
  initHovered(hovered);
  initPressed(pressed);
}

