#include "StdAfx.h"
#include "Renderer.h"
#include "Window.h"
#include "Button.h"
#include "Text.h"



Renderer::Renderer():font_(0),currVertIdx_(0),color_(Vector4(255,255,255,255)),activeTex_(-1),width_(1600), height_(900), textSize_(1.0f), guiTexHeight_(-1), guiTexWidth_(-1),
  globalGUIWindow_(NULL), draggedWidget_(NULL), offsetX_(0), offsetY_(0), xAtStartDrag_(0), yAtStartDrag_(0), startMouseX_(0), startMouseY_(0),
  renderingDragged_(false),lastColor_(Vector4(255,255,255,255)),flushes_(0),vertices_(0),tilesTexWidth_(0),tilesTexHeight_(0), floatingWidget_(NULL),
  keyboardListner_(NULL)
{
  globalGUIWindow_ = new GlobalWindow(Rect(0,0,1,1));
  for (char c = 'a'; c <= 'z'; ++c) {
    fontMapX_[c] = (c - 'a') / 104.0f;
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    fontMapX_[c] = (c - 'A' + 26) / 104.0f;
  }
  for (char c = '0'; c <= '9'; ++c) {
    fontMapX_[c] = (c - '0' + 26*2) / 104.0f;
  }
  int i = 61;
  fontMapX_['!'] = (++i)/104.0f;
  fontMapX_['@'] = (++i)/104.0f;
  fontMapX_['#'] = (++i)/104.0f;
  fontMapX_['$'] = (++i)/104.0f;
  fontMapX_['%'] = (++i)/104.0f;
  fontMapX_['^'] = (++i)/104.0f;
  fontMapX_['&'] = (++i)/104.0f;
  fontMapX_['*'] = (++i)/104.0f;
  fontMapX_['('] = (++i)/104.0f;
  fontMapX_[')'] = (++i)/104.0f;
  fontMapX_['-'] = (++i)/104.0f;
  fontMapX_['+'] = (++i)/104.0f;
  fontMapX_['='] = (++i)/104.0f;
  fontMapX_[':'] = (++i)/104.0f;
  fontMapX_['"'] = (++i)/104.0f;
  fontMapX_['\''] = (++i)/104.0f;
  fontMapX_['.'] = (++i)/104.0f;
  fontMapX_[','] = (++i)/104.0f;
  fontMapX_['/'] = (++i)/104.0f;
  fontMapX_['|'] = (++i)/104.0f;
}

Renderer::~Renderer()
{
  delete globalGUIWindow_;
  delete toolTip_;
}

Renderer& Renderer::getInstance()
{
  static Renderer instance;
  return instance;
}

bool Renderer::init()
{
  Logger::getInstance().log(INFO_LOG_NAME, "Starting renderer initialization");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }
  
  Logger::getInstance().log(INFO_LOG_NAME, "SDL successfully initialized");
  SDL_putenv("SDL_VIDEO_WINDOW_POS=20,40");

  SDL_WM_SetCaption("SGame", NULL);

  SDL_Surface* surf = SDL_SetVideoMode(width_, height_, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
  if (!surf) {
    return false;
  }

  Logger::getInstance().log(INFO_LOG_NAME, "Video mode set");

  const char* verstr = (const char*)glGetString( GL_VERSION );
  if (verstr) {
    Logger::getInstance().log(INFO_LOG_NAME, CString("OpenGL version: ") + CString(verstr));
  } else {
    Logger::getInstance().log(INFO_LOG_NAME, CString("Couldn't determine OpenGL version"));
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, width_, height_);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  IMG_Init(IMG_INIT_PNG);
  Logger::getInstance().log(INFO_LOG_NAME, "Loading textures...");
  loadFonts();
  loadTexture("res/gui.png", gui_, guiTexWidth_, guiTexHeight_);
  loadTexture("res/walls.png", tiles_, tilesTexWidth_, tilesTexHeight_);
  loadTexture("res/bg.png", bgTex_);
  loadTexture("res/heads.png", headsTex_, headsTexSize_.x, headsTexSize_.y);
  Logger::getInstance().log(INFO_LOG_NAME, "Finished loading textures.");

  toolTip_ = new TexturedWidget(Rect(0,0,0,0));
  TextureParams params = {164,152,20,20,2,2,2,2};
  toolTip_->init(params);

  return true;
}

void Renderer::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  linesToRender_.clear();
  currVertIdx_ = 0;
  color_ = Vector4(255,255,255,255);

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glAlphaFunc( GL_GREATER, 1.0f );	
  glEnable ( GL_BLEND );
}

void Renderer::render()
{
  //First render GUI frames for each widget. Then run virtual render function for each widget.
  setWidgetForTooltip(NULL);
  globalGUIWindow_->renderFrameRec();
  globalGUIWindow_->renderContRec();
  if (draggedWidget_ || floatingWidget_) {
    renderingDragged_ = true;
    offsetX_ = mouseX_ - startMouseX_;
    offsetY_ = mouseY_ - startMouseY_;
    float lastOpacity = (float)color_[3];
    if (draggedWidget_) {
      color_[3] = 100;
      draggedWidget_->renderFrameRec();
      draggedWidget_->renderContRec();
    } else {
      color_[3] = 200;
      offsetX_ = mouseX_;
      offsetY_ = mouseY_;
      floatingWidget_->renderFrameRec();
      floatingWidget_->renderContRec();
    }
    color_[3] = lastOpacity;
    offsetX_ = 0;
    offsetY_ = 0;
    renderingDragged_ = false;
  }
  if (renderTooltipFor_ && !draggedWidget_ && !floatingWidget_) {
    renderToolTip(renderTooltipFor_->getToolTip());
  }
}

void Renderer::renderEnd()
{
  CString infoStr = "DIPs: " + CString(flushes_) + " Vertices: " + CString(vertices_);
  renderText(0, 1.0f-getCharHeight(), infoStr);
  flushes_ = 0;
  vertices_ = 0;
  renderTextLines();
  flushVerts();
  SDL_GL_SwapBuffers();
}

void Renderer::renderText( float x, float y, CString text )
{
  TextToRender ttr;
  ttr.fx = x + offsetX_;
  ttr.fy = y + offsetY_;
  ttr.text = text;
  ttr.color = color_;
  ttr.size_ = textSize_;
  linesToRender_.push_back(ttr);
}

void Renderer::loadFonts()
{
  loadTexture("res/font2.png", font_);
}

void Renderer::loadTexture(CString fileName, GLuint& tex, int& width, int& height)
{
  SDL_Surface* fontSurf = IMG_Load(fileName);
  if (!fontSurf) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to load " + fileName);
    return;
  }
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  int mode = GL_RGB;
  if (fontSurf->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }
  width = fontSurf->w;
  height = fontSurf->h;
  glTexImage2D(GL_TEXTURE_2D, 0, mode, fontSurf->w, fontSurf->h, 0, mode, GL_UNSIGNED_BYTE, fontSurf->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  Logger::getInstance().log(INFO_LOG_NAME, "Successfully loaded " + fileName);
}

void Renderer::loadTexture( CString fileName, GLuint& tex )
{
  int a, b;
  loadTexture(fileName, tex, a, b);
}

void Renderer::renderTextLines()
{
  Vector4 lastColor = color_;
  for (auto itr = linesToRender_.begin(); itr != linesToRender_.end(); ++itr) {
    TextToRender ttr = *itr;
    renderTextLine(ttr);
  }
  color_ = lastColor;
}

void Renderer::renderTextLine(TextToRender& ttr)
{
  float width = 9 / (float)width_ * ttr.size_;
  float height = 15 / (float)height_ * ttr.size_;
  color_ = ttr.color;
  for (int i=0; i<ttr.text.getSize(); ++i) {
    char c = ttr.text[i];
    if (fontMapX_.count(c) == 0) {
      continue;
    }
    float x = ttr.fx + width*i;
    float y = ttr.fy;
    float tx = fontMapX_[c];
    float ty = 0.0f;
    //char: 9x15
    Rect pos;
    pos.left = x;
    pos.width = width;
    pos.top = y;
    pos.height = height;
    Rect texPos;
    texPos.left = tx;
    texPos.width = 1/104.0f;
    texPos.top = ty;
    texPos.height = 1.0f;
    drawTexRect(pos, font_, texPos);
  }
}

void Renderer::drawTexRect(Rect pos, GLuint texID, Rect texPos)
{
  if (texID != activeTex_) {
    flushVerts();
    glBindTexture(GL_TEXTURE_2D, texID);
    activeTex_ = texID;
  }

  addVertex(pos.left + offsetX_,           1-(pos.top) - offsetY_,              texPos.left,                texPos.top);
  addVertex(pos.left+pos.width + offsetX_, 1-(pos.top) - offsetY_,              texPos.left + texPos.width, texPos.top);
  addVertex(pos.left+pos.width + offsetX_, 1-(pos.top + pos.height) - offsetY_, texPos.left + texPos.width, texPos.top + texPos.height);

  addVertex(pos.left + offsetX_,           1-(pos.top) - offsetY_,              texPos.left,                texPos.top);
  addVertex(pos.left+pos.width + offsetX_, 1-(pos.top + pos.height) - offsetY_, texPos.left + texPos.width, texPos.top + texPos.height);
  addVertex(pos.left + offsetX_,           1-(pos.top + pos.height) - offsetY_, texPos.left,                texPos.top + texPos.height);
}

void Renderer::addVertex( float x, float y, float u, float v )
{
  ++vertices_;
  verts_[currVertIdx_].x = x;
  verts_[currVertIdx_].y = y;
  verts_[currVertIdx_].z = 0;
  verts_[currVertIdx_].u = u;
  verts_[currVertIdx_].v = v;
  verts_[currVertIdx_].r = (GLubyte)color_[0];
  verts_[currVertIdx_].g = (GLubyte)color_[1];
  verts_[currVertIdx_].b = (GLubyte)color_[2];
  verts_[currVertIdx_].a = (GLubyte)color_[3];
  ++currVertIdx_;
  if (currVertIdx_ >= MAX_VERTS-3) {
    flushVerts();
  }
}

void Renderer::flushVerts()
{
  if (currVertIdx_ == 0) {
    return;
  }
  ++flushes_;
  glVertexPointer(3, GL_FLOAT,  sizeof(Vertex), (void*) &verts_[0].x);
  glEnableClientState( GL_VERTEX_ARRAY );
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)&verts_[0].r);
  glEnableClientState( GL_COLOR_ARRAY );
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*) &verts_[0].u);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, (GLsizei) currVertIdx_);

  currVertIdx_ = 0;
  glFlush();
}

void Renderer::initWidgetDimensions( TexturedWidget* widget )
{
  widget->initDimensions(width_, height_, guiTexWidth_, guiTexHeight_);
}

void Renderer::handleGUIEvent( SDL_Event& event )
{
  int x = event.motion.x;
  int y = event.motion.y;
  float fx = x / (float)width_;
  float fy = y / (float)height_;
  if (event.type == SDL_MOUSEMOTION) {
    mouseX_ = fx;
    mouseY_ = fy;
  }
  globalGUIWindow_->handleEvent(event, fx, fy);
}

void Renderer::handleKeyboardEvent( SDL_Event& event )
{
  if (!keyboardListner_) {
    return;
  }
  switch (event.type)
  {
  case SDL_KEYDOWN:
    keyboardListner_->keyDown(event.key.keysym);
    break;
  default:
    break;
  }
}

void Renderer::setDraggedWidget( Widget* w )
{
  if (!w) {
    offsetX_ = 0;
    offsetY_ = 0;
  } else {
    startMouseX_ = mouseX_;
    startMouseY_ = mouseY_;
  }
  draggedWidget_ = w;
}

void Renderer::setFloatingWidget( Widget* w )
{
  floatingWidget_ = w;
}

float Renderer::getCharWidth()
{
  return textSize_ * 9.0f / (float)width_;
}

float Renderer::getCharHeight()
{
  return textSize_ * 15.0f / (float)height_;
}

void Renderer::addWidget( Widget* window )
{
  globalGUIWindow_->addWidget(window);
}

void Renderer::setColor( Vector4 color )
{
  lastColor_ = color_;
  color_ = color;
}

void Renderer::setColor( Vector3 color )
{
  lastColor_ = color_;
  color_[0] = color[0];
  color_[1] = color[1];
  color_[2] = color[2];
}

void Renderer::resetColor()
{
  color_ = lastColor_;
}

void Renderer::clearWindows()
{
  globalGUIWindow_->clear();
}

void Renderer::renderToolTip(CString text)
{
  setTextSize(1.0f);
  float x = getMouseX();
  float y = getMouseY();
  float width = getCharWidth() * text.getSize() * 1.02f;
  float height = getCharHeight() * 1.02f;
  y -= height;
  toolTip_->resize(Rect(x, y, width, height));
  toolTip_->render();
  setColor(Vector3(0,0,0));
  renderText(x + width * 0.01f, y + height * 0.01f, text);
  resetColor();
}

void Renderer::resize(int width, int height)
{
  //float dx = width / (float)width_;
  //float dy = height / (float)height_;
  //width_ = width;
  //height_ = height;
  //Rect size = globalGUIWindow_->getSize();
  //size.width *= dx;
  //size.height *= dy;
  //size.top *= dy;
  //size.left *= dx;
  //globalGUIWindow_->resize(size);
  //glViewport(0, 0, width_, height_);
}

void Renderer::setKeyboardListener( Widget* w )
{
  if (keyboardListner_) {
    keyboardListner_->onKeyboardFocusLose();
  }
  if (w) {
    w->onKeyboardFocusGain();
  }
  keyboardListner_ = w;
}
