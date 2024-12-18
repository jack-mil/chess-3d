#include <iostream>

#include "engine.hpp"
#include "app.hpp"

#include "chess.hpp"
#include "UCI.hpp"

using namespace chess3d;

static uciadapter::Go settings;

Engine::Engine(App* app) :  m_app(app), m_board(new chess::Board(chess::constants::STARTPOS)) {
  settings.depth = 10;
}

Engine::~Engine() {
  quitEngine();
}

/** Return true if quit a running engine, else false if already quit */
bool Engine::quitEngine() {
  if (m_engine) {
    m_engine->quit();  // send the uci quit command to stop stockfish
    m_engine.reset();  // delete object
    std::cout << "Killed Stockfish Engine\n";
    return true;
  }
  std::cout << "Stockfish is not running\n";
  return false;
}

bool Engine::initializeEngine() {
  quitEngine(); // quit/kill if it is already running
  try {
    m_engine = std::make_unique<uciadapter::UCI>("stockfish");
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  std::cout << "Started Engine\n";
  std::cout << m_engine->GetName() << '\n';
  // std::cout << m_engine->GetInfoStrings() << '\n';

  m_engine->ucinewgame();
  std::cout << "Begin new game\n";
  std::cout << "Waiting player move...\n";

  return true;
}

bool Engine::makePlayerMove(const std::string& move) {
  chess::Color side = m_board->sideToMove();
  // Player must play as white
  if (side != chess::Color::WHITE) {
    std::cout << "It is not white's turn!\n";
    return false;
  }

  auto validMove = chess::uci::uciToMove(*m_board, move);
  if (validMove == chess::Move::NO_MOVE) {
    std::cout << "'" + move + "' is invalid! Try again\n";
    return false;
  }
  m_board->makeMove(validMove);
  std::cout << "Player moved: " << validMove << '\n';
  std::cout << m_board->getFen() << '\n';
  m_app->movePiece(validMove.from(), validMove.to());
  return true;
}

bool Engine::getEngineMove(std::string& moveOut) {
  chess::Color side = m_board->sideToMove();
  // AI must play as black
  if (side != chess::Color::BLACK) {
    std::cout << "It is not AI's turn!\n";
    return false;
  }

  m_engine->position(m_board->getFen());
  m_engine->go(settings);
  // give engine 500 milliseconds to think
  m_engine->SyncAfter(500);
  auto uciMove = m_engine->GetBestMove();
  auto validMove = chess::uci::uciToMove(*m_board, uciMove);
  if (validMove == chess::Move::NO_MOVE) {
    std::cout << "Engine produced an illegal move (!?) (" + uciMove + ")\n";
    return false;
  }
  // make move on our internal board
  m_board->makeMove(validMove);
  std::cout << "Computer moved: " << validMove << '\n';
  std::cout << m_board->getFen() << '\n';
  m_app->movePiece(validMove.from(), validMove.to());
  // return string from engine
  moveOut = uciMove;
  return true;
}

std::string Engine::getBoardString() const {
  return m_board->getFen();
}

bool Engine::isRunning() const {
  return (m_engine != nullptr);
}
