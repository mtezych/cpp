
#ifndef VULKAN_SWAPCHAIN
#define VULKAN_SWAPCHAIN

#include "VulkanSemaphore.h"

#include <vector>

namespace vk
{
	struct VulkanDrvice;

	struct VulkanSwapchain
	{
		VkSwapchainKHR swapchain;
		const VulkanDevice& device;

		PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR;
		PFN_vkDestroySwapchainKHR   vkDestroySwapchainKHR;
		PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
		PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR;

		VulkanSwapchain
		(
			const VulkanDevice& device,
			const VkSwapchainCreateInfoKHR& swapchainCreateInfo
		);

		~VulkanSwapchain();

		std::vector<VkImage> Images() const;

		struct AcquireInfo
		{
			uint32_t        imageIndex;
			VulkanSemaphore imageAvailable;
		};
		AcquireInfo Acquire() const;
	};
}

#endif
