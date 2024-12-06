#pragma once
#include <memory>

#include <string>

// forward declarations
namespace chess{ class Board; };

namespace uciadapter{ class UCI; };

namespace chess3d {

class App; // forward declaration for movePiece callback

class Engine {
public:
  Engine(App* app);
  ~Engine();
  bool initializeEngine();
  bool quitEngine();
  bool makePlayerMove(const std::string& move);
  bool getEngineMove(std::string& moveOut);
  std::string getBoardString() const;
  bool isRunning() const;

private:
  App* m_app;
  std::unique_ptr<chess::Board> m_board;
  std::unique_ptr<uciadapter::UCI> m_engine;
};

} // namespace chess3d
