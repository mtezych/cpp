
#include "VulkanRenderPass.h"

#include "VulkanDevice.h"

namespace vk
{
	RenderPass::RenderPass
	(
		const Device&                               device,
		const std::vector<VkAttachmentDescription>& attachments,
		const std::vector<VkSubpassDescription>&    subpasses,
		const std::vector<VkSubpassDependency>&     dependencies
	):
		device     { device.device  },
		renderPass { VK_NULL_HANDLE },

		vkCreateRenderPass
		{
			device.LoadDeviceProcedure<symbol::vkCreateRenderPass>()
		},
		vkDestroyRenderPass
		{
			device.LoadDeviceProcedure<symbol::vkDestroyRenderPass>()
		}
	{
		const auto createInfo = VkRenderPassCreateInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			nullptr,
			0,
			uint32_t (  attachments.size() ),  attachments.data(),
			uint32_t (    subpasses.size() ),    subpasses.data(),
			uint32_t ( dependencies.size() ), dependencies.data(),
		};

		const auto result = vkCreateRenderPass
		(
			device.device,
			&createInfo,
			nullptr,
			&renderPass
		);
		assert(result == VK_SUCCESS);
	}

	RenderPass::~RenderPass()
	{
		if (renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(device, renderPass, nullptr);
		}
	}
}
