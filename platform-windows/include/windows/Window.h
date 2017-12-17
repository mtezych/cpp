
#ifndef PLATFORM_WINDOWS_WINDOW
#define PLATFORM_WINDOWS_WINDOW

#include <Windows.h>

#include <util/vec.h>

namespace windows
{
	struct Bitmap;

	// NUCLEX GAMES BLOG -> Simple Window Class
	// http://blog.nuclex-games.com/2012/07/simple-main-window-class

	struct Window
	{
		// For Win32 HINSTANCE and HMODULE are both the base address of module.
		// https://blogs.msdn.microsoft.com/oldnewthing/20040614-00/?p=38903

		HMODULE moduleHandle;
		HWND    windowHandle;
		ATOM    classAtom;
		HDC     deviceContextHandle;

		static LRESULT CALLBACK
		WindowProcedure (HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

		Window (const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		template <typename Callable>
		void ReceiveMessages (const Callable& render) const
		{
			while (true)
			{
				auto message = MSG { };

				if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					 DispatchMessage(&message);

					if (message.message == WM_QUIT)
					{
						break;
					}
				}
				else
				{
					render();
				}
			}
		}

		void Draw (const Bitmap& bitmap);
	};
}

#endif
