
#ifndef VULKAN_FRAMEBUFFER
#define VULKAN_FRAMEBUFFER

#include <vulkan/vulkan.h>

#include <vulkan/ImageView.h>

#include <vector>

namespace vk
{
	struct Device;
	struct RenderPass;

	struct Framebuffer
	{
		const Device* device;
		VkFramebuffer vkFramebuffer;

		Framebuffer
		(
			const RenderPass&             renderPass,
			const std::vector<ImageView>& imageViews,
			const VkExtent2D&             size,
			const uint32_t                layers
		);

		~Framebuffer ();

		Framebuffer (Framebuffer&& framebuffer);
		Framebuffer (const Framebuffer& framebuffer) = delete;

		Framebuffer& operator = (Framebuffer&& framebuffer);
		Framebuffer& operator = (const Framebuffer& framebuffer) = delete;
	};
}

#endif
