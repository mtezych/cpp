
#ifndef VULKAN_FRAMEBUFFER
#define VULKAN_FRAMEBUFFER

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include "VulkanImageView.h"

#include <vector>

namespace vk
{
	struct VulkanDevice;
	struct VulkanRenderPass;

	struct VulkanFramebuffer
	{
		VkDevice      device;
		VkFramebuffer framebuffer;

		PFN_vkCreateFramebuffer  vkCreateFramebuffer;
		PFN_vkDestroyFramebuffer vkDestroyFramebuffer;

		VulkanFramebuffer();

		VulkanFramebuffer
		(
			const VulkanDevice&                 device,
			const VulkanRenderPass&             renderPass,
			const std::vector<VulkanImageView>& imageViews,
			const VkExtent2D&                   size,
			const uint32_t                      layers
		);

		~VulkanFramebuffer();

		VulkanFramebuffer(VulkanFramebuffer&& framebuffer);
		VulkanFramebuffer(const VulkanFramebuffer& framebuffer) = delete;

		VulkanFramebuffer& operator =(VulkanFramebuffer&& framebuffer);
		VulkanFramebuffer& operator =(const VulkanFramebuffer& framebuffer) = delete;
	};
}

#endif
