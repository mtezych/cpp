
#include <vulkan/Buffer.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/DeviceMemory.h>

#include <cassert>

namespace vk
{
	Buffer::CreateInfo::CreateInfo
	(
		const VkBufferCreateFlags    flags,
		const VkDeviceSize           size,
		const VkBufferUsageFlags     usage,
		const VkSharingMode          sharingMode,
		const std::vector<uint32_t>& queueFamilyIndices
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			flags,
			size,
			usage,
			sharingMode,
			static_cast<uint32_t>(queueFamilyIndices.size()),
			queueFamilyIndices.data(),
		}
	{
		if (sharingMode == VK_SHARING_MODE_CONCURRENT)
		{
			assert(queueFamilyIndices.size() == 0);
		}
	}

	Buffer::Buffer (const Device& device, const CreateInfo& createInfo)
	:
		device   { &device        },
		vkBuffer { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateBuffer
		(
			device.vkDevice,
			&createInfo.createInfo,
			nullptr,
			&vkBuffer
		);
		assert(result == VK_SUCCESS);
	}

	Buffer::~Buffer ()
	{
		if (vkBuffer != VK_NULL_HANDLE)
		{
			device->vkDestroyBuffer(device->vkDevice, vkBuffer, nullptr);
		}
	}

	Buffer::Buffer (Buffer && buffer)
	:
		device   { buffer.device   },
		vkBuffer { buffer.vkBuffer }
	{
		buffer.device   = nullptr;
		buffer.vkBuffer = VK_NULL_HANDLE;
	}

	Buffer& Buffer::operator = (Buffer&& buffer)
	{
		if (vkBuffer != VK_NULL_HANDLE)
		{
			device->vkDestroyBuffer(device->vkDevice, vkBuffer, nullptr);
		}

		device   = buffer.device;
		vkBuffer = buffer.vkBuffer;

		buffer.device   = nullptr;
		buffer.vkBuffer = VK_NULL_HANDLE;

		return *this;
	}

	void Buffer::BindDeviceMemory
	(
		const DeviceMemory &deviceMemory,
		const VkDeviceSize deviceMemoryOffset
	)
	{
		const auto result = device->vkBindBufferMemory
		(
			device->vkDevice,
			vkBuffer,
			deviceMemory.vkDeviceMemory,
			deviceMemoryOffset
		);
		assert(result == VK_SUCCESS);
	}
}
