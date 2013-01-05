#include "StdAfx.h"
#include "TextArea.h"
#include "Renderer.h"

TextArea::TextArea(Rect size):Widget(size), pb_(NULL), maxSize_(150), first_(0), maxLinesDisplayed_(0)
{
  TextureParams params = {1, 79, 65, 17, 7, 7, 7, 7};
  initRegular(params);

  pb_ = new ProgressBar(Rect(0.95, 0.02, 0.04, 0.96));
  pb_->setParams(0, 0, 0);
  addWidget(pb_);
}

TextArea::~TextArea()
{
}

void TextArea::onResize()
{
  Renderer& renderer = Renderer::getInstance();
  float charHeight = renderer.getCharHeight();
  maxLinesDisplayed_ = (int)((size_.height - 0.02f) / charHeight);
  resizePB();
}

void TextArea::pushLine( CString line )
{
  lines_.push_back(line);
  if ((int)lines_.size() >= maxSize_) {
    lines_.pop_front();
  }
  if ((int)lines_.size() > maxLinesDisplayed_) {
    first_ = (int)lines_.size() - maxLinesDisplayed_;
  }
  resizePB();
}

void TextArea::onMouseWheelScroll( int direction )
{
  first_ -= direction;
  if (first_ + maxLinesDisplayed_ >= (int)lines_.size()) {
    first_ = lines_.size() - maxLinesDisplayed_;
  }
  if (first_ < 0) {
    first_ = 0;
  }
  resizePB();
}

void TextArea::render()
{
  Renderer& renderer = Renderer::getInstance();
  float charHeight = renderer.getCharHeight();
  float lastTop = 0.01f + size_.top;
  int cnt = -1;
  for (auto itr = lines_.begin(); itr != lines_.end(); ++itr) {
    ++cnt;
    if (cnt < first_ || cnt >= first_ + maxLinesDisplayed_) {
      continue;
    }
    CString& line = *itr;
    renderer.renderText(size_.left + 0.01f, lastTop, line);
    lastTop += charHeight;
  }
}

void TextArea::resizePB()
{
  int last = first_ + maxLinesDisplayed_;
  if (last >= (int)lines_.size()) {
    last = lines_.size();
  }
  pb_->setParams(lines_.size(), first_, last);
}
