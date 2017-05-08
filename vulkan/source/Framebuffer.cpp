
#include <vulkan/Framebuffer.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/RenderPass.h>

#include <cassert>
#include <algorithm>

namespace vk
{
	Framebuffer::Framebuffer
	(
		const Device&                 device,
		const RenderPass&             renderPass,
		const std::vector<ImageView>& imageViews,
		const VkExtent2D&             size,
		const uint32_t                layers
	):
		device        { &device },
		vkFramebuffer { VK_NULL_HANDLE }
	{
		auto vkImageViews = std::vector<VkImageView>
		(
			imageViews.size(), VK_NULL_HANDLE
		);
		std::transform
		(
			imageViews.begin(), imageViews.end(), vkImageViews.begin(),

			[](const ImageView& imageView) -> VkImageView
			{
				return imageView.vkImageView;
			}
		);

		const auto createInfo = VkFramebufferCreateInfo
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			nullptr,
			0,
			renderPass.vkRenderPass,
			uint32_t ( vkImageViews.size() ), vkImageViews.data(),
			size.width, size.height, layers
		};

		const auto result = device.vkCreateFramebuffer
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkFramebuffer
		);
		assert(result == VK_SUCCESS);
	}

	Framebuffer::~Framebuffer ()
	{
		if (vkFramebuffer != VK_NULL_HANDLE)
		{
			device->vkDestroyFramebuffer(device->vkDevice, vkFramebuffer, nullptr);
		}
	}

	Framebuffer::Framebuffer (Framebuffer&& framebuffer)
	:
		device        { framebuffer.device        },
		vkFramebuffer { framebuffer.vkFramebuffer }
	{
		framebuffer.device        = nullptr;
		framebuffer.vkFramebuffer = VK_NULL_HANDLE;
	}

	Framebuffer& Framebuffer::operator = (Framebuffer&& framebuffer)
	{
		if (vkFramebuffer != VK_NULL_HANDLE)
		{
			device->vkDestroyFramebuffer(device->vkDevice, vkFramebuffer, nullptr);
		}

		device        = framebuffer.device;
		vkFramebuffer = framebuffer.vkFramebuffer;

		framebuffer.device        = nullptr;
		framebuffer.vkFramebuffer = VK_NULL_HANDLE;

		return *this;
	}
}
