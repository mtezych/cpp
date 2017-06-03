
#include <vulkan/Image.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/DeviceMemory.h>

#include <cassert>

namespace vk
{
	Image::CreateInfo::CreateInfo
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
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			flags,
			imageType,
			format,
			extent,
			mipLevels,
			arrayLayers,
			samples,
			tiling,
			usage,
			sharingMode,
			static_cast<uint32_t>(queueFamilyIndices.size()),
			queueFamilyIndices.data(),
			initialLayout,
		}
	{
		if (sharingMode == VK_SHARING_MODE_CONCURRENT)
		{
			assert(queueFamilyIndices.size() == 0);
		}
	}

	Image::Image (const Device& device, const VkImage vkImage)
	:
		device  { &device },
		vkImage { vkImage },
		isWSI   { true }
	{
	}

	Image::Image (const Device& device, const CreateInfo& createInfo)
	:
		device  { &device },
		vkImage { VK_NULL_HANDLE },
		isWSI   { false }
	{
		const auto result = device.vkCreateImage
		(
			device.vkDevice,
			&createInfo.createInfo,
			nullptr,
			&vkImage
		);
		assert(result == VK_SUCCESS);
	}

	Image::~Image ()
	{
		if (vkImage != VK_NULL_HANDLE && !isWSI)
		{
			device->vkDestroyImage(device->vkDevice, vkImage, nullptr);
		}
	}

	Image::Image (Image&& image)
	:
		device  { image.device  },
		vkImage { image.vkImage },
		isWSI   { image.isWSI   }
	{
		image.device  = nullptr;
		image.vkImage = VK_NULL_HANDLE;
		image.isWSI   = false;
	}

	Image& Image::operator = (Image&& image)
	{
		if (vkImage != VK_NULL_HANDLE && !isWSI)
		{
			device->vkDestroyImage(device->vkDevice, vkImage, nullptr);
		}

		device  = image.device;
		vkImage = image.vkImage;
		isWSI   = image.isWSI;

		image.device  = nullptr;
		image.vkImage = VK_NULL_HANDLE;
		image.isWSI   = false;

		return *this;
	}

	void Image::BindDeviceMemory
	(
		const DeviceMemory& deviceMemory,
		const VkDeviceSize  deviceMemoryOffset
	)
	{
		const auto result = device->vkBindImageMemory
		(
			device->vkDevice,
			vkImage,
			deviceMemory.vkDeviceMemory,
			deviceMemoryOffset
		);
		assert(result == VK_SUCCESS);
	}

	VkSubresourceLayout
	Image::GetSubresourceLayout (const VkImageSubresource& subresource) const
	{
		auto subresourceLayout = VkSubresourceLayout { };

		device->vkGetImageSubresourceLayout
		(
			device->vkDevice,
			vkImage,
			&subresource,
			&subresourceLayout
		);

		return subresourceLayout;
	}
}
