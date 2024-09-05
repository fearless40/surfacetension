#pragma once
#include "TimeStep.hpp"
#include "Window.h"



/*namespace Graphics::D3D11 {
	class Driver;
}*/
namespace os::windows {
class WindowsGame : Window {
public:
  WindowsGame(HINSTANCE hInstance);
  ~WindowsGame();

  void run();
  void shutdown();

private:
  void next_frame();
  void next_game_step(float since_last_frame);
  void next_render_step(float since_last_frame);

  void onKey(WORD vkCode, bool isKeyUp, int repeatCount) override;
  void onSize(RectInt newSize) override;

  HINSTANCE mInst;
  HACCEL mAccelTable;
  util::TimeStep timer;
  bool mRunning;
  //	Graphics::D3D11::Driver * graphics_driver{ nullptr };
};
} // namespace os