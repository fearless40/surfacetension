#pragma once

extern void game_main();

namespace os {
namespace console {
bool connect();
void disconnect();
} // namespace console

struct NextLogicFrame
{
  float step;
};

struct NextRenderFrame
{
  float step;
};


bool create_main_window();
void run_event_loop();
} // namespace os