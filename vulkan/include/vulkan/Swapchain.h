
#ifndef VULKAN_SWAPCHAIN
#define VULKAN_SWAPCHAIN

#include <vulkan/Semaphore.h>
#include <vulkan/Image.h>

#include <vector>

namespace vk
{
	struct Device;
	struct Surface;

	struct Swapchain
	{
		struct CreateInfo
		{
			VkSwapchainCreateInfoKHR createInfo;

			CreateInfo
			(
				const uint32_t                      minImageCount,
				const VkFormat                      imageFormat,
				const VkColorSpaceKHR               imageColorSpace,
				const uint32_t                      imageArrayLayers,
				const VkImageUsageFlags             imageUsage,
				const VkSharingMode                 imageSharingMode,
				const std::vector<uint32_t>&        queueFamilyIndices,
				const VkSurfaceTransformFlagBitsKHR preTransform,
				const VkCompositeAlphaFlagBitsKHR   compositeAlpha,
				const VkPresentModeKHR              presentMode,
				const VkBool32                      clipped,
				const Swapchain&                    oldSwapchain
			);
		};

		const Device*  device;
		VkSwapchainKHR vkSwapchain;

		Swapchain ();

		Swapchain
		(
			const Device&     device,
			const Surface&    surface,
			const CreateInfo& createInfo
		);

		~Swapchain();

		std::vector<Image> Images() const;

		struct AcquireInfo
		{
			uint32_t  imageIndex;
			Semaphore imageAvailable;
		};
		AcquireInfo Acquire() const;
	};
}

#endif
