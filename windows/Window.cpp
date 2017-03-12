
#include "Window.h"

namespace windows
{
	Window::Window(uint32_t width, uint32_t height)
	:
		windowHandle        { nullptr },
		classAtom           { 0       },
		deviceContextHandle { nullptr }
	{
		const auto instance = HINSTANCE { GetModuleHandle(nullptr) };
		assert(instance != nullptr);

		const auto windowClass = WNDCLASSEX
		{
			sizeof(WNDCLASSEX),                 // size
			CS_HREDRAW | CS_VREDRAW | CS_OWNDC, // style
			WindowProcedure,                    // window procedure
			0,                                  // class  extra data
			0,                                  // window extra data
			instance,                           // instance
			LoadIcon(nullptr, IDI_APPLICATION), // icon
			LoadCursor(nullptr, IDC_ARROW),     // cursor
			nullptr,                            // background brush
			nullptr,                            // menu name
			TEXT("WindowClass"),                // class name
			nullptr,                            // small icon
		};
		assert(windowClass.hIcon   != nullptr);
		assert(windowClass.hCursor != nullptr);

		classAtom = RegisterClassEx(&windowClass);
		assert(classAtom != 0);

		// Class name and atom can be used interchangeably.
		// https://blogs.msdn.microsoft.com/oldnewthing/20080501-00/?p=22503
		windowHandle = CreateWindowEx
		(
			0,                            // extended style
			MAKEINTATOM(classAtom),       // class atom or name
			TEXT("WinAPI Window Title"),  // title
			WS_OVERLAPPEDWINDOW,          // style
			0, 0, width, height,          // x, y, width, height
			nullptr,                      // parent window
			nullptr,                      // menu
			instance,                     // instance
			nullptr                       // parameter
		);
		assert(windowHandle != nullptr);

		deviceContextHandle = GetDC(windowHandle);
		assert(deviceContextHandle != nullptr);

		const auto result = ShowWindow(windowHandle, SW_SHOW);
	}

	Window::~Window()
	{
		auto result = ReleaseDC(windowHandle, deviceContextHandle);
		assert(result != 0);

		result = DestroyWindow(windowHandle);
		assert(result != 0);

		const auto instance = HINSTANCE { GetModuleHandle(nullptr) };
		assert(instance != nullptr);

		result = UnregisterClass(MAKEINTATOM(classAtom), instance);
		assert(result != 0);
	}

	Window::Window(Window&& window)
	:
		windowHandle        { window.windowHandle        },
		classAtom           { window.classAtom           },
		deviceContextHandle { window.deviceContextHandle }
	{
		window.windowHandle        = nullptr;
		window.classAtom           = 0;
		window.deviceContextHandle = nullptr;
	}

	Window& Window::operator = (Window&& window)
	{
		windowHandle        = window.windowHandle;
		classAtom           = window.classAtom;
		deviceContextHandle = window.deviceContextHandle;

		window.windowHandle        = nullptr;
		window.classAtom           = 0;
		window.deviceContextHandle = nullptr;

		return *this;
	}

	LRESULT CALLBACK
	Window::WindowProcedure(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_KEYDOWN :
			{
				if (wParam == VK_ESCAPE)
				{
					PostMessage(hWindow, WM_CLOSE, 0, 0);
				}
				return 0;
			}
			case WM_CLOSE :
			// Usually in response to WM_CLOSE apps call DestoryWindow().
			//
			// However, since Window destructor will call DestoryWindow()
			// anyway posting WM_QUIT by calling PostQuitMessage() is OK.
			//
			// This design allows ReciveMessages() to be called multiple times
			// and preserves valifity of Window until it will be destroyed.
			//
			// Note that calling DestoryWindow() posts WM_QUIT message,
			// which is never pocessed by WindowProc().
			case WM_DESTROY :
			{
				PostQuitMessage(0);
				return 0;
			}
			default :
			{
				return DefWindowProc(hWindow, uMsg, wParam, lParam);
			}
		}
	}
}
