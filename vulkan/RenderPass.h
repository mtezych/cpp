
#ifndef VULKAN_RENDER_PASS
#define VULKAN_RENDER_PASS

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include "Symbols.h"

#include <cassert>
#include <vector>

namespace vk
{
	struct Device;

	struct RenderPass
	{
		VkDevice     device;
		VkRenderPass renderPass;

		PFN_vkCreateRenderPass  vkCreateRenderPass;
		PFN_vkDestroyRenderPass vkDestroyRenderPass;

		RenderPass
		(
			const Device&                               device,
			const std::vector<VkAttachmentDescription>& attachments,
			const std::vector<VkSubpassDescription>&    subpasses,
			const std::vector<VkSubpassDependency>&     dependencies
		);

		~RenderPass();
	};
}

#endif
