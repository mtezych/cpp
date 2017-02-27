
#ifndef VULKAN_COMMAND_POOL
#define VULKAN_COMMAND_POOL

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanDevice;

	struct VulkanCommandPool
	{
		VkCommandPool commandPool;
		VkDevice      device;

		PFN_vkCreateCommandPool  vkCreateCommandPool;
		PFN_vkDestroyCommandPool vkDestroyCommandPool;

		VulkanCommandPool(const VulkanDevice& device, const uint32_t queueFamilyIndex);

		~VulkanCommandPool();
	};
}

#endif
