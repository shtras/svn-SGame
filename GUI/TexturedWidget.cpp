#include "StdAfx.h"
#include "TexturedWidget.h"
#include "Renderer.h"

Widget::Widget(Rect size):
  size_(size), regularTexture_(NULL), hoveredTexture_(NULL), pressedTexture_(NULL), activeTexture_(NULL), hovered_(false), pressed_(false), clickable_(false),
  draggable_(false), dragged_(false),highlighted_(false),scrollTransparrent_(false),visible_(true),align_(NoAlign),listensKeyboard_(false)
{

}

Widget::~Widget()
{
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* child = *itr;
    delete child;
  }
  delete regularTexture_;
  delete hoveredTexture_;
  delete pressedTexture_;
}

void Widget::renderFrameRec()
{
  Renderer& renderer = Renderer::getInstance();
  if (!visible_) {
    return;
  }
  if (!renderer.renderingDragged() && renderer.getDraggedWidget() == this) {
    return;
  }
  Renderer::getInstance().setColor(Vector3(255,255,255));
  if (activeTexture_) {
    activeTexture_->render();
  }
  customFrameRender();
  //Draw all the children
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* child = *itr;
    child->renderFrameRec();
  }
  if (hovered_ && toolTip_ != "") {
    renderer.setWidgetForTooltip(this);
  }
}

void Widget::renderContRec()
{
  if (!visible_) {
    return;
  }
  if (!Renderer::getInstance().renderingDragged() && Renderer::getInstance().getDraggedWidget() == this) {
    return;
  }
  //virtual render for each widget
  render();
  //Draw all the children
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* child = *itr;
    child->renderContRec();
  }
}

void Widget::initRegular( TextureParams params )
{
  regularTexture_ = new TexturedWidget(size_);
  regularTexture_->init(params);
  activeTexture_ = regularTexture_;
}

void Widget::initHovered( TextureParams params )
{
  hoveredTexture_ = new TexturedWidget(size_);
  hoveredTexture_->init(params);
}

void Widget::initPressed( TextureParams params )
{
  pressedTexture_ = new TexturedWidget(size_);
  pressedTexture_->init(params);
}

void Widget::lmDown()
{
  if (pressedTexture_) {
    activeTexture_ = pressedTexture_;
  }
  pressed_ = true;
  if (draggable_) {
    drag();
    Renderer::getInstance().setDraggedWidget(this);
  }
  onLMDown();
}

void Widget::rmDown()
{
  onRMDown();
}

void Widget::lmUp()
{
  activeTexture_ = hoveredTexture_?hoveredTexture_:regularTexture_;
  Renderer::getInstance().setKeyboardListener(NULL);
  if (pressed_ && clickable_) {
    click();
  }
  Widget* draggedWidget = Renderer::getInstance().getDraggedWidget();
  if (draggedWidget && draggedWidget != this) {
    drop(draggedWidget);
  }
  if (draggedWidget) {
    draggedWidget->stopDrag();
    Renderer::getInstance().setDraggedWidget(NULL);
  }
  pressed_ = false;
  onLMUp();
}

void Widget::rmUp()
{
  onRMUp();
}

void Widget::click()
{
  assert (clickable_);
  if (listensKeyboard_) {
    Renderer::getInstance().setKeyboardListener(this);
  } else {
    Renderer::getInstance().setKeyboardListener(NULL);
  }
  onClick();
}

void Widget::hoverEnter()
{
  hovered_ = true;
  if (hoveredTexture_ && !highlighted_) {
    activeTexture_ = hoveredTexture_;
  }
  onHoverEnter();
}

void Widget::hoverExit()
{
  hovered_ = false;
  pressed_ = false;
  activeTexture_ = regularTexture_;
  if (highlighted_ && pressedTexture_) {
    activeTexture_ = pressedTexture_;
  }
  onHoverExit();
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    (*itr)->hoverExit();
  }
}

void Widget::drag()
{
  onDrag();
}

void Widget::stopDrag()
{
  onStopDrag();
}

void Widget::drop( Widget* w )
{
  onDrop(w);
}

void Widget::mouseMove()
{
  onMouseMove();
}

void Widget::mouseWheelScroll(int direction)
{
  onMouseWheelScroll(direction);
}

void Widget::keyDown( SDL_keysym details )
{
  assert(listensKeyboard_);
  onKeyDown(details);
}

void Widget::addWidget( Widget* widget )
{
  children_.push_back(widget);
  Rect newSize;
  newSize.left = size_.left + widget->size_.left*size_.width;
  newSize.top = size_.top + widget->size_.top*size_.height;
  newSize.width = widget->size_.width * size_.width;
  newSize.height = widget->size_.height * size_.height;
  widget->resize(newSize);
}

