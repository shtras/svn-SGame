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
  void drawTexRect(Rect pos, GLuint texID, Rect texPos);
  void setTextSize(float size) {textSize_ = size;}
  void setColor(Vector4 color);
  void setColor(Vector3 color);
  void resetColor();
  GLuint getGUITex() {return gui_;}
  GLuint getTilesTex() {return tiles_;}
  GLuint getBGTex() {return bgTex_;}
  void initWidgetDimensions(TexturedWidget* widget);
  void handleGUIEvent(SDL_Event& event);
  void setDraggedWidget(Widget* w);
  Widget* getDraggedWidget() {return draggedWidget_;}
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
private:
  Renderer();
  ~Renderer();

  void loadFonts();
  void loadTexture(CString fileName, GLuint& tex);
  void loadTexture(CString fileName, GLuint& tex, int& width, int& height);
  void renderTextLines();
  void renderTextLine(TextToRender& ttr);
  void addVertex(float x, float y, float u, float v);
public:
  void flushVerts();
  GLuint font_;
  GLuint gui_;
  GLuint tiles_;
  GLuint bgTex_;
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

  map<char, int> fontMap_;

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
};