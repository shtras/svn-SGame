#pragma once
#include "GameWorld.h"

class SGame: public has_slots<>
{
public:
  enum GameState {Menu, Editor, Game, Quit};
  static SGame& getInstance();
  bool init();
  bool run();
  void toggleEditor();
  void toggleMenu();
  void toggleGame();
private:
  bool mainLoop();
  bool initMenu();
  bool finishMenu();
  bool initEditor();
  bool finishEditor();
  bool initGame();
  bool finishGame();
  void handleEvent(SDL_Event& event);
  void quit();
  void close();
  SGame();
  ~SGame();
  GameState state_;
  bool stateRunnig_;
  GameWorld* world_;
};
