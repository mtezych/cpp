
#ifndef VULKAN_IMAGE
#define VULKAN_IMAGE

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct Image
	{
		struct CreateInfo
		{
			VkImageCreateInfo createInfo;

			CreateInfo
			(
				const VkImageCreateFlags     flags,
				const VkImageType            imageType,
				const VkFormat               format,
				const VkExtent3D&            extent,
				const uint32_t               mipLevels,
				const uint32_t               arrayLayers,
				const VkSampleCountFlagBits  samples,
				const VkImageTiling          tiling,
				const VkImageUsageFlags      usage,
				const VkSharingMode          sharingMode,
				const std::vector<uint32_t>& queueFamilyIndices,
				const VkImageLayout          initialLayout
			);
		};

		const Device* device;
		VkImage       vkImage;
		bool          isWSI;

		Image (const Device& device, const VkImage image);

		Image (const Device& device, const CreateInfo& createInfo);

		~Image ();

		Image (Image&& image);
		Image (const Image& image) = delete;

		Image& operator = (Image&& image);
		Image& operator = (const Image& image) = delete;
	};
}

#endif
