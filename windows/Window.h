
#define UNICODE
#include <Windows.h>

#include <cassert>
#include <cstdint>

namespace windows
{
	class Window
	{
	private:
		HWND windowHandle;
		ATOM classAtom;
		HDC  deviceContextHandle;

		static LRESULT CALLBACK
		WindowProcedure(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		Window(const uint32_t width, const uint32_t height);
		~Window();

		Window(Window&& window);
		Window(const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		template <typename Callable>
		void RecieveMessages(const Callable& render) const
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
	};
}
