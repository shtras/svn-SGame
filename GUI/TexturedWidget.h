#pragma once

struct TextureParams
{
  int textureLeft;
  int textureTop;
  int textureWidth;
  int textureHeight;
  int borderLeft;
  int borderTop;
  int borderRight;
  int borderDown;
};

class TexturedWidget
{
public:
  TexturedWidget(Rect size);
  ~TexturedWidget();
  void render();
  void initDimensions(int width, int height, int texWidth, int texHeight);
  void invalidateDimensions();
  void init(TextureParams params);
  void resize(Rect size);
private:
  TexturedWidget();
  Rect size_;
  TextureParams texParams_;
  Rect topLeft_;
  Rect topLeftTex_;
  Rect left_;
  Rect leftTex_;
  Rect bottomLeft_;
  Rect bottomLeftTex_;
  Rect top_;
  Rect topTex_;
  Rect bottom_;
  Rect bottomTex_;
  Rect topRight_;
  Rect topRightTex_;
  Rect right_;
  Rect rightTex_;
  Rect bottomRight_;
  Rect bottomRightTex_;
  Rect body_;
  Rect bodyTex_;
  bool dimensionsInitialized_;
};


class Widget
{
public:
  Widget(Rect size);
  virtual ~Widget();
  void renderFrameRec();
  void renderContRec();
  virtual void render() {}
  void initRegular(TextureParams params);
  void initHovered(TextureParams params);
  void initPressed(TextureParams params);
  //Base events
  void lmDown();
  void rmDown();
  void lmUp();
  void rmUp();
  void click();
  void hoverEnter();
  void hoverExit();
  void drag();
  void drop(Widget* w);
  void mouseMove();
  void mouseWheelScroll(int direction);
  //User events
  virtual void onLMDown() {}
  virtual void onRMDown() {}
  virtual void onLMUp() {}
  virtual void onRMUp() {}
  virtual void onClick() {}
  virtual void onHoverEnter() {}
  virtual void onHoverExit() {}
  virtual void onDrag() {}
  virtual void onDrop(Widget* w) {}
  virtual void onMouseMove() {}
  virtual void onMouseWheelScroll(int direction) {}
  //Child care
  void addWidget(Widget* widget);
  void removeWidget(Widget* widget);
  void clear();
  bool handleEvent(SDL_Event& event, float fx, float fy);
  void resize(Rect size);
  Rect getSize() {return size_;}
  void setHighlighted(bool value);
  bool isHighlighted() {return highlighted_;}
protected:
  Rect size_;
  bool clickable_;
  bool draggable_;
  list<Widget*> children_;
  TexturedWidget* regularTexture_;
  TexturedWidget* hoveredTexture_;
  TexturedWidget* pressedTexture_;
  TexturedWidget* activeTexture_;
private:
  Widget();
  bool hovered_;
  bool pressed_;
  bool dragged_;
  bool highlighted_;
};
