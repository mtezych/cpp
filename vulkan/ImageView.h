
#ifndef VULKAN_IMAGE_VIEW
#define VULKAN_IMAGE_VIEW

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct ImageView
	{
		const Device* device;
		VkImageView   vkImageView;

		ImageView
		(
			const Device&                device,
			const VkImageViewCreateInfo& createInfo
		);

		~ImageView();

		ImageView(ImageView&& imageView);
		ImageView(const ImageView& imageView) = delete;

		ImageView& operator =(ImageView&& imageView);
		ImageView& operator =(const ImageView& imageView) = delete;
	};
}

#endif
