
#ifndef VULKAN_SURFACE
#define VULKAN_SURFACE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef VK_USE_PLATFORM_XLIB_KHR
#include <xlib/Display.h>
#include <xlib/Window.h>
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

#ifdef VK_USE_PLATFORM_XLIB_KHR
		Surface
		(
			const Instance&      instance,
			const xlib::Display& display,
			const xlib::Window&  window
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
			const windows::Window& window,
		);
#endif

		~Surface();
	};
}

#endif
