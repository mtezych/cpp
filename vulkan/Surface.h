
#ifndef VULKAN_SURFACE
#define VULKAN_SURFACE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef VK_USE_PLATFORM_XLIB_KHR
#include "Display.h"
#include "Window.h"
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#include "Display.h"
#include "Surface.h"
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#include "Window.h"
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
#include "Window.h"
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
