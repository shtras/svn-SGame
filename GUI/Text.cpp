#include "StdAfx.h"
#include "Text.h"
#include "Renderer.h"

Text::Text(Rect size):Widget(size),textSize_(1.0f),color_(Vector4(255, 255, 255, 255))
{

}

Text::~Text()
{

}

void Text::render()
{
  Renderer::getInstance().setTextSize(textSize_);
  Renderer::getInstance().setColor(color_);
  Renderer::getInstance().renderText(size_.left + size_.width/2 - caption_.getSize()*Renderer::getInstance().getCharWidth()/2.0f, 
    size_.top + size_.height/2 - Renderer::getInstance().getCharHeight()*0.5f, caption_);
  Renderer::getInstance().resetColor();
}
