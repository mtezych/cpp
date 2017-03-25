
#ifndef VULKAN_SWAPCHAIN
#define VULKAN_SWAPCHAIN

#include "Semaphore.h"

#include <vector>

namespace vk
{
	struct Device;

	struct Swapchain
	{
		VkSwapchainKHR swapchain;
		const Device&  device;

		PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR;
		PFN_vkDestroySwapchainKHR   vkDestroySwapchainKHR;
		PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
		PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR;

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
