
#include <vulkan/Surface.h>

#include <vulkan/Symbols.h>
#include <vulkan/Instance.h>

namespace vk
{
	Surface::Surface
	(
		const Instance&          instance,
		const platform::Display& display,
		const platform::Window&  window
	):
#if defined(PLATFORM_XLIB) || defined(PLATFORM_XCB)

		Surface { instance, display, window.nativeWindow }

#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_WINDOWS)

		Surface { instance, window.nativeWindow }

#endif
	{
	}

#ifdef VK_USE_PLATFORM_XLIB_KHR
	Surface::Surface
	(
		const Instance&      instance,
		const xlib::Display& display,
		const xlib::Window&  window
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE }
	{
		const auto surfaceCreateInfo = VkXlibSurfaceCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			display.xDisplay,
			window.xWindow
		};
		const auto result = instance.vkCreateXlibSurfaceKHR
		(
			instance.vkInstance,
			&surfaceCreateInfo,
			nullptr,
			&vkSurface
		);
		assert(result == VK_SUCCESS);
	}
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
	Surface::Surface
	(
		const Instance&        instance,
		const xcb::Connection& connection,
		const xcb::Window&     window
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE }
	{
		const auto surfaceCreateInfo = VkXcbSurfaceCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			connection.xcbConnection,
			window.xcbWindow
		};
		const auto result = instance.vkCreateXcbSurfaceKHR
		(
			instance.vkInstance,
			&surfaceCreateInfo,
			nullptr,
			&vkSurface
		);
		assert(result == VK_SUCCESS);
	}
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	Surface::Surface
	(
		const Instance&         instance,
		const wayland::Display& display,
		const wayland::Surface& surface
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE }
	{
		const auto surfaceCreateInfo = VkWaylandSurfaceCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			display.wlDisplay,
			surface.wlSurface,
		};
		const auto result = instance.vkCreateWaylandSurfaceKHR
		(
			instance.vkInstance,
			&surfaceCreateInfo,
			nullptr,
			&vkSurface
		);
		assert(result == VK_SUCCESS);
	}
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
	Surface::Surface
	(
		const Instance&        instance,
		const android::Window& window
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE }
	{
		const auto surfaceCreateInfo = VkAndroidSurfaceCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			window.NativeHandle()
		};
		const auto result = instance.vkCreateAndroidSurfaceKHR
		(
			instance.vkInstance,
			&surfaceCreateInfo,
			nullptr,
			&vkSurface
		);
		assert(result == VK_SUCCESS);
	}
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
	Surface::Surface
	(
		const Instance&        instance,
		const windows::Window& window
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE }
	{
		const auto surfaceCreateInfo = VkWin32SurfaceCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			window.moduleHandle,
			window.windowHandle,
		};
		const auto result = instance.vkCreateWin32SurfaceKHR
		(
			instance.vkInstance,
			&surfaceCreateInfo,
			nullptr,
			&vkSurface
		);
		assert(result == VK_SUCCESS);
	}
#endif

	Surface::~Surface ()
	{
		if (vkSurface != VK_NULL_HANDLE)
		{
			instance->vkDestroySurfaceKHR(instance->vkInstance, vkSurface, nullptr);
		}
	}

	Surface::Surface (Surface&& surface)
	:
		instance  { surface.instance  },
		vkSurface { surface.vkSurface }
	{
		surface.instance  = nullptr;
		surface.vkSurface = VK_NULL_HANDLE;
	}

	Surface& Surface::operator = (Surface&& surface)
	{
		if (vkSurface != VK_NULL_HANDLE)
		{
			instance->vkDestroySurfaceKHR(instance->vkInstance, vkSurface, nullptr);
		}

		instance  = surface.instance;
		vkSurface = surface.vkSurface;

		surface.instance  = nullptr;
		surface.vkSurface = VK_NULL_HANDLE;

		return *this;
	}
}
