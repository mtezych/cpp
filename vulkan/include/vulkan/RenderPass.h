
#ifndef VULKAN_RENDER_PASS
#define VULKAN_RENDER_PASS

#include <vulkan/vulkan.h>

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

		RenderPass (RenderPass&& renderPass);
		RenderPass (const RenderPass& renderPass) = delete;

		RenderPass& operator = (RenderPass&& renderPass);
		RenderPass& operator = (const RenderPass& renderPass) = delete;
	};
}

#endif
