
#ifndef VULKAN_SEMAPHORE
#define VULKAN_SEMAPHORE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanDevice;

	struct VulkanSemaphore
	{
		VkSemaphore semaphore;
		VkDevice    device;

		PFN_vkCreateSemaphore  vkCreateSemaphore;
		PFN_vkDestroySemaphore vkDestroySemaphore;

		VulkanSemaphore();

		VulkanSemaphore(const VulkanDevice& device);

		VulkanSemaphore(VulkanSemaphore&& semaphore);
		VulkanSemaphore& operator = (VulkanSemaphore&& semaphore);

		~VulkanSemaphore();
	};
}

#endif
