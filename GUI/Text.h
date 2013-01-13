#pragma once
#include "TexturedWidget.h"

class Text: public Widget
{
public:
  Text(Rect size);
  ~Text();
  void render();
  void setCaption(CString caption) {caption_ = caption;}
  void setTextSize(float size) {textSize_ = size;}
  void setColor(Vector4 color) {color_ = color;}
protected:  
  CString caption_;
  float textSize_;
  Vector4 color_;
};

class FramedText: public Text
{
public:
  FramedText(Rect size);
  ~FramedText();
private:
};