void Widget::resize( Rect size )
{
  Rect oldSize = size_;
  size_ = size;
  if (regularTexture_) {
    regularTexture_->resize(size);
  }
  if (hoveredTexture_) {
    hoveredTexture_->resize(size);
  }
  if (pressedTexture_) {
    pressedTexture_->resize(size);
  }
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* child = *itr;
    Rect newSize;
    float widthChange = size.width / oldSize.width;
    float heightChange = size.height / oldSize.height;
    newSize.left = (child->size_.left - oldSize.left)*widthChange + size_.left;
    newSize.top = (child->size_.top - oldSize.top)*heightChange + size_.top;
    newSize.width = child->size_.width * widthChange;
    newSize.height = child->size_.height * heightChange;
    child->resize(newSize);
  }
  onResize();
}

void Widget::removeWidget( Widget* widget )
{
  children_.remove(widget);
}

void Widget::clear()
{
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* w = *itr;
    delete w;
  }
  children_.clear();
}

bool Widget::handleEvent( SDL_Event& event, float fx, float fy )
{
  if (!visible_) {
    return false;
  }
  if (Renderer::getInstance().getDraggedWidget() == this) {
    return false; //Dragged widget doesn't receive events
  }
  if ((fx < size_.left || fx > size_.left + size_.width) || (fy < size_.top || fy > size_.top + size_.height)) {
    if (hovered_) {
      hoverExit();
    }
    return false;
  }
  if (!hovered_) {
    hoverEnter();
  }
  bool childHandeled = false;
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    Widget* child = *itr;
    childHandeled |= child->handleEvent(event, fx, fy);
  }

  bool handled = true;
  if (!childHandeled) { //Button events not handled by any child
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        lmDown();
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        rmDown();
      } else if (event.button.button == SDL_BUTTON_WHEELUP) {
        if (scrollTransparrent_) {
          handled = false;
        } else {
          mouseWheelScroll(1);
        }
      } else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
        if (scrollTransparrent_) {
          handled = false;
        } else {
          mouseWheelScroll(-1);
        }
      }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        lmUp();
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        rmUp();
      }
    } else if (event.type == SDL_MOUSEMOTION) {
      mouseMove();
    }
  }

  return handled;
}

void Widget::setHighlighted( bool value )
{
  if (value && pressedTexture_) {
    activeTexture_ = pressedTexture_;
  }
  if (!value) {
    activeTexture_ = regularTexture_;
  }
  highlighted_ = value;
}

void Widget::keyboardFocusGain()
{
  onKeyboardFocusGain();
}

void Widget::keyboardFocusLose()
{
  onKeyboardFocusLose();
}

TexturedWidget::TexturedWidget(Rect size):dimensionsInitialized_(false), size_(size)
{

}

TexturedWidget::~TexturedWidget()
{

}

void TexturedWidget::init( TextureParams params )
{
  texParams_ = params;
}

void TexturedWidget::render()
{
  Renderer& renderer = Renderer::getInstance();
  if (!dimensionsInitialized_) {
    renderer.initWidgetDimensions(this);
  }
  //int x = (int)(renderer.getWidth() * body_.left);
  //int y = (int)(renderer.getHeight() * body_.top);
  //int width = (int)(renderer.getWidth() * body_.width);
  //int height = (int)(renderer.getHeight() * body_.height);
  //renderer.flushVerts();
  //glScissor(x, renderer.getHeight() - y, width, height);
  //glEnable(GL_SCISSOR_TEST);
  
  GLuint guitex = renderer.getGUITex();
  renderer.drawTexRect(topLeft_, guitex, topLeftTex_);
  renderer.drawTexRect(left_, guitex, leftTex_);
  renderer.drawTexRect(bottomLeft_, guitex, bottomLeftTex_);
  renderer.drawTexRect(top_, guitex, topTex_);
  renderer.drawTexRect(body_, guitex, bodyTex_);
  renderer.drawTexRect(bottom_, guitex, bottomTex_);
  renderer.drawTexRect(topRight_, guitex, topRightTex_);
  renderer.drawTexRect(right_, guitex, rightTex_);
  renderer.drawTexRect(bottomRight_, guitex, bottomRightTex_);
  
  //glScissor(0, 0, renderer.getWidth(), renderer.getHeight());
  //renderer.flushVerts();
  //glDisable(GL_SCISSOR_TEST);
}

