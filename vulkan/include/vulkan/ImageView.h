
#ifndef VULKAN_IMAGE_VIEW
#define VULKAN_IMAGE_VIEW

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;
	struct Image;

	struct ImageView
	{
		struct CreateInfo
		{
			VkImageViewCreateInfo createInfo;

			CreateInfo
			(
				const VkImageViewType          type,
				const VkFormat                 format,
				const VkComponentMapping       components,
				const VkImageSubresourceRange& subresourceRange
			);
		};

		const Device* device;
		VkImageView   vkImageView;

		ImageView
		(
			const Device&     device,
			const Image&      image,
			const CreateInfo& createInfo
		);

		~ImageView ();

		ImageView (ImageView&& imageView);
		ImageView (const ImageView& imageView) = delete;

		ImageView& operator = (ImageView&& imageView);
		ImageView& operator = (const ImageView& imageView) = delete;
	};
}

#endif
