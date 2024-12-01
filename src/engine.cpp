#include <iostream>

#include "engine.hpp"

using namespace chess3d;

Engine::Engine() : m_board(chess::constants::STARTPOS) {
  m_settings.depth = 10;
  m_engine = nullptr;
}

Engine::~Engine() {
  quitEngine();
}

bool Engine::quitEngine() {
  if (m_engine != nullptr) {
    m_engine->quit();
    delete m_engine;
    m_engine = nullptr;
    return true;
  }
  return false;
}

bool Engine::initializeEngine() {
  quitEngine(); // quit/kill if it is already running
  try {
    m_engine = new uciadapter::UCI("stockfish");
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }

  std::cout << m_engine->GetName() << '\n';
  std::cout << m_engine->GetAuthor() << '\n';

  m_engine->ucinewgame();

  return true;
}

bool Engine::makePlayerMove(const std::string& move) {
  auto validMove = chess::uci::uciToMove(m_board, move);
  if (validMove == chess::Move::NO_MOVE) {
    return false;
  }
  m_board.makeMove(validMove);
  return true;
}

bool Engine::makeEngineMove(std::string& moveOut) {
  chess::Color side = m_board.sideToMove();
  // AI must play as black
  if (side != chess::Color::BLACK) {
    return false;
  }

  m_engine->position(m_board.getFen());
  m_engine->go(m_settings);
  // give engine 500 milliseconds to think
  m_engine->SyncAfter(500);
  auto uciMove = m_engine->GetBestMove();
  auto validMove = chess::uci::uciToMove(m_board, uciMove);
  if (validMove == chess::Move::NO_MOVE) {
    return false;
  }
  // make move on our internal board
  m_board.makeMove(validMove);
  // return string from engine
  moveOut = uciMove;
  return true;
}

std::string Engine::getBoardString() const {
  return m_board.getFen();
}

bool Engine::isRunning() const {
  return (m_engine == nullptr);
}
