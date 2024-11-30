#pragma once

#include "imgui.h"

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
    Console();
    ~Console();
    void draw();
    void AddLog(const char* move);

    char InputBuf[256];
    ImVector<char*> Items;
    int LogLimit = 1000;
  };
  void drawHeader();
  void drawConsole();

  bool m_open = true;
  bool m_demo = false;
  Console* m_console = nullptr;
};
} // namespace chess3d
