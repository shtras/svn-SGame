#pragma once
#include "Text.h"

class Button: public Text
{
public:
  Button(Rect size, bool defaultTexture = true);
  ~Button();
  signal0<> sigClick;
  void onClick();
  void init(TextureParams regular, TextureParams hovered, TextureParams pressed);
private:
};
