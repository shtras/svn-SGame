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
  float width = Renderer::getInstance().getCharWidth() * caption_.getSize();
  if (width > size_.width * 0.95f) {
    float multiplier = width / size_.width / 0.95f;
    float newSize = textSize_ / multiplier;
    Renderer::getInstance().setTextSize(newSize);
  }
  Renderer::getInstance().setColor(color_);
  float left = size_.left;
  if (align_ != Widget::LeftAlign) {
    left = size_.left + size_.width/2 - caption_.getSize()*Renderer::getInstance().getCharWidth()/2.0f;
  }
  Renderer::getInstance().renderText(left, 
    size_.top + size_.height/2 - Renderer::getInstance().getCharHeight()*0.5f, caption_);
  Renderer::getInstance().resetColor();
}

FramedText::FramedText( Rect size ):Text(size)
{
  TextureParams params = {1, 79, 65, 17, 7, 7, 7, 7};
  initRegular(params);
}

FramedText::~FramedText()
{

}
