
#include <vulkan/Swapchain.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

#include <algorithm>

namespace vk
{
	Swapchain::Swapchain
	(
		const Device&                   device,
		const VkSwapchainCreateInfoKHR& swapchainCreateInfo
	):
		device      { &device },
		vkSwapchain { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateSwapchainKHR
		(
			device.vkDevice,
			&swapchainCreateInfo,
			nullptr,
			&vkSwapchain
		);
		assert(result == VK_SUCCESS);
	}

	Swapchain::~Swapchain()
	{
		if (vkSwapchain != VK_NULL_HANDLE)
		{
			device->vkDestroySwapchainKHR(device->vkDevice, vkSwapchain, nullptr);
		}
	}

	std::vector<Image> Swapchain::Images() const
	{
		auto imagesCount = uint32_t { 0 };
		auto result = device->vkGetSwapchainImagesKHR
		(
			device->vkDevice, vkSwapchain,
			&imagesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto vkImages = std::vector<VkImage>
		(
			imagesCount,
			VkImage { VK_NULL_HANDLE }
		);
		result = device->vkGetSwapchainImagesKHR
		(
			device->vkDevice, vkSwapchain,
			&imagesCount, vkImages.data()
		);
		assert(result == VK_SUCCESS);

		auto images = std::vector<Image> { };
		images.reserve(imagesCount);
		for (const auto vkImage : vkImages)
		{
			images.emplace_back(*device, vkImage);
		}

		return images;
	}

	Swapchain::AcquireInfo Swapchain::Acquire() const
	{
		constexpr auto timeout = uint64_t { 0 };

		auto imageIndex = uint32_t { 0 };

		auto imageAvailable = Semaphore { *device };

		const auto result = device->vkAcquireNextImageKHR
		(
			device->vkDevice, vkSwapchain,
			timeout,
			VkSemaphore { imageAvailable.vkSemaphore },
			VkFence { VK_NULL_HANDLE },
			&imageIndex
		);
		assert(result == VK_SUCCESS);

		return AcquireInfo { imageIndex, std::move(imageAvailable) };
	}
}
