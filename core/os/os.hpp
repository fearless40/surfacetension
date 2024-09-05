#pragma once

extern void game_main();

namespace os {
namespace console {
bool connect();
void disconnect();
} // namespace console

bool create_main_window();
void run_event_loop();
} // namespace os