#include "StdAfx.h"
#include "Button.h"
#include "Renderer.h"

Button::Button(Rect size, bool defaultTexture/* = true*/):Text(size),param_((void*)0xbadbeef)
{
  if (defaultTexture) {
    TextureParams params = {1, 63, 32, 15, 6, 6, 6, 6};
    initRegular(params);
    TextureParams paramsHovered = {34, 63, 32, 15, 6, 6, 6, 6};
    initHovered(paramsHovered);
    TextureParams paramsPressed = {67, 63, 32, 15, 6, 6, 6, 6};
    initPressed(paramsPressed);
  }
  clickable_ = true;
}

Button::~Button()
{

}

void Button::onClick()
{
  if (param_ != (void*)0xbadbeef) {
    sig1Click.emit(param_);
  } else {
    sigClick.emit();
  }
}

void Button::init( TextureParams regular, TextureParams hovered, TextureParams pressed )
{
  assert (!regularTexture_ && !hoveredTexture_ && !pressedTexture_);
  initRegular(regular);
  initHovered(hovered);
  initPressed(pressed);
}

