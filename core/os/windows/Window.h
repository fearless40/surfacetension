#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define UNICODE
// Windows Header Files:

#include <windows.h>
#include <string>
#include <unordered_map>

#include "RectT.hpp"
#include "Vector2.hpp"


// Stripped down window class for use in a game engine.
namespace os::windows {

using PointInt = math::Vector2T<int>;
using RectInt =  math::RectT<int, PointInt>;

using PointDouble = math::Vector2T<double>;
using RectDouble  = math::RectT<double, PointDouble>;

class Window {
public: // Type defs
  using WindowPtr = Window *;
  typedef std::unordered_map<HWND, WindowPtr> ChildList;

public:
  // Constructors and Destructor
  Window(void);
  Window(HWND wnd);
  virtual ~Window(void);

public: // Properties
  HWND getHWND() const { return hwnd; }
  bool isValid() const {
    if (hwnd != 0)
      return true;
    return false;
  }
  RectInt getClientPos() const;

public: // Modification functions
  // May want to override to disallow modifing the window
  // Very dangerous... can lead to memory leaks if the Window class already
  // has a window in it. It should destroy the old window.
  virtual void setHWND(HWND wnd);

  // May want to override
  virtual void setParent(WindowPtr parent);

  // May want to override
  virtual void addChild(WindowPtr child);
  virtual void removeChild(WindowPtr child);

  // Show the window
  virtual void show();

  // Getters / Information

  bool operator<(WindowPtr p) const { return hwnd < p->getHWND(); }

  // Virtual Messaging
protected:
  // Signifies when a window has a new parent set for it.
  // Allows the window to connect to the parents signals
  virtual void onParent(WindowPtr parent);

  // When a window gets resized this gets called
  // If you ovverride make sure that you call
  // Window::onSize in your overriden function, otherwise
  // the signal to change the size won't be activated
  virtual void onSize(RectInt newSize);

  // Called on WM_COMMAND
  // Return true if your window handled the event. Otherwise return false
  // to let the owner window know if it needs to handle the event
  virtual bool onCommand(int wmEvent);

  // Used to determine if a child window or the current window
  // should handle the event
  virtual bool onCommandDispatch(HWND wnd, int wmEvent);

  // Called on WM_DESTROY
  // Only useful if you need to do some very special clean up, otherwise
  // the class should clean up after itself
  virtual void onDestroy();

  // Called when create is called (not associated with WM_CREATE)
  // Override to set certain window functions
  virtual void onCreateStatic();

  // Called on WM_PAINT
  virtual void onPaint(HDC dc, RectInt bounds);

  // WM_COMMAND on Acclerator
  virtual void onAcclerator(int id);

  // WM_COMMAND on Menu
  virtual void onMenu(int id);

  // WM_KEY*
  virtual void onKey(WORD vkCode, bool isKeyUp, int repeatCount);

  // Rather than editing the window class everytime I need to handle a new
  // command you can override the following function it is called if
  // Window::WndProc doesn't have any logic.
  virtual int WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected: // Protected Functions
private:   // Private functions
  // Remove the class pointer stored in the windows property map.
  // The memory is allocated on the process heap and needs to be
  // freed.
  void _RemoveHWNDProperty();

protected:
  // Window Handle
  HWND hwnd;

  // List of child windows
  ChildList children;

protected:
  static HWND createHWND(HWND parentHWND = 0,
                         const std::wstring WindowsClass = L"BlankWindow",
                         RectInt bounds = RectInt(0, 0, 0, 0), DWORD style = 0,
                         DWORD ExtendedStyle = 0,
                         const std::wstring title = L"No Title",
                         HINSTANCE hinst = 0, HMENU hmenu = 0);

public: // Static Functions
  // Allows one to extract a C++ class pointer from a windows
  // HWND. Usefull in the message map handling classes
  static void SetClassPointer(Window *wPtr);
  static Window *GetClassPointer(HWND wHandle);
  static int CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                              LPARAM lParam);
  static std::wstring RegisterWindowClasses(HINSTANCE hInst, int AppResourceID,
                                            int IconResourceID,
                                            std::wstring wTitle,
                                            std::wstring wClass);

  // Override to create a new window
  static WindowPtr createWindow(std::wstring WindowsClass,
                                RectInt bounds = RectInt{0, 0, 0, 0},
                                DWORD style = 0, DWORD ExtendedStyle = 0,
                                const std::wstring title = L"",
                                WindowPtr parent = nullptr, HINSTANCE hinst = 0,
                                HMENU hmenu = 0);

  // Special Helper Functions
  static RectInt fromRECT(const RECT &r);
};

} // namespace os