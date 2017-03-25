
#include "VulkanSwapchain.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"

namespace vk
{
	Swapchain::Swapchain
	(
		const Device&                   device,
		const VkSwapchainCreateInfoKHR& swapchainCreateInfo
	):
		swapchain { VK_NULL_HANDLE },
		device    { device },

		vkCreateSwapchainKHR
		{
			device.LoadDeviceProcedure<symbol::vkCreateSwapchainKHR >()
		},
		vkDestroySwapchainKHR
		{
			device.LoadDeviceProcedure<symbol::vkDestroySwapchainKHR>()
		},
		vkGetSwapchainImagesKHR
		{
			device.LoadDeviceProcedure<symbol::vkGetSwapchainImagesKHR>()
		},
		vkAcquireNextImageKHR
		{
			device.LoadDeviceProcedure<symbol::vkAcquireNextImageKHR>()
		}
	{
		const auto result = vkCreateSwapchainKHR
		(
			device.device,
			&swapchainCreateInfo,
			nullptr,
			&swapchain
		);
		assert(result == VK_SUCCESS);
	}

	Swapchain::~Swapchain()
	{
		if (swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(device.device, swapchain, nullptr);
		}
	}

	std::vector<VkImage> Swapchain::Images() const
	{
		auto swapchainImagesCount = uint32_t { 0 };
		auto result = vkGetSwapchainImagesKHR
		(
			device.device, swapchain,
			&swapchainImagesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto swapchainImages = std::vector<VkImage>
		(
			swapchainImagesCount,
			VkImage { VK_NULL_HANDLE }
		);
		result = vkGetSwapchainImagesKHR
		(
			device.device, swapchain,
			&swapchainImagesCount, swapchainImages.data()
		);
		assert(result == VK_SUCCESS);

		return swapchainImages;
	}

	Swapchain::AcquireInfo Swapchain::Acquire() const
	{
		auto imageIndex = uint32_t { 0 };
		constexpr auto timeout = uint64_t { 0 };

		auto imageAvailable = Semaphore { device };

		const auto result = vkAcquireNextImageKHR
		(
			device.device, swapchain,
			timeout,
			VkSemaphore { imageAvailable.semaphore },
			VkFence { VK_NULL_HANDLE },
			&imageIndex
		);
		assert(result == VK_SUCCESS);

		return AcquireInfo { imageIndex, std::move(imageAvailable) };
	}
}