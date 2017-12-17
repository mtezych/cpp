
#include <windows/Window.h>

#include <windows/Bitmap.h>

#include <cassert>
#include <string>
#include <type_traits>

namespace windows
{
	namespace
	{
		HMODULE CurrentModuleHandle ()
		{
			auto moduleHandle = HMODULE { nullptr };

			const auto addressWithinModule = CurrentModuleHandle;

			const auto result = GetModuleHandleEx
			(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				reinterpret_cast<LPCTSTR>(addressWithinModule),
				&moduleHandle
			);
			assert(result != 0);
			assert(moduleHandle != nullptr);

			return moduleHandle;
		}

		std::wstring GenerateUniqueClassName ()
		{
			auto guid = GUID { };

			const auto resultHandle = CoCreateGuid(&guid);
			assert(resultHandle == S_OK);

			auto guidString = std::wstring
			{
				L"{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}~"
			};

			const auto charsWritten = StringFromGUID2
			(
				guid, &guidString[0], guidString.size()
			);
			assert(charsWritten == guidString.size());

			return guidString;
		}

		template <typename ObjectHandleType>
		ObjectHandleType SelectObjectForDeviceContext
		(
			const HDC deviceContextHandle,
			const ObjectHandleType objectHandle
		)
		{
			static_assert
			(
				std::is_same<ObjectHandleType, HBITMAP>::value ||
				std::is_same<ObjectHandleType, HBRUSH >::value ||
				std::is_same<ObjectHandleType, HFONT  >::value ||
				std::is_same<ObjectHandleType, HPEN   >::value,""
			);

			const auto replacedObjectHandle = SelectObject
			(
				deviceContextHandle, objectHandle
			);
			assert(replacedObjectHandle != nullptr);

			return static_cast<ObjectHandleType>(replacedObjectHandle);
		}
	}

	Window::Window (const util::uvec2& size)
	:
		moduleHandle        { nullptr },
		windowHandle        { nullptr },
		classAtom           { 0       },
		deviceContextHandle { nullptr }
	{
		moduleHandle = CurrentModuleHandle();

		const auto className = GenerateUniqueClassName();

		const auto windowClass = WNDCLASSEX
		{
			sizeof(WNDCLASSEX),                 // size
			CS_HREDRAW | CS_VREDRAW | CS_OWNDC, // style
			WindowProcedure,                    // window procedure
			0,                                  // class  extra data
			0,                                  // window extra data
			HINSTANCE { moduleHandle },         // instance
			LoadIcon(nullptr, IDI_APPLICATION), // icon
			LoadCursor(nullptr, IDC_ARROW),     // cursor
			nullptr,                            // background brush
			nullptr,                            // menu name
			className.c_str(),                  // class name
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
			0,                                   // extended style
			MAKEINTATOM(classAtom),             // class atom or name
			TEXT("WinAPI Window Title"),       // title
			WS_OVERLAPPEDWINDOW,              // style
			0, 0,                            // x, y
			static_cast<int>(size.width),   // width
			static_cast<int>(size.height), // height
			nullptr,                      // parent window
			nullptr,                     // menu
			HINSTANCE { moduleHandle }, // instance
			this                       // create parameter recived in WM_CREATE
		);
		assert(windowHandle != nullptr);

		deviceContextHandle = GetDC(windowHandle);
		assert(deviceContextHandle != nullptr);

		const auto previousUserData = SetWindowLongPtr
		(
			windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
		);
		assert(previousUserData == 0);

		const auto result = ShowWindow(windowHandle, SW_SHOW);
	}

	Window::~Window ()
	{
		const auto previousUserData = SetWindowLongPtr
		(
			windowHandle, GWLP_USERDATA, 0
		);
		assert(reinterpret_cast<Window*>(previousUserData) == this);

		auto result = ReleaseDC(windowHandle, deviceContextHandle);
		assert(result != 0);

		result = DestroyWindow(windowHandle);
		assert(result != 0);

		result = UnregisterClass(MAKEINTATOM(classAtom), moduleHandle);
		assert(result != 0);
	}

	Window::Window (Window&& window)
	:
		moduleHandle        { window.moduleHandle        },
		windowHandle        { window.windowHandle        },
		classAtom           { window.classAtom           },
		deviceContextHandle { window.deviceContextHandle }
	{
		window.moduleHandle        = nullptr;
		window.windowHandle        = nullptr;
		window.classAtom           = 0;
		window.deviceContextHandle = nullptr;
	}

	Window& Window::operator = (Window&& window)
	{
		moduleHandle        = window.moduleHandle;
		windowHandle        = window.windowHandle;
		classAtom           = window.classAtom;
		deviceContextHandle = window.deviceContextHandle;

		window.moduleHandle        = nullptr;
		window.windowHandle        = nullptr;
		window.classAtom           = 0;
		window.deviceContextHandle = nullptr;

		return *this;
	}

	LRESULT CALLBACK
	Window::WindowProcedure(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		const auto self = reinterpret_cast<Window*>
		(
			GetWindowLongPtr(hWindow, GWLP_USERDATA)
		);

		switch (uMsg)
		{
			case WM_CREATE :
			{
				const auto createStruct = reinterpret_cast<const LPCREATESTRUCT>(lParam);
				// create parameter send by CreateWindowEx()
				const auto createParam = createStruct->lpCreateParams;
				return 0;
			}
			case WM_KEYDOWN :
			{
				if (wParam == VK_ESCAPE)
				{
					const auto result = PostMessage(hWindow, WM_CLOSE, 0, 0);
					assert(result != 0);
				}
				return 0;
			}
			// Usually in response to WM_CLOSE apps call DestoryWindow().
			//
			// However, since Window destructor will call DestoryWindow()
			// anyway posting WM_QUIT by calling PostQuitMessage() is OK.
			//
			// This design allows ReciveMessages() to be called multiple times
			// and preserves validity of Window until it will be destroyed.
			//
			// Note that calling DestoryWindow() posts WM_QUIT message,
			// which is never possessed by WindowProc().
			case WM_CLOSE :
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

	void Window::Draw (const Bitmap& bitmap)
	{
		// Drawing without the WM_PAINT message is OK.
		// https://msdn.microsoft.com/en-us/library/dd162492(v=vs.85).aspx

		const auto previousBitmapHandle = SelectObjectForDeviceContext
		(
			bitmap.deviceContextHandle, bitmap.bitmapHandle
		);
		{
			const auto bitmapSize = bitmap.Size();

			const auto result = BitBlt
			(
				deviceContextHandle,
				0, 0, bitmapSize.width, bitmapSize.height,
				bitmap.deviceContextHandle,
				0, 0,
				SRCCOPY
			);
			assert(result != 0);
		}
		const auto bitmapHandle = SelectObjectForDeviceContext
		(
			bitmap.deviceContextHandle, previousBitmapHandle
		);
		assert(bitmapHandle == bitmap.bitmapHandle);
	}
}
