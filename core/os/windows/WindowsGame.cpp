
#include "WindowsGame.h"
#include "Event.hpp"
#include "Window.h"
#include "os.hpp"

// #include "../game-gwell/Game.h"
namespace os::windows {
WindowsGame::~WindowsGame() {
  // delete graphics_driver;
}

WindowsGame::WindowsGame(HINSTANCE hInstance)
    : mInst(hInstance), mRunning(false) {
  Window::RegisterWindowClasses(mInst, NULL, NULL, L"D3D Window", L"D3DWindow");
  mAccelTable = LoadAccelerators(mInst, (LPCTSTR)NULL);
  setHWND(Window::createHWND(0, L"D3DWindow"));

  show();
}

void WindowsGame::run() {
  MSG msg;
  mRunning = true;
  RECT wndSize;
  GetWindowRect(getHWND(), &wndSize);

  unsigned int width = wndSize.right - wndSize.left;
  unsigned int height = wndSize.bottom - wndSize.top;

  m_driver.SetWindow(getHWND(), width, height);
  m_driver.CreateDeviceResources();
  m_driver.CreateWindowSizeDependentResources();

  timer.start();

  while (mRunning) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (!TranslateAccelerator(msg.hwnd, mAccelTable, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }

      if (msg.message == WM_QUIT) {
        mRunning = false;
        break;
      }
    }

    next_frame();
  }
  return;
}

void WindowsGame::shutdown() { mRunning = false; }

void WindowsGame::next_frame() {
  timer.startFrame();
  if (timer.isUpdateRequired()) {
    next_game_step(timer.getInterpolation());
    timer.performUpdate();
  } else {
    next_render_step(timer.getInterpolation());
  }
}

void WindowsGame::next_game_step(float step) {
  Events::Event<os::NextLogicFrame>::Fire({step});
}

void WindowsGame::next_render_step(float step) {
  Events::Event<os::NextRenderFrame>::Fire({step});

  m_driver.Prepare();
  auto commandList = m_driver.GetCommandList();

  // Clear the views.
  auto const rtvDescriptor = m_driver.GetRenderTargetView();
  auto const dsvDescriptor = m_driver.GetDepthStencilView();

  float colors[4] = {0.2f, .2f, 1.f, 0.f};

  commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
  commandList->ClearRenderTargetView(rtvDescriptor, colors, 0, nullptr);
  commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH,
                                     1.0f, 0, 0, nullptr);

  // Set the viewport and scissor rect.
  auto const viewport = m_driver.GetScreenViewport();
  auto const scissorRect = m_driver.GetScissorRect();
  commandList->RSSetViewports(1, &viewport);
  commandList->RSSetScissorRects(1, &scissorRect);

  m_driver.Present();
}

void WindowsGame::onSize(RectInt newSize) {}

void WindowsGame::onKey(WORD vkCode, bool isKeyUp, int repeatCount) {
  // Events::Event<Engine::KeyEvent>::Fire({vkCode, isKeyUp, repeatCount});
}

} // namespace os::windows