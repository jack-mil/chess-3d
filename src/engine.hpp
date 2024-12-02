#pragma once

#include <string>

#include "chess.hpp"

#include "UCI.hpp"

namespace chess3d {

class Engine {
public:
  Engine();
  ~Engine();
  bool initializeEngine();
  bool quitEngine();
  bool makePlayerMove(const std::string& move);
  bool getEngineMove(std::string& moveOut);
  std::string getBoardString() const;
  bool isRunning() const;

private:
  uciadapter::Go m_settings;
  chess::Board m_board;

  uciadapter::UCI* m_engine = nullptr;
};

} // namespace chess3d
