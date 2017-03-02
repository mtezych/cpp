
#ifndef VULKAN_RENDER_PASS
#define VULKAN_RENDER_PASS

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include "VulkanSymbols.h"

#include <cassert>
#include <vector>

namespace vk
{
	struct VulkanDevice;

	struct VulkanRenderPass
	{
		VkDevice     device;
		VkRenderPass renderPass;

		PFN_vkCreateRenderPass  vkCreateRenderPass;
		PFN_vkDestroyRenderPass vkDestroyRenderPass;

		VulkanRenderPass
		(
			const VulkanDevice&                         device,
			const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>&    subpasses,
			const std::vector<VkSubpassDependency>&     dependencies
		);

		~VulkanRenderPass();
	};
}

#endif
