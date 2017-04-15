
#include <vulkan/Image.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

#include <cassert>

namespace vk
{
	Image::Image (const Device& device, const VkImage vkImage)
	:
		device  { &device },
		vkImage { vkImage },
		isWSI   { true }
	{
	}

	Image::Image (const Device& device, const VkImageCreateInfo& createInfo)
	:
		device  { &device },
		vkImage { VK_NULL_HANDLE },
		isWSI   { false }
	{
		const auto result = device.vkCreateImage
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkImage
		);
		assert(result == VK_SUCCESS);
	}

	Image::~Image()
	{
		if (vkImage != VK_NULL_HANDLE && !isWSI)
		{
			device->vkDestroyImage(device->vkDevice, vkImage, nullptr);
		}
	}

	Image::Image(Image&& image)
	:
		device  { image.device  },
		vkImage { image.vkImage },
		isWSI   { image.isWSI   }
	{
		image.device  = nullptr;
		image.vkImage = VK_NULL_HANDLE;
		image.isWSI   = false;
	}

	Image& Image::operator =(Image&& image)
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
}
