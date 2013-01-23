#pragma once
#include "TexturedWidget.h"
#include "GlobalWindow.h"
#include "Window.h"

struct TextToRender
{
  float fx;
  float fy;
  float size_;
  CString text;
  Vector4 color;
};

struct Vertex
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat u;
  GLfloat v;
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
};

#define MAX_VERTS (4096*3)

class Renderer
{
public:
  static Renderer& getInstance();
  bool init();
  void clear();
  void render();
  void renderEnd();
  void renderText(float x, float y, CString text);
  void drawTexRect(Rect pos, GLuint texID, Rect texPos, int rotation = 0);
  void setTextSize(float size) {textSize_ = size;}
  void setColor(Vector4 color);
  void setColor(Vector3 color);
  void resetColor();
  GLuint getGUITex() {return gui_;}
  GLuint getTilesTex() {return tiles_;}
  GLuint getBGTex() {return bgTex_;}
  GLuint getHeadsTex() {return headsTex_;}
  DCoord getHeadsTexSize() {return headsTexSize_;}
  void initWidgetDimensions(TexturedWidget* widget);
  void handleGUIEvent(SDL_Event& event);
  void handleKeyboardEvent(SDL_Event& event);
  void setDraggedWidget(Widget* w);
  Widget* getDraggedWidget() {return draggedWidget_;}
  void setFloatingWidget(Widget* w);
  Widget* getFloatingWidget() {return floatingWidget_;}
  void setKeyboardListener(Widget* w);
  bool renderingDragged() {return renderingDragged_;}
  float getOffsetX() {return startMouseX_ - mouseX_;}
  float getOffsetY() {return startMouseY_ - mouseY_;}
  float getMouseX() {return mouseX_;}
  float getMouseY() {return mouseY_;}
  int getWidth() {return width_;}
  int getHeight() {return height_;}
  float getCharWidth();
  float getCharHeight();
  void addWidget(Widget* window);
  void clearWindows();
  int getTilesTexWidth() {return tilesTexWidth_;}
  int getTilesTexHeight() {return tilesTexHeight_;}
  void setWidgetForTooltip(Widget* w) {renderTooltipFor_ = w;}
  void resize(int width, int height);
  void setFloatingWidgetWidthLimit(float value) {floatingWidgetWidthLimit_ = value;}
private:
  Renderer();
  ~Renderer();
  void renderToolTip(CString text);
  void loadFonts();
  void loadTexture(CString fileName, GLuint& tex);
  void loadTexture(CString fileName, GLuint& tex, int& width, int& height);
  void renderTextLines();
  void renderTextLine(TextToRender& ttr);
  void addVertex(float x, float y, float u, float v);
  void flushVerts();
  GLuint font_;
  GLuint gui_;
  GLuint tiles_;
  GLuint bgTex_;
  GLuint headsTex_;
  DCoord headsTexSize_;
  Vector4 color_;
  Vector4 lastColor_;
  Vertex verts_[MAX_VERTS];
  int currVertIdx_;
  GLuint activeTex_;
  int width_;
  int height_;
  float textSize_;
  int guiTexWidth_;
  int guiTexHeight_;
  int tilesTexHeight_;
  int tilesTexWidth_;

  list<TextToRender> linesToRender_;
  GlobalWindow* globalGUIWindow_;
  //Render at this offset
  float offsetX_;
  float offsetY_;
  //For drag-drop render
  Widget* draggedWidget_;
  float xAtStartDrag_;
  float yAtStartDrag_;
  float mouseX_;
  float mouseY_;
  float startMouseX_;
  float startMouseY_;
  bool renderingDragged_;
  int flushes_;
  int vertices_;
  Widget* renderTooltipFor_;
  TexturedWidget* toolTip_;
  Widget* floatingWidget_;
  Widget* keyboardListner_;
  map<char, float> fontMapX_;
  float floatingWidgetWidthLimit_;
  int charWidth_;
  int charHeight_;
};