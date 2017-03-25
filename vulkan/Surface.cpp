
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
		surface  { VK_NULL_HANDLE    },
		instance { instance.instance },

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
//			instance.instance,
//			&surfaceCreateInfo,
//			nullptr,
//			&surface
//		);
//		assert(result == VK_SUCCESS);
	}

	Surface::~Surface()
	{
		if (surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(instance, surface, nullptr);
		}
	}
}
