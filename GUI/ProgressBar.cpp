#include "StdAfx.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar( Rect size ):Widget(size)
{
  TextureParams bgParams = {265, 0, 4, 30, 1, 1, 1, 1};
  TextureParams selectionParams = {270, 0, 4, 30, 1, 1, 1, 1};
  bg_ = new TexturedWidget(size);
  bg_->init(bgParams);
  selection_ = new TexturedWidget(size);
  selection_->init(selectionParams);
}

ProgressBar::~ProgressBar()
{
  delete bg_;
  delete selection_;
}

void ProgressBar::customFrameRender()
{
  bg_->render();
  selection_->render();
}

void ProgressBar::setParams( int numItems, int firstItem, int lastItem )
{
  if (numItems == 0) {
    numItems = 1;
    firstItem = 0;
    lastItem = 1;
  }
  bg_->resize(size_);
  numItems_ = numItems;
  firstItem_ = firstItem;
  lastItem_ = lastItem;
  Rect selectionSize = size_;
  float begin = firstItem / (float) numItems;
  float end = lastItem / (float) numItems;
  selectionSize.top = size_.top + size_.height*begin;
  selectionSize.height = size_.height * (end - begin);
  selection_->resize(selectionSize);
}
