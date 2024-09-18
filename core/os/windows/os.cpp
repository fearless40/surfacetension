#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include "os.hpp"
#include "WindowsGame.h"
#include <Windows.h>

static os::windows::WindowsGame *g_main_window = nullptr;
static HINSTANCE g_hInstance;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
            int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  g_hInstance = hInstance;

  // g_main_window = new os::windows::WindowsGame(hInstance);

  game_main();

  delete g_main_window;
  return 0;
}

namespace os {
namespace console {
bool connect() {

  if (!AllocConsole()) {
    // Add some error handling here.
    // You can call GetLastError() to get more info about the error.
    return false;
  }

  // std::cout, std::clog, std::cerr, std::cin
  FILE *fDummy;
  freopen_s(&fDummy, "CONOUT$", "w", stdout);
  freopen_s(&fDummy, "CONOUT$", "w", stderr);
  freopen_s(&fDummy, "CONIN$", "r", stdin);
  std::cout.clear();
  std::clog.clear();
  std::cerr.clear();
  std::cin.clear();

  // std::wcout, std::wclog, std::wcerr, std::wcin
  HANDLE hConOut = CreateFile(TEXT("CONOUT$"), GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  HANDLE hConIn = CreateFile(TEXT("CONIN$"), GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
  SetStdHandle(STD_ERROR_HANDLE, hConOut);
  SetStdHandle(STD_INPUT_HANDLE, hConIn);
  std::wcout.clear();
  std::wclog.clear();
  std::wcerr.clear();
  std::wcin.clear();

  return true;
}

void disconnect() { FreeConsole(); }

} // namespace console

bool create_main_window() {
  g_main_window = new os::windows::WindowsGame(g_hInstance);
  return true;
}

void run_event_loop() { g_main_window->run(); }


} // namespace os