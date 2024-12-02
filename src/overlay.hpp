#pragma once

#include <imgui.h> // included with Ogre

#include "engine.hpp"

namespace chess3d {

class Overlay {
public:
  Overlay();
  ~Overlay();

  void draw();
  void toggleShow();
  void toggleDebug();

private:
  struct Console {
    Console(Overlay* parent);
    ~Console();
    void draw();
    void makeMove(const char* move);

    Overlay* parent;
    char InputBuf[25];
    ImVector<char*> Items;
    int LogLimit = 1000;
  };
  void drawHeader();
  void drawButtons();
  void drawConsole();

  bool m_open = true;
  bool m_demo = false;
  bool m_running = false;
  /** Text feed of player and engine moves */
  Console* m_console = nullptr;
  /** Chess game state and stockfish connection */
  Engine m_engine;
};
} // namespace chess3d
