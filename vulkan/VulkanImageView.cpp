
#include "VulkanImageView.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"

#include <cassert>

namespace vk
{
	ImageView::ImageView()
	:
		device    { VK_NULL_HANDLE },
		imageView { VK_NULL_HANDLE },

		vkCreateImageView  { nullptr },
		vkDestroyImageView { nullptr }
	{
	}

	ImageView::operator VkImageView() const
	{
		return imageView;
	}

	ImageView::ImageView
	(
		const Device&                device,
		const VkImageViewCreateInfo& createInfo
	):
		device    { device.device  },
		imageView { VK_NULL_HANDLE },

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
			device.device,
			&createInfo,
			nullptr,
			&imageView
		);
		assert(result == VK_SUCCESS);
	}

	ImageView::~ImageView()
	{
		if (imageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
	}

	ImageView::ImageView(ImageView&& imageView)
	:
		device    { imageView.device    },
		imageView { imageView.imageView },

		vkCreateImageView  { imageView.vkCreateImageView  },
		vkDestroyImageView { imageView.vkDestroyImageView }
	{
		imageView.device    = VK_NULL_HANDLE;
		imageView.imageView = VK_NULL_HANDLE;

		imageView.vkCreateImageView  = nullptr;
		imageView.vkDestroyImageView = nullptr;
	}

	ImageView& ImageView::operator =(ImageView&& imageView)
	{
		if (this->imageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(this->device, this->imageView, nullptr);
		}

		this->device    = imageView.device;
		this->imageView = imageView.imageView;

		this->vkCreateImageView  = imageView.vkCreateImageView;
		this->vkDestroyImageView = imageView.vkDestroyImageView;

		imageView.device    = VK_NULL_HANDLE;
		imageView.imageView = VK_NULL_HANDLE;

		imageView.vkCreateImageView  = nullptr;
		imageView.vkDestroyImageView = nullptr;

		return *this;
	}
}
