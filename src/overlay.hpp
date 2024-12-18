#pragma once

#include <imgui.h> // included with Ogre

namespace chess3d {

class Engine; // forward declaration for making moves

class Overlay {
public:
  Overlay(Engine* engine);

  void draw();
  void toggleShow();
  void toggleDebug();

private:
  struct Console {

    Console(Overlay* parent);
    ~Console();
    void draw();
    void makePlayerMove(const char* move);
    void makeEngineMove();

    Overlay* m_parent;
    char InputBuf[25];
    ImVector<char*> Items;
    int LogLimit = 1000;
  };

  void drawHeader();
  void drawButtons();
  void drawConsole();

  bool m_open;
  bool m_demo;
  bool m_running;
  /** Chess game state and stockfish connection */
  Engine* m_engine;
  /** Text feed of player and engine moves */
  Console m_console;
};
} // namespace chess3d
