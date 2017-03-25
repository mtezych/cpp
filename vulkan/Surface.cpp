
#include "Surface.h"

#include "Symbols.h"
#include "Instance.h"

namespace vk
{
	Surface::Surface
	(
		const Instance&          instance,
		const platform::Display& display,
		const platform::Window&  window
	):
		instance  { &instance },
		vkSurface { VK_NULL_HANDLE },


//		vkCreateXlibSurfaceKHR
//		{
//			instance.LoadInstanceProcedure<symbol::vkCreateXlibSurfaceKHR>()
//		},
		vkDestroySurfaceKHR
		{
			instance.LoadInstanceProcedure<symbol::vkDestroySurfaceKHR>()
		}
	{
//		const auto surfaceCreateInfo = VkXlibSurfaceCreateInfoKHR
//		{
//			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
//			nullptr,
//			0,
//			window.display,
//			window.window
//		};
//		const auto result = vkCreateXlibSurfaceKHR
//		(
//			instance.vkInstance,
//			&surfaceCreateInfo,
//			nullptr,
//			&vkSurface
//		);
//		assert(result == VK_SUCCESS);
	}

	Surface::~Surface()
	{
		if (vkSurface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(instance->vkInstance, vkSurface, nullptr);
		}
	}
}
