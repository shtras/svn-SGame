#pragma once

class SGame: public has_slots<>
{
public:
  enum GameState {Menu, Editor, Game, Quit};
  static SGame& getInstance();
  bool init();
  bool run();
private:
  bool mainLoop();
  bool initMenu();
  bool finishMenu();
  bool initEditor();
  bool finishEditor();
  void handleEvent(SDL_Event& event);
  void quit();
  void toggleEditor();
  void toggleMenu();
  void close();
  SGame();
  ~SGame();
  GameState state_;
  bool stateRunnig_;
};
