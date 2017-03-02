
#ifndef VULKAN_IMAGE_VIEW
#define VULKAN_IMAGE_VIEW

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanDevice;

	struct VulkanImageView
	{
		VkDevice     device;
		VkImageView  imageView;

		PFN_vkCreateImageView  vkCreateImageView;
		PFN_vkDestroyImageView vkDestroyImageView;

		VulkanImageView();

		operator VkImageView() const;

		VulkanImageView
		(
			const VulkanDevice&          device,
			const VkImageViewCreateInfo& createInfo
		);

		~VulkanImageView();

		VulkanImageView(VulkanImageView&& imageView);
		VulkanImageView(const VulkanImageView& imageView) = delete;

		VulkanImageView& operator =(VulkanImageView&& imageView);
		VulkanImageView& operator =(const VulkanImageView& imageView) = delete;
	};
}

#endif