void TexturedWidget::initDimensions( int width, int height, int texWidth, int texHeight )
{
  dimensionsInitialized_ = true;
  float fWidth = (float)width;
  float fHeight = (float)height;
  float fTexWidth = (float)texWidth;
  float fTexHeight = (float)texHeight;

  topLeft_.left = size_.left;
  topLeft_.top = size_.top;
  topLeft_.width = texParams_.borderLeft/fWidth;
  topLeft_.height = texParams_.borderTop/fHeight;
  topLeftTex_.left = texParams_.textureLeft/fTexWidth;
  topLeftTex_.top = texParams_.textureTop/fTexHeight;
  topLeftTex_.width = texParams_.borderLeft/fTexWidth;
  topLeftTex_.height = texParams_.borderTop/fTexHeight;

  left_.left = size_.left;
  left_.top = size_.top + texParams_.borderTop/fHeight;
  left_.width = texParams_.borderLeft/fWidth;
  left_.height = size_.height - texParams_.borderTop/fHeight - texParams_.borderDown/fHeight;
  leftTex_.left = texParams_.textureLeft/fTexWidth;
  leftTex_.top = texParams_.textureTop/fTexHeight + texParams_.borderTop/fTexHeight;
  leftTex_.width = texParams_.borderLeft/fTexWidth;
  leftTex_.height = texParams_.textureHeight/fTexHeight - texParams_.borderTop/fTexHeight - texParams_.borderDown/fTexHeight;

  bottomLeft_.left = size_.left;
  bottomLeft_.top = size_.top + size_.height - texParams_.borderDown/fHeight;
  bottomLeft_.width = texParams_.borderLeft/fWidth;
  bottomLeft_.height = texParams_.borderDown/fHeight;
  bottomLeftTex_.left = texParams_.textureLeft/fTexWidth;
  bottomLeftTex_.top = texParams_.textureTop/fTexHeight + texParams_.textureHeight/fTexHeight - texParams_.borderDown/fTexHeight;
  bottomLeftTex_.width = texParams_.borderLeft/fTexWidth;
  bottomLeftTex_.height = texParams_.borderDown/fTexHeight;

  top_.left = size_.left + texParams_.borderLeft/fWidth;
  top_.top = size_.top;
  top_.width = size_.width - texParams_.borderLeft/fWidth - texParams_.borderRight/fWidth;
  top_.height = texParams_.borderTop/fHeight;
  topTex_.left = texParams_.textureLeft/fTexWidth + texParams_.borderLeft/fTexWidth;
  topTex_.top = texParams_.textureTop/fTexHeight;
  topTex_.width = texParams_.textureWidth/fTexWidth - texParams_.borderLeft/fTexWidth - texParams_.borderRight/fTexWidth;
  topTex_.height = texParams_.borderTop/fTexHeight;

  body_.left = size_.left + texParams_.borderLeft/fWidth;
  body_.top = size_.top + texParams_.borderTop/fHeight;
  body_.width = size_.width - texParams_.borderLeft/fWidth - texParams_.borderRight/fWidth;
  body_.height = size_.height - texParams_.borderTop/fHeight - texParams_.borderDown/fHeight;
  bodyTex_.left = texParams_.textureLeft/fTexWidth + texParams_.borderLeft/fTexWidth;
  bodyTex_.top = texParams_.textureTop/fTexHeight + texParams_.borderTop/fTexHeight;
  bodyTex_.width = texParams_.textureWidth/fTexWidth - texParams_.borderLeft/fTexWidth - texParams_.borderRight/fTexWidth;
  bodyTex_.height = texParams_.textureHeight/fTexHeight - texParams_.borderTop/fTexHeight - texParams_.borderDown/fTexHeight;

  bottom_.left = size_.left + texParams_.borderLeft/fWidth;
  bottom_.top = size_.top + size_.height - texParams_.borderDown/fHeight;
  bottom_.width = body_.width;
  bottom_.height = bottomLeft_.height;
  bottomTex_.left = bottomLeftTex_.left + bottomLeftTex_.width;
  bottomTex_.top = bottomLeftTex_.top;
  bottomTex_.width = bodyTex_.width;
  bottomTex_.height = bottomLeftTex_.height;

  topRight_.left = top_.left + top_.width;
  topRight_.top = top_.top;
  topRight_.width = texParams_.borderRight/fWidth;
  topRight_.height = top_.height;
  topRightTex_.left = topTex_.left + topTex_.width;
  topRightTex_.top = topTex_.top;
  topRightTex_.width = texParams_.borderRight/fTexWidth;
  topRightTex_.height = topTex_.height;

  right_.left = topRight_.left;
  right_.top = body_.top;
  right_.width = topRight_.width;
  right_.height = body_.height;
  rightTex_.left = topRightTex_.left;
  rightTex_.top = bodyTex_.top;
  rightTex_.width = topRightTex_.width;
  rightTex_.height = bodyTex_.height;

  bottomRight_.left = right_.left;
  bottomRight_.top = bottom_.top;
  bottomRight_.width = right_.width;
  bottomRight_.height = bottom_.height;
  bottomRightTex_.left = rightTex_.left;
  bottomRightTex_.top = bottomTex_.top;
  bottomRightTex_.width = rightTex_.width;
  bottomRightTex_.height = bottomTex_.height;
}

void TexturedWidget::invalidateDimensions()
{
  dimensionsInitialized_ = false;
}

void TexturedWidget::resize( Rect size )
{
  size_ = size;
  invalidateDimensions();
}

