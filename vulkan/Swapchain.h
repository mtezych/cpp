
#ifndef VULKAN_SWAPCHAIN
#define VULKAN_SWAPCHAIN

#include "Semaphore.h"

#include <vector>

namespace vk
{
	struct Device;

	struct Swapchain
	{
		const Device*  device;
		VkSwapchainKHR vkSwapchain;

		Swapchain
		(
			const Device&                   device,
			const VkSwapchainCreateInfoKHR& swapchainCreateInfo
		);

		~Swapchain();

		std::vector<VkImage> Images() const;

		struct AcquireInfo
		{
			uint32_t  imageIndex;
			Semaphore imageAvailable;
		};
		AcquireInfo Acquire() const;
	};
}

#endif
