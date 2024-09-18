#pragma once
#include "TimeStep.hpp"
#include "Window.h"
#include "dx12driver.hpp"


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
  
  D3D12Driver m_driver;
};
} // namespace os