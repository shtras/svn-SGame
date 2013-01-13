#pragma once
#include "Text.h"

class Input: public FramedText
{
public:
  Input(Rect size);
  ~Input();
  void onKeyDown(SDL_keysym details);
  void onKeyboardFocusGain();
  void onKeyboardFocusLose();
  CString getCaption();
private:
  bool inFocus_;
};
