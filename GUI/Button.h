#pragma once
#include "Text.h"

class Button: public Text
{
public:
  Button(Rect size, bool defaultTexture = true);
  ~Button();
  signal0<> sigClick;
  signal1<void*> sig1Click;
  void onClick();
  void init(TextureParams regular, TextureParams hovered, TextureParams pressed);
  void setParam(void* param) {param_ = param;}
private:
  void* param_;
};
