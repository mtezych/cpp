
#include <vulkan/Memory.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	Memory::CreateInfo::CreateInfo
	(
		const VkDeviceSize allocationSize,
		const uint32_t     memoryTypeIndex
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			allocationSize,
			memoryTypeIndex,
		}
	{
	}

	Memory::Memory (const Device& device, const CreateInfo& createInfo)
	:
		device   { &device        },
		vkMemory { VK_NULL_HANDLE }
	{
		const auto result = device.vkAllocateMemory
		(
			device.vkDevice,
			&createInfo.createInfo,
			nullptr,
			&vkMemory
		);
		assert(result == VK_SUCCESS);
	}

	Memory::~Memory ()
	{
		if (vkMemory != VK_NULL_HANDLE)
		{
			device->vkFreeMemory(device->vkDevice, vkMemory, nullptr);
		}
	}

	Memory::Memory (Memory&& memory)
	:
		device   { memory.device   },
		vkMemory { memory.vkMemory }
	{
		memory.device   = nullptr;
		memory.vkMemory = VK_NULL_HANDLE;
	}

	Memory& Memory::operator = (Memory&& memory)
	{
		if (vkMemory != VK_NULL_HANDLE)
		{
			device->vkFreeMemory(device->vkDevice, vkMemory, nullptr);
		}

		device   = memory.device;
		vkMemory = memory.vkMemory;

		memory.device   = nullptr;
		memory.vkMemory = VK_NULL_HANDLE;

		return *this;
	}
}
