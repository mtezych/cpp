
#include <vulkan/RenderPass.h>

#include <vulkan/Device.h>

namespace vk
{
	RenderPass::RenderPass
	(
		const Device&                               device,
		const std::vector<VkAttachmentDescription>& attachments,
		const std::vector<VkSubpassDescription>&    subpasses,
		const std::vector<VkSubpassDependency>&     dependencies
	):
		device       { &device },
		vkRenderPass { VK_NULL_HANDLE }
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

		const auto result = device.vkCreateRenderPass
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkRenderPass
		);
		assert(result == VK_SUCCESS);
	}

	RenderPass::~RenderPass ()
	{
		if (vkRenderPass != VK_NULL_HANDLE)
		{
			device->vkDestroyRenderPass(device->vkDevice, vkRenderPass, nullptr);
		}
	}
}
