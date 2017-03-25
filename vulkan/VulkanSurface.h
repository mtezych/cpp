
#ifndef VULKAN_SURFACE
#define VULKAN_SURFACE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

// #include <Window.h>

namespace platform
{
	struct Display;
	struct Window;
}

namespace vk
{
	struct Instance;

	struct Surface
	{
		VkSurfaceKHR surface;
		VkInstance   instance;

//		PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
		PFN_vkDestroySurfaceKHR    vkDestroySurfaceKHR;

		Surface
		(
			const Instance&          instance,
			const platform::Display& display,
			const platform::Window&  window
		);

		~Surface();
	};
}

#endif
