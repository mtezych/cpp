
#ifndef VULKAN_IMAGE
#define VULKAN_IMAGE

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct Image
	{
		const Device* device;
		VkImage       vkImage;
		bool          isWSI;

		Image (const Device& device, const VkImage image);

		Image
		(
			const Device&            device,
			const VkImageCreateInfo& createInfo
		);

		~Image ();

		Image (Image&& image);
		Image (const Image& image) = delete;

		Image& operator = (Image&& image);
		Image& operator = (const Image& image) = delete;
	};
}

#endif
