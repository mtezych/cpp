
#include "Swapchain.h"

#include "Symbols.h"
#include "Device.h"

namespace vk
{
	Swapchain::Swapchain
	(
		const Device&                   device,
		const VkSwapchainCreateInfoKHR& swapchainCreateInfo
	):
		device      { &device },
		vkSwapchain { VK_NULL_HANDLE },

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
			vkDestroySwapchainKHR(device->vkDevice, vkSwapchain, nullptr);
		}
	}

	std::vector<VkImage> Swapchain::Images() const
	{
		auto swapchainImagesCount = uint32_t { 0 };
		auto result = vkGetSwapchainImagesKHR
		(
			device->vkDevice, vkSwapchain,
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
			device->vkDevice, vkSwapchain,
			&swapchainImagesCount, swapchainImages.data()
		);
		assert(result == VK_SUCCESS);

		return swapchainImages;
	}

	Swapchain::AcquireInfo Swapchain::Acquire() const
	{
		auto imageIndex = uint32_t { 0 };
		constexpr auto timeout = uint64_t { 0 };

		auto imageAvailable = Semaphore { *device };

		const auto result = vkAcquireNextImageKHR
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
