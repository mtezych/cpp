
#include <vulkan/ImageView.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/Image.h>

#include <cassert>

namespace vk
{
	ImageView::CreateInfo::CreateInfo
	(
		const VkImageViewType          type,
		const VkFormat                 format,
		const VkComponentMapping       components,
		const VkImageSubresourceRange& subresourceRange
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			VkImage { VK_NULL_HANDLE },
			type,
			format,
			components,
			subresourceRange,
		}
	{
	}

	ImageView::ImageView
	(
		const Device&     device,
		const Image&      image,
		const CreateInfo& createInfo
	):
		device      { &device        },
		vkImageView { VK_NULL_HANDLE }
	{
		auto createInfoCopy = createInfo.createInfo;
		createInfoCopy.image = image.vkImage;

		const auto result = device.vkCreateImageView
		(
			device.vkDevice,
			&createInfoCopy,
			nullptr,
			&vkImageView
		);
		assert(result == VK_SUCCESS);
	}

	ImageView::~ImageView ()
	{
		if (vkImageView != VK_NULL_HANDLE)
		{
			device->vkDestroyImageView(device->vkDevice, vkImageView, nullptr);
		}
	}

	ImageView::ImageView (ImageView&& imageView)
	:
		device      { imageView.device      },
		vkImageView { imageView.vkImageView }
	{
		imageView.device      = nullptr;
		imageView.vkImageView = VK_NULL_HANDLE;
	}

	ImageView& ImageView::operator = (ImageView&& imageView)
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
