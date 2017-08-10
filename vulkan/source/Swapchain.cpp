
#include <vulkan/Swapchain.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/Surface.h>

#include <algorithm>

namespace vk
{
	Swapchain::CreateInfo::CreateInfo
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
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,
			VkSurfaceKHR { VK_NULL_HANDLE },
			minImageCount,
			imageFormat,
			imageColorSpace,
			VkExtent2D { 0, 0 },
			imageArrayLayers,
			imageUsage,
			imageSharingMode,
			static_cast<uint32_t>(queueFamilyIndices.size()),
			(queueFamilyIndices.size() > 0) ? queueFamilyIndices.data() : nullptr,
			preTransform,
			compositeAlpha,
			presentMode,
			clipped,
			oldSwapchain.vkSwapchain,
		}
	{
		if (imageSharingMode == VK_SHARING_MODE_CONCURRENT)
		{
			assert(queueFamilyIndices.size() == 0);
		}
	}

	Swapchain::Swapchain ()
	:
		device      { nullptr },
		vkSwapchain { VK_NULL_HANDLE }
	{
	}

	Swapchain::Swapchain
	(
		const Device&     device,
		const Surface&    surface,
		const CreateInfo& createInfo
	):
		device      { &device },
		vkSwapchain { VK_NULL_HANDLE }
	{
		const auto surfaceCapabilities =
			device.physicalDevice->GetPhysicalDeviceSurfaceCapabilities(surface);

		auto createInfoCopy = createInfo.createInfo;
		createInfoCopy.surface     = surface.vkSurface;
		createInfoCopy.imageExtent = surfaceCapabilities.currentExtent;

		const auto result = device.vkCreateSwapchainKHR
		(
			device.vkDevice,
			&createInfoCopy,
			nullptr,
			&vkSwapchain
		);
		assert(result == VK_SUCCESS);
	}

	Swapchain::~Swapchain ()
	{
		if (vkSwapchain != VK_NULL_HANDLE)
		{
			device->vkDestroySwapchainKHR(device->vkDevice, vkSwapchain, nullptr);
		}
	}

	Swapchain::Swapchain (Swapchain&& swapchain)
	:
		device      { swapchain.device      },
		vkSwapchain { swapchain.vkSwapchain }
	{
		swapchain.device      = nullptr;
		swapchain.vkSwapchain = VK_NULL_HANDLE;
	}

	Swapchain& Swapchain::operator = (Swapchain&& swapchain)
	{
		if (vkSwapchain != VK_NULL_HANDLE)
		{
			device->vkDestroySwapchainKHR(device->vkDevice, vkSwapchain, nullptr);
		}

		device      = swapchain.device;
		vkSwapchain = swapchain.vkSwapchain;

		swapchain.device      = nullptr;
		swapchain.vkSwapchain = VK_NULL_HANDLE;

		return *this;
	}

	std::vector<Image> Swapchain::GetImages () const
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
			imagesCount, VkImage { VK_NULL_HANDLE }
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

	Swapchain::AcquireInfo Swapchain::Acquire () const
	{
		constexpr auto timeout = uint64_t { 0 };

		auto imageIndex = uint32_t { 0 };

		auto imageAvailable = Semaphore { *device };

		const auto result = device->vkAcquireNextImageKHR
		(
			device->vkDevice, vkSwapchain,
			timeout,
			imageAvailable.vkSemaphore,
			VkFence { VK_NULL_HANDLE },
			&imageIndex
		);
		assert(result == VK_SUCCESS);

		return AcquireInfo { imageIndex, std::move(imageAvailable) };
	}
}
