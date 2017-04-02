
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
		const Device* device;
		VkRenderPass  vkRenderPass;

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