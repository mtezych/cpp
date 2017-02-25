
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
	struct VulkanInstance;

	struct VulkanSurface
	{
		VkSurfaceKHR surface;
		VkInstance   instance;

//		PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
		PFN_vkDestroySurfaceKHR    vkDestroySurfaceKHR;

		VulkanSurface
		(
			const VulkanInstance&    instance,
			const platform::Display& display,
			const platform::Window&  window
		);

		~VulkanSurface();
	};
}

#endif
