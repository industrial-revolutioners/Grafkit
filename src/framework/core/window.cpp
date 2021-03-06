#include "stdafx.h"

#include <tchar.h>

#include "window.h"

//============================================================================================================
// Window handler and callback
//============================================================================================================
#define LRWAPI LRESULT WINAPI
typedef LRWAPI windowCallBackProc_t(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

namespace {
	LRWAPI window_callback_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    Window* window;

	LRWAPI window_callback_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
			// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
			} break;

			// Check if the window is being closed.
			case WM_CLOSE:
			{
				PostQuitMessage(0);
			} break;

			// All other messages pass to the message handler in the system class.
			default:
			{
#if 0
				Window *wnd = findWindow(hwnd);
				if (wnd && wnd->getHandler())
					return wnd->getHandler()->MessageHandler(hwnd, msg, wp, lp);
#else
				if(window && window->getHandler())
					return window->getHandler()->MessageHandler(hwnd, msg, wp, lp);

#endif
			}
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

//============================================================================================================
// Windowmaker class
//============================================================================================================

Window::Window(WindowHandler *_handler)
	:handler(_handler)
{
	window = this;
	ZeroMemory(this->wndtitle, sizeof(this->wndtitle));
	this->init();

}

Window::Window(WindowHandler *_handler, TCHAR *title)
	: handler(_handler)
{
	window = this;
	lstrcpyn(title, this->wndtitle, 512);
	this->init();
}

void Window::init() {
//	registerHwnd(this);

	this->w_height = 0;
	this->w_width = 0;
	this->r_height = 0;
	this->r_width = 0;
	this->fullscreen = 0;

	this->hWnd = nullptr;
	this->hDc = nullptr;
	this->hRc = nullptr;

	this->isWindShown = false;

	// Register A Window Class
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = static_cast<WNDPROC>(&window_callback_proc);
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = "Engine";
	if (RegisterClassEx(&windowClass) == 0)
	{
		MessageBox(HWND_DESKTOP, _T("RegisterClassEx Failed!"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

Window::~Window(){
	this->destructEverything();
//	unregisterHwnd(this);
	window = nullptr;
}

void Window::createWindow(int sx, int sy, int isfullscreen){
	this->w_width = sx;
	this->w_height= sy;
	this->fullscreen = isfullscreen;

	DWORD windowFrame  = 0;
	DWORD windowStyle  = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
	DWORD windowExStyle= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT windowRect = {0, 0, this->w_width, this->w_height};

	if( fullscreen ) {
		DEVMODE	screenSettings;
		ZeroMemory( &screenSettings, sizeof( screenSettings ) );
		screenSettings.dmSize		= sizeof( screenSettings );
		screenSettings.dmPelsWidth	= w_width;
		screenSettings.dmPelsHeight	= w_height;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		long reso = ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN );
		if( reso != DISP_CHANGE_SUCCESSFUL ) {
			MessageBox( nullptr, _T(""), _T("Could not initialize fullscreen window."), MB_OK );
			exit(1);
		}
		windowFrame = NULL;
		ShowCursor(false);
	} else {
		ShowCursor(true);
		windowFrame = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ;
	}

	this->hWnd = CreateWindowEx(windowExStyle, _T("Engine"), this->wndtitle, windowStyle | windowFrame, windowRect.top, windowRect.left, windowRect.right, windowRect.bottom, nullptr, nullptr, GetModuleHandle( nullptr ), nullptr );

	if( hWnd == nullptr ){
		MessageBox (HWND_DESKTOP, _T("Cannot create window"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}

	this->hDc = GetDC(hWnd);	///@todo: ez a resource nincs felszabaditva

	GetClientRect(this->hWnd, &windowRect);
	this->r_height = windowRect.bottom;
	this->r_width = windowRect.right;

	this->hideWindow();
}

void Window::showWindow(){
	SetForegroundWindow(hWnd);
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );
	this->isWindShown = true;
}

void Window::hideWindow(){
	ShowWindow( hWnd, SW_HIDE );
	UpdateWindow( hWnd );
	this->isWindShown = false;
}

void Window::destructEverything()
{
	peekWindowMessage();
	if (this->hWnd != nullptr)
	{
		if (this->hDc != nullptr)
		{
			ReleaseDC (this->hWnd, this->hDc);
			this->hDc = nullptr;
		}
		DestroyWindow (this->hWnd);
		this->hWnd = nullptr;
	}

	if (this->fullscreen)
	{
		ChangeDisplaySettings (nullptr,0);
		ShowCursor (TRUE);
	}
	peekWindowMessage();

    UnregisterClass("Engine", nullptr);
}

void Window::destroyWindow(){
	this->destructEverything();
}

void Window::recreateWindow(){
	bool show = this->isWindShown;
	this->destroyWindow();
	this->createWindow(this->w_width, this->w_height, this->fullscreen);
	if (show) this->showWindow(); else this->hideWindow();
}

bool Window::peekWindowMessage(){
	MSG uMsg;
	bool quit = false;
	while (PeekMessage( &uMsg, nullptr, 0, 0, PM_REMOVE)) { 
		TranslateMessage( &uMsg ); 
		DispatchMessage( &uMsg ); 
		if (uMsg.message == WM_QUIT) quit = true;
	}
	return quit;
}

void Window::setTitle(const char* txt){
	SetWindowText(hWnd, txt);
}

