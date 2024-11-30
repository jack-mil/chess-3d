#include <fmt/base.h>
#include <fmt/color.h>

#include "chessEngine.hpp"

static void log_err(std::string msg)
{
  fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::red), "{}\n", msg);
}
static void log_info(std::string msg)
{
  fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::bright_blue), "{}\n", msg);
}

int main(/* int argc, char const *argv[] */)
{
  ChessEngine engine;
  auto success = engine.initializeEngine();
  if (!success) {
    return 1;
  }
  fmt::println(engine.getBoardString());
  if (!engine.makePlayerMove("e2e4"))
  {
    log_err("Send move failed.");
  }
  fmt::println(engine.getBoardString());
  std::string aiMove;
  engine.makeEngineMove(aiMove);
  log_info(aiMove);
  fmt::println(engine.getBoardString());
  
  return 0;
}
