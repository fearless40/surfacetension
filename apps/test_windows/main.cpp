
#include <iostream>
//#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include "os.hpp"

void game_main() {

  os::create_main_window();
  os::console::connect();
  auto logger = spdlog::stdout_color_st("consolelogger");

  logger->info("Hello from spdlog");
  
  std::cout << "Hello from a gui program" << '\n';

  
  os::run_event_loop();
}