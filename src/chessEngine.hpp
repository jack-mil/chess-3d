#pragma once

#include <string>

#include "chess.hpp"
#include "UCI.hpp"

class ChessEngine {
public:
  ChessEngine();
  ~ChessEngine();
  bool initializeEngine();
  bool makePlayerMove(const std::string& move);
  bool makeEngineMove(std::string& moveOut);
  std::string getBoardString() const;

private:
  uciadapter::Go m_settings;
  chess::Board m_board;

  uciadapter::UCI* m_engine = nullptr;
};
