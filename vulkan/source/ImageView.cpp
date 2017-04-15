
#include <vulkan/ImageView.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

#include <cassert>

namespace vk
{
	ImageView::ImageView
	(
		const Device&                device,
		const VkImageViewCreateInfo& createInfo
	):
		device      { &device },
		vkImageView { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateImageView
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkImageView
		);
		assert(result == VK_SUCCESS);
	}

	ImageView::~ImageView()
	{
		if (vkImageView != VK_NULL_HANDLE)
		{
			device->vkDestroyImageView(device->vkDevice, vkImageView, nullptr);
		}
	}

	ImageView::ImageView(ImageView&& imageView)
	:
		device      { imageView.device      },
		vkImageView { imageView.vkImageView }
	{
		imageView.device      = nullptr;
		imageView.vkImageView = VK_NULL_HANDLE;
	}

	ImageView& ImageView::operator =(ImageView&& imageView)
	{
		if (vkImageView != VK_NULL_HANDLE)
		{
			device->vkDestroyImageView(device->vkDevice, vkImageView, nullptr);
		}

		device      = imageView.device;
		vkImageView = imageView.vkImageView;

		imageView.device      = nullptr;
		imageView.vkImageView = VK_NULL_HANDLE;

		return *this;
	}
}
