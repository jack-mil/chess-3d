#include <fmt/base.h>
#include <fmt/color.h>

#include "chessEngine.hpp"
#include "ChessApp.hpp"

template <typename ... T>
static void log_err(std::string msg, T&&... args)
{
  fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::red), "{}\n", fmt::format(msg, args...));
}
// static void log_info(std::string msg)
// {
//   fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::bright_blue), "{}\n", msg);
// }

// int main(/* int argc, char const *argv[] */)
// {
//   // ChessEngine engine;
//   // auto success = engine.initializeEngine();
//   // if (!success) {
//   //   return 1;
//   // }
//   // fmt::println(engine.getBoardString());
//   // if (!engine.makePlayerMove("e2e4"))
//   // {
//   //   log_err("Send move failed.");
//   // }
//   // fmt::println(engine.getBoardString());
//   // std::string aiMove;
//   // engine.makeEngineMove(aiMove);
//   // log_info(aiMove);
//   // fmt::println(engine.getBoardString());
  
//   return 0;

// }
int main(/* int argc, char **argv */)
{
  try {
    ChessApplication app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
  }
  catch (const std::exception& e) {
    log_err("Error occurred during execution: {}", e.what());
    return 1;
  }

  return 0;
}
