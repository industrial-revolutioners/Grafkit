#include "stdafx.h"

#include "System.h"
#include "input.h"

#include "window.h"

///@todo add log mock

using namespace Grafkit;
using namespace FWdebugExceptions;

System::System()
	: WindowHandler(), m_window(this), m_pInput(nullptr)
{
	// init logger 
	LOGGER(Log::Logger().Info("---- APPSTART ----"));
}

System::~System()
{
	delete m_pInput;

}

int System::Execute() {
	int result = 0;
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// ================================================================================================================================
	// --- init

	// +++ crete graphics device here 

	// + exception handling
	try
	{
		result = this->Initialize();
		if (result != 0) {
			this->Release();
			return 1;
		}
	}
	catch (FWdebug::Exception & ex)
	{
		///@todo handle exceptions here 
		MessageBoxA(nullptr, ex.what(), "Exception", 0);
		LOGGER(Log::Logger().Error(ex.what()));

		this->Release();

		return 0;	// who will do the shutdown???
	}

	// ================================================================================================================================
	// --- Mainloop

	try
	{
		int done = 0;
		while (!done)
		{
			// Handle the windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			else
			{
				// Otherwise do the frame processing.
				result = this->Mainloop();
				if (result != 0)
				{
					done = 1;
				}
			}

		}

	}
	catch (FWdebug::Exception& ex)
	{
#ifdef _DEBUG
		DebugBreak();
#endif 
		MessageBoxA(NULL, ex.what(), "Exception", 0);
		LOGGER(Log::Logger().Error(ex.what()));

	}

	// ================================================================================================================================
	// --- teardown
	{
		this->Release();
	}

	this->ShutdownWindows();

	return 0;
}



LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		m_pInput->KeyDown((unsigned int)wparam);
		return 0;
	}

	case WM_KEYUP:
	{
		m_pInput->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void System::InitializeWindows(int screenWidth, int screenHeight, int fullscreen, int resizeable, const char* pTitle)
{
	this->m_window.createWindow(screenWidth, screenHeight, fullscreen);
	this->m_window.showWindow();
	this->m_window.setTitle(pTitle);

	m_pInput = new Input();
	m_pInput->Initialize();
}

void System::ShutdownWindows()
{
	this->m_window.destroyWindow();
}

