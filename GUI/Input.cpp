#include "StdAfx.h"
#include "Input.h"

Input::Input( Rect size ):FramedText(size),inFocus_(false)
{
  listensKeyboard_ = true;
  clickable_ = true;
  align_ = LeftAlign;
}

Input::~Input()
{

}

void Input::onKeyDown( SDL_keysym details )
{
  assert (inFocus_);
  caption_ = caption_.remove('|');
  char sym;
  if (details.sym == SDLK_BACKSPACE) {
    if (caption_.getSize() == 0) {
      return;
    }
    caption_ = caption_.left(caption_.getSize()-1);
    return;
  } else if (details.sym >= SDLK_a && details.sym <= SDLK_z) {
    sym = details.sym - SDLK_a + 'a';
    if ((details.mod & KMOD_LSHIFT) != 0 || (details.mod & KMOD_RSHIFT) != 0) {
      sym = sym - 'a' + 'A';
    }
  } else if (details.sym >= SDLK_0 && details.sym <= SDLK_9) {
    sym = details.sym - SDLK_0 + '0';
  } else if (details.sym == SDLK_PERIOD) {
    sym = '.';
  } else {
    return;
  }
  caption_ += sym;
  caption_ += "|";
}

void Input::onKeyboardFocusGain()
{
  caption_ += "|";
  inFocus_ = true;
}

void Input::onKeyboardFocusLose()
{
  if (caption_.contains('|')) {
    caption_ = caption_.remove('|');
  }
  inFocus_ = false;
}

CString Input::getCaption()
{
  if (inFocus_) {
    assert (caption_.contains('|'));
    return caption_.remove('|');
  }
  return caption_;
}

