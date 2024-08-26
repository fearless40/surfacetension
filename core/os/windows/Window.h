#pragma once

#include "..\util\Math\Vector2.h"
#include "..\util\Math\RectT.h"
#include "..\util\InvasiveRefCount.h"
#include "..\util\RefCountPointer.h"
#include <unordered_map>
#include <string>
//#include <boost\container\flat_map.hpp>

// Stripped down window class for use in a game engine. 

class Window : public RefCount
{
public: //Type defs
	typedef RefCountPointer<Window>  WindowPtr;
	typedef std::unordered_map< HWND, WindowPtr > ChildList;

	typedef ST::Vector2T<int> stdPoint;
	typedef ST::RectT<int, stdPoint> stdBounds;

	typedef ST::Vector2T<double> PointD;
	typedef ST::RectT<double, PointD> RectD;

	typedef stdBounds RectI;
	typedef stdPoint PointI;



public:
	// Constructors and Destructor
	Window(void);
	Window(HWND wnd);
	virtual ~Window(void);

public: // Properties
	HWND getHWND() const { return hwnd; }
	bool isValid() const { if(hwnd!=0) return true; return false; }
	RectI getClientPos() const;

public:  // Modification functions
	
	// May want to override to disallow modifing the window
	// Very dangerous... can lead to memory leaks if the Window class already
	// has a window in it. It should destroy the old window.
	virtual void setHWND( HWND wnd );
	
	// May want to override
	virtual void setParent( WindowPtr parent );

	// May want to override
	virtual void addChild( WindowPtr child );
	virtual void removeChild( WindowPtr child );

	// Show the window
	virtual void show();

	// Getters / Information

	bool operator < ( WindowPtr p ) const
	{
		return hwnd < p->getHWND();
	}

// Virtual Messaging
protected: 
	
	// Signifies when a window has a new parent set for it.
	// Allows the window to connect to the parents signals
	virtual void onParent( WindowPtr parent );

	// When a window gets resized this gets called
	// If you ovverride make sure that you call
	// Window::onSize in your overriden function, otherwise
	// the signal to change the size won't be activated
	virtual void onSize( stdBounds newSize );

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
	virtual void onPaint( HDC dc, RectI bounds );

	// WM_COMMAND on Acclerator
	virtual void onAcclerator( int id );

	// WM_COMMAND on Menu
	virtual void onMenu( int id );

	// WM_KEY*
	virtual void onKey(WORD vkCode, bool isKeyUp, int repeatCount);


	// Rather than editing the window class everytime I need to handle a new command
	// you can override the following function it is called if Window::WndProc doesn't
	// have any logic. 
	virtual int WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

protected: // Protected Functions
	


private: //Private functions
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
	static HWND createHWND(HWND parentHWND = 0, const std::wstring WindowsClass = L"BlankWindow" , RectI bounds = RectI(0,0,0,0), DWORD style = 0, DWORD ExtendedStyle = 0, const std::wstring title = L"No Title",  HINSTANCE hinst = 0, HMENU hmenu = 0 );


public: // Static Functions
	// Allows one to extract a C++ class pointer from a windows 
	// HWND. Usefull in the message map handling classes
	static void SetClassPointer(Window * wPtr);
	static Window * GetClassPointer(HWND wHandle);
	static int CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static std::wstring RegisterWindowClasses(HINSTANCE hInst, int AppResourceID,int IconResourceID, std::wstring wTitle, std::wstring wClass);
	
	// Override to create a new window
	static WindowPtr createWindow(std::wstring WindowsClass, RectI bounds = RectI{ 0,0,0,0 }, DWORD style = 0, DWORD ExtendedStyle = 0, const std::wstring title = L"", WindowPtr parent = nullptr, HINSTANCE hinst = 0, HMENU hmenu = 0);
	

	/*template <class WindowType>
	static WindowPtr create( WindowPtr parent = WindowPtr() )
	{
		
		HWND hwnd = 0;
		HWND pHWND = 0;

		// If there is a parent then
		if( parent )
			pHWND = parent->getHWND();	
			
		hwnd = WindowType::createHWND( pHWND );
	
		if(hwnd)
		{
			auto win = std::make_shared< WindowType >( hwnd );

			 // Fire the virtual event to allow the class to make modifications to the code
			win->onCreateStatic();
	 
			 // If a parent window was specified then make sure the class knows
			 if( parent ) 
				 parent->addChild( win );

			return win;
		}
		else
			return std::shared_ptr< WindowType >();
	}
	*/
	// Special Helper Functions
	static RectI fromRECT( const RECT & r );
	
};


/*
template <class BaseWindow> 
class Window_Helper_Implementation : public BaseWindow
{
protected:
	friend class Window;
	static HWND createHWND(HWND parent) { return BaseWindow::createHWND( parent ); } 
};

#define WindowNeededDeclares(MYCLASSNAME) friend class Window; friend class Window_Helper_Implementation<MYCLASSNAME>;

*/