#include "StdAfx.h"
#include "SGame.h"
#include "Renderer.h"
#include "Button.h"
#include "VersionInfo.h"
#include "RoomParser.h"
#include "ItemsDB.h"
#include "ShipEditor.h"
#include "FileOpenDialog.h"
#include "GameWindow.h"
#include "Credits.h"
#include "Timer.h"
#include "SoundManager.h"

const char* Version = "0.0.6.";
//TODO for next major version:
//1. Complete editor
//TODO for next minor version:
//0.0.4:
//1.V Compartments by category display at tools panel
//2.V Compartments and items building / display at ShipView
//3.V Support of multiple decks and compartments by categories in the build tools
//4.V Implement scrollbar in tools menu
//0.0.5:
//1.V Multiple layers in ship view
//2.V Compartment purposes and additional parameters
//3.V Links between compartments
//3.1 Show required links in hover info
//4.V Info panel with ship parameters
//5.V Selection of which deck to show as ghost (upper or lower)
//0.0.6
//1. Oxygen system (compressors, pipes, dispensers)
//1.1 Oxygen overview
//2. Power system (cables, producers, consumers)
//2.1 Power overview
//***Shifted to 0.0.2 at build #349
//Implemented GUI, basic editor
//Floor, walls building, erase
//***Shifted to 0.0.3 at build #432
//Doors building
//Items database
//Parsing of rooms.txt - compartments and items description file
//***Progress:
//Compartments creation and file parsing
//Scroll in the compartments menu
//Compartments sorted by categories
//Multiple decks support. Upper deck shows ghost image of the lower one in the empty space
//Redesigned GUI. Implemented progress bar
//***Shifted to 0.0.4 at build #614
//Implemented ship statistics and compartment parameters
//Added floor rendering at wall tiles
//***Demo released on build #718
//Implemented compartment rotation
//Implemented ghost deck selection
//Implemented accessibility and connectivity check for tiles
//Implemented multiple layers (construction and accessibility)
//Implemented main entrance to the ship (only one possible)
//Added entrance validation
//Added connections and additional info on hover windows
//***Shifted to 0.0.5 at build #905
//Implemented stairs
//Implemented save/load
//Added Input widget
//***Shifted to 0.0.6 at build #1510

SGame::SGame():state_(Menu),stateRunnig_(false),world_(NULL),paused_(true)
{

}

SGame::~SGame()
{

}

SGame& SGame::getInstance()
{
  static SGame instance;
  return instance;
}

bool SGame::init()
{
  bool res = Renderer::getInstance().init();
  if (!res) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to initialize renderer");
    return false;
  }
  res = ItemsDB::getInstance().init();
  if (!res) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to initialize items DB");
    return false;
  }
  res = SoundManager::getInstance().init();
  if (!res) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to sound manager");
    return false;
  }
  return true;
}

bool SGame::run()
{
  while (state_ != Quit) {
    switch (state_)
    {
    case Menu:
      Logger::getInstance().log(INFO_LOG_NAME, "Switching to menu");
      initMenu();
      mainLoop();
      finishMenu();
      break;
    case Editor:
      Logger::getInstance().log(INFO_LOG_NAME, "Switching to editor");
      initEditor();
      mainLoop();
      finishEditor();
      break;
    case Game:
      Logger::getInstance().log(INFO_LOG_NAME, "Switching to game");
      initGame();
      mainLoop();
      finishGame();
      break;
    case ShowCredits:
      Logger::getInstance().log(INFO_LOG_NAME, "Switching to credits");
      initCredits();
      mainLoop();
      finishCredits();
      break;
    default:
      Logger::getInstance().log(ERROR_LOG_NAME, "Encountered unknown state. Terminating.");
      state_ = Quit;
      break;
    }
  }
  close();
  return true;
}

bool SGame::mainLoop()
{
  //typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
  //PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
  //wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

  //if( wglSwapIntervalEXT ) {
  //  wglSwapIntervalEXT(0);
  //} 
  int fpsTimeBase = SDL_GetTicks();
  int currTime = fpsTimeBase;
  int lastTime = currTime;
  int accumulator = 0;
  int dt = 10;
  int accumulatorEvents = 0;
  int dtEvents = 6;
  int frames = 0;
  float fps = 0;
  CString version = Version + CString(BUILD_NUM) + " " + CString(BUILD_STR);
  while(stateRunnig_) {
    if (accumulatorEvents > dtEvents) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        handleEvent(event);
      }
      accumulatorEvents -= dtEvents;
    }
    currTime = SDL_GetTicks();
    int delta = currTime - lastTime;
    accumulatorEvents += delta;
    if (!paused_) {
      accumulator += delta;
      while (accumulator >= dt) {
        accumulator -= dt;
        if (state_ == Game && world_) {
          world_->step();
          Timer::getTime().progress(1);
        }
      }
    } else {
      Timer::getTime().progress(0);
    }
    lastTime = currTime;
    ++frames;
    if (currTime - fpsTimeBase > 100) {
      fps = frames / (float)(currTime - fpsTimeBase) * 1000.0f;
      fpsTimeBase = currTime;
      frames = 0;
    }
    Renderer::getInstance().clear();
    Renderer::getInstance().render();
    Renderer::getInstance().setTextSize(1);
    Renderer::getInstance().setColor(Vector4(255, 255, 255, 255));
    CString fpsString = "FPS: " + CString(fps, 2);
    Renderer::getInstance().renderText(0.0f, 0.0f, fpsString);
    Renderer::getInstance().renderText(1.0f - version.getSize()*Renderer::getInstance().getCharWidth(), 1.0f - Renderer::getInstance().getCharHeight(), version);

    Renderer::getInstance().renderEnd();
    checkReleaseError("OpenGL error during main loop. Something bad happened :(");
    SoundManager::getInstance().update();
    Sleep(0);
  }
  return true;
}

bool SGame::initMenu()
{
  Renderer& renderer = Renderer::getInstance();
  stateRunnig_ = true;
  Window* menuWindow = new Window(Rect(0.3, 0.1, 0.4, 0.8));
  renderer.addWidget(menuWindow);
  double top = 0.1;
  double height = 0.15;
  Button* editorButton = new Button(Rect(0.1, top, 0.8, height));
  editorButton->setCaption("Ship editor");
  editorButton->setColor(Vector4(0, 0, 255, 255));
  editorButton->setTextSize(3);
  menuWindow->addWidget(editorButton);
  editorButton->sigClick.connect(this, &SGame::toggleEditor);
  top += height + 0.05;

  Button* newGameButton = new Button(Rect(0.1, top, 0.8, height));
  newGameButton->setCaption("New game");
  newGameButton->setColor(Vector4(0, 0, 255, 255));
  newGameButton->setTextSize(3);
  menuWindow->addWidget(newGameButton);
  newGameButton->sigClick.connect(this, &SGame::toggleGame);
  top += height + 0.05;

  Button* creditsButton = new Button(Rect(0.1, top, 0.8, height));
  creditsButton->setCaption("Credits");
  creditsButton->setColor(Vector4(0, 0, 255, 255));
  creditsButton->setTextSize(3);
  menuWindow->addWidget(creditsButton);
  creditsButton->sigClick.connect(this, &SGame::toggleCredits);
  top += height + 0.05;

  Button* quitButton = new Button(Rect(0.1, top, 0.8, height));
  quitButton->setCaption("Quit");
  quitButton->setColor(Vector4(0, 0, 255, 255));
  quitButton->setTextSize(3);
  menuWindow->addWidget(quitButton);
  quitButton->sigClick.connect(this, &SGame::quit);
  top += height + 0.05;

  return true;
}

bool SGame::initEditor()
{
  ShipEditor* editor = new ShipEditor(Rect(0,0,1,1));
  Renderer::getInstance().addWidget(editor);
  stateRunnig_ = true;
  return true;
}

bool SGame::finishMenu()
{
  Renderer::getInstance().clearWindows();
  return true;
}

bool SGame::initGame()
{
  paused_ = true;
  stateRunnig_ = true;
  assert(!world_);
  world_ = new GameWorld();
  bool res = world_->init();
  if (!res) {
    state_ = Quit;
    stateRunnig_ = false;
    Logger::getInstance().log(ERROR_LOG_NAME, "Could not init game world");
    return false;
  }

  GameWindow* gameWindow = new GameWindow(Rect(0,0,1,1));
  Renderer::getInstance().addWidget(gameWindow);
  res = gameWindow->init(world_);
  
  if (!res) {
    state_ = Quit;
    stateRunnig_ = false;
    Logger::getInstance().log(ERROR_LOG_NAME, "Could not init game window");
    return false;
  }

  return true;
}

bool SGame::finishGame()
{
  delete world_;
  world_ = NULL;
  Renderer::getInstance().clearWindows();
  return true;
}

void SGame::handleEvent( SDL_Event& event )
{
  switch (event.type)
  {
  case SDL_QUIT:
    quit();
    break;
  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEMOTION:
    Renderer::getInstance().handleGUIEvent(event);
    break;
  case SDL_KEYDOWN:
    Renderer::getInstance().handleKeyboardEvent(event);
    if (event.key.keysym.sym == SDLK_SPACE) {
      togglePause();
    }
    break;
  case SDL_VIDEORESIZE:
    Renderer::getInstance().resize(event.resize.w, event.resize.h);
    break;
  default:
    break;
  }
}

void SGame::close()
{
  SDL_Quit();
}

void SGame::quit()
{
  state_ = Quit;
  stateRunnig_ = false;
}

void SGame::toggleEditor()
{
  state_ = Editor;
  stateRunnig_ = false;
}

void SGame::toggleMenu()
{
  state_ = Menu;
  stateRunnig_ = false;
}

void SGame::toggleGame()
{
  state_ = Game;
  stateRunnig_ = false;
}

bool SGame::finishEditor()
{
  Renderer::getInstance().clearWindows();
  return true;
}

void SGame::toggleCredits()
{
  state_ = ShowCredits;
  stateRunnig_ = false;
}

bool SGame::initCredits()
{
  stateRunnig_ = true;
  Credits* credits = new Credits(Rect(0,0,1,1));
  Renderer::getInstance().addWidget(credits);
  return true;
}

bool SGame::finishCredits()
{
  Renderer::getInstance().clearWindows();
  return true;
}

void SGame::togglePause()
{
  paused_ = !paused_;
}

#ifdef WIN32
int __stdcall WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
#else
int main(int argc, char** argv)
#endif
{
  Logger::getInstance().log(INFO_LOG_NAME, "Application started. Hello, World!");
  Logger::getInstance().log(INFO_LOG_NAME, CString("Application version: ") + Version + CString(BUILD_NUM) + " " + CString(BUILD_STR));
#if defined(WIN32) && defined(DEBUG)
  AllocConsole();
  FILE* stream = NULL;
  errno_t err = freopen_s(&stream, "CON", "w", stdout);
#endif
  SGame& game = SGame::getInstance();
  bool res = game.init();
  if (res) {
    Logger::getInstance().log(INFO_LOG_NAME, "Initialization successfully completed");
    game.run();
  } else {
    Logger::getInstance().log(ERROR_LOG_NAME, "Something went horribly wrong. Look above for details.");
  }

#if defined(WIN32) && defined(DEBUG)
  FreeConsole();
#endif
  Logger::getInstance().log(INFO_LOG_NAME, "Application ended. Bye!");
}
