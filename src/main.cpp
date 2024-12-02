#include <fmt/base.h>
#include <fmt/color.h>

#include "app.hpp"

template <typename... T>
static void log_err(std::string msg, T&&... args) {
  fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::red), "{}\n", fmt::format(msg, args...));
}
// static void log_info(std::string msg)
// {
//   fmt::print(stderr, fmt::emphasis::bold | fg(fmt::terminal_color::bright_blue), "{}\n", msg);
// }

int main(/* int argc, char **argv */) {
  try {
    chess3d::App app;
    app.exec();
  } catch (const std::exception& e) {
    log_err("Error occurred during execution: {}", e.what());
    return 1;
  }

  return 0;
}
