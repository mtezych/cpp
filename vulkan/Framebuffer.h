
#ifndef VULKAN_FRAMEBUFFER
#define VULKAN_FRAMEBUFFER

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include "ImageView.h"

#include <vector>

namespace vk
{
	struct Device;
	struct RenderPass;

	struct Framebuffer
	{
		const Device* device;
		VkFramebuffer vkFramebuffer;

		Framebuffer();

		Framebuffer
		(
			const Device&                 device,
			const RenderPass&             renderPass,
			const std::vector<ImageView>& imageViews,
			const VkExtent2D&             size,
			const uint32_t                layers
		);

		~Framebuffer();

		Framebuffer(Framebuffer&& framebuffer);
		Framebuffer(const Framebuffer& framebuffer) = delete;

		Framebuffer& operator =(Framebuffer&& framebuffer);
		Framebuffer& operator =(const Framebuffer& framebuffer) = delete;
	};
}

#endif
