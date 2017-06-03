
#include <vulkan/BufferView.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/Buffer.h>

#include <cassert>

namespace vk
{
	BufferView::CreateInfo::CreateInfo
	(
		const VkFormat     format,
		const VkDeviceSize offset,
		const VkDeviceSize range
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			nullptr,
			0,
			VkBuffer { VK_NULL_HANDLE },
			format,
			offset,
			range,
		}
	{
	}

	BufferView::BufferView
	(
		const Device&     device,
		const Buffer&     buffer,
		const CreateInfo& createInfo
	):
		device       { &device        },
		vkBufferView { VK_NULL_HANDLE }
	{
		auto createInfoCopy = createInfo.createInfo;
		createInfoCopy.buffer = buffer.vkBuffer;

		const auto result = device.vkCreateBufferView
		(
			device.vkDevice,
			&createInfoCopy,
			nullptr,
			&vkBufferView
		);
		assert(result == VK_SUCCESS);
	}

	BufferView::~BufferView ()
	{
		if (vkBufferView != VK_NULL_HANDLE)
		{
			device->vkDestroyBufferView(device->vkDevice, vkBufferView, nullptr);
		}
	}

	BufferView::BufferView (BufferView&& bufferView)
	:
		device       { bufferView.device       },
		vkBufferView { bufferView.vkBufferView }
	{
		bufferView.device       = nullptr;
		bufferView.vkBufferView = VK_NULL_HANDLE;
	}

	BufferView& BufferView::operator = (BufferView&& bufferView)
	{
		if (vkBufferView != VK_NULL_HANDLE)
		{
			device->vkDestroyBufferView(device->vkDevice, vkBufferView, nullptr);
		}

		device       = bufferView.device;
		vkBufferView = bufferView.vkBufferView;

		bufferView.device       = nullptr;
		bufferView.vkBufferView = VK_NULL_HANDLE;

		return *this;
	}
}
