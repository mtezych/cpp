
#include "ImageView.h"

#include "Symbols.h"
#include "Device.h"

#include <cassert>

namespace vk
{
	ImageView::ImageView()
	:
		device      { nullptr },
		vkImageView { VK_NULL_HANDLE },

		vkCreateImageView  { nullptr },
		vkDestroyImageView { nullptr }
	{
	}

	ImageView::operator VkImageView() const
	{
		return vkImageView;
	}

	ImageView::ImageView
	(
		const Device&                device,
		const VkImageViewCreateInfo& createInfo
	):
		device      { &device },
		vkImageView { VK_NULL_HANDLE },

		vkCreateImageView
		{
			device.LoadDeviceProcedure<symbol::vkCreateImageView>()
		},
		vkDestroyImageView
		{
			device.LoadDeviceProcedure<symbol::vkDestroyImageView>()
		}
	{
		const auto result = vkCreateImageView
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
			vkDestroyImageView(device->vkDevice, vkImageView, nullptr);
		}
	}

	ImageView::ImageView(ImageView&& imageView)
	:
		device      { imageView.device      },
		vkImageView { imageView.vkImageView },

		vkCreateImageView  { imageView.vkCreateImageView  },
		vkDestroyImageView { imageView.vkDestroyImageView }
	{
		imageView.device      = nullptr;
		imageView.vkImageView = VK_NULL_HANDLE;

		imageView.vkCreateImageView  = nullptr;
		imageView.vkDestroyImageView = nullptr;
	}

	ImageView& ImageView::operator =(ImageView&& imageView)
	{
		if (vkImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(device->vkDevice, vkImageView, nullptr);
		}

		device      = imageView.device;
		vkImageView = imageView.vkImageView;

		vkCreateImageView  = imageView.vkCreateImageView;
		vkDestroyImageView = imageView.vkDestroyImageView;

		imageView.device      = nullptr;
		imageView.vkImageView = VK_NULL_HANDLE;

		imageView.vkCreateImageView  = nullptr;
		imageView.vkDestroyImageView = nullptr;

		return *this;
	}
}
