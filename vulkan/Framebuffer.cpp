
#include "Framebuffer.h"

#include "Symbols.h"
#include "Device.h"
#include "RenderPass.h"

#include <cassert>
#include <algorithm>

namespace vk
{
	Framebuffer::Framebuffer()
	:
		device      { VK_NULL_HANDLE },
		framebuffer { VK_NULL_HANDLE },

		vkCreateFramebuffer  { nullptr },
		vkDestroyFramebuffer { nullptr }
	{
	}

	Framebuffer::Framebuffer
	(
		const Device&                 device,
		const RenderPass&             renderPass,
		const std::vector<ImageView>& imageViews,
		const VkExtent2D&             size,
		const uint32_t                layers
	):
		device      { device.device  },
		framebuffer { VK_NULL_HANDLE },

		vkCreateFramebuffer
		{
			device.LoadDeviceProcedure<symbol::vkCreateFramebuffer>()
		},
		vkDestroyFramebuffer
		{
			device.LoadDeviceProcedure<symbol::vkDestroyFramebuffer>()
		}
	{
		auto imageViewHandles = std::vector<VkImageView>
		(
			imageViews.size(), VK_NULL_HANDLE
		);
		std::copy(imageViews.begin(), imageViews.end(), imageViewHandles.begin());

		const auto createInfo = VkFramebufferCreateInfo
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			nullptr,
			0,
			renderPass.renderPass,
			uint32_t ( imageViewHandles.size() ),
			imageViewHandles.data(),
			size.width, size.height, layers
		};

		const auto result = vkCreateFramebuffer
		(
			device.device,
			&createInfo,
			nullptr,
			&framebuffer
		);
		assert(result == VK_SUCCESS);
	}

	Framebuffer::~Framebuffer()
	{
		if (framebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}

	Framebuffer::Framebuffer(Framebuffer&& framebuffer)
	:
		device      { framebuffer.device      },
		framebuffer { framebuffer.framebuffer },

		vkCreateFramebuffer  { framebuffer.vkCreateFramebuffer  },
		vkDestroyFramebuffer { framebuffer.vkDestroyFramebuffer }
	{
		framebuffer.device      = VK_NULL_HANDLE;
		framebuffer.framebuffer = VK_NULL_HANDLE;

		framebuffer.vkCreateFramebuffer  = nullptr;
		framebuffer.vkDestroyFramebuffer = nullptr;
	}

	Framebuffer& Framebuffer::operator =(Framebuffer&& framebuffer)
	{
		if (this->framebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(this->device, this->framebuffer, nullptr);
		}

		this->device      = framebuffer.device;
		this->framebuffer = framebuffer.framebuffer;

		this->vkCreateFramebuffer  = framebuffer.vkCreateFramebuffer;
		this->vkDestroyFramebuffer = framebuffer.vkDestroyFramebuffer;

		framebuffer.device      = VK_NULL_HANDLE;
		framebuffer.framebuffer = VK_NULL_HANDLE;

		framebuffer.vkCreateFramebuffer  = nullptr;
		framebuffer.vkDestroyFramebuffer = nullptr;

		return *this;
	}
}
