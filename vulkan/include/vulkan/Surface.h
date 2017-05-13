
#ifndef VULKAN_SURFACE
#define VULKAN_SURFACE

#include <vulkan/vulkan.h>

#include <platform/Display.h>
#include <platform/Window.h>

#ifdef VK_USE_PLATFORM_XLIB_KHR

	#include <xlib/Display.h>
	#include <xlib/Window.h>

#endif

#ifdef VK_USE_PLATFORM_XCB_KHR

	#include <xcb/Connection.h>
	#include <xcb/Window.h>

#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

	#include <wayland/Display.h>
	#include <wayland/Surface.h>

#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR

	#include <android/Window.h>

#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR

	#include <windows/Window.h>

#endif

namespace vk
{
	struct Instance;

	struct Surface
	{
		const Instance* instance;
		VkSurfaceKHR    vkSurface;

		Surface
		(
			const Instance&          instance,
			const platform::Display& display,
			const platform::Window&  window
		);

#ifdef VK_USE_PLATFORM_XLIB_KHR
		Surface
		(
			const Instance&      instance,
			const xlib::Display& display,
			const xlib::Window&  window
		);
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
		Surface
		(
			const Instance&        instance,
			const xcb::Connection& connection,
			const xcb::Window&     window
		);
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		Surface
		(
			const Instance&         instance,
			const wayland::Display& display,
			const wayland::Surface& surface
		);
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
		Surface
		(
			const Instance&        instance,
			const android::Window& window
		);
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
		Surface
		(
			const Instance&        instance,
			const windows::Window& window
		);
#endif

		~Surface();
	};
}

#if defined(PLATFORM_XLIB)

#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME

#elif defined(PLATFORM_XCB)

#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME

#elif defined(PLATFORM_WAYLAND)

#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME

#elif defined(PLATFORM_ANDROID)

#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME

#elif defined(PLATFORM_WINDOWS)

#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#endif

#endif
