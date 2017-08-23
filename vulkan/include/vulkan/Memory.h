
#ifndef VULKAN_MEMORY
#define VULKAN_MEMORY

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct Memory
	{
		struct CreateInfo
		{
			VkMemoryAllocateInfo createInfo;

			CreateInfo
			(
				const VkDeviceSize allocationSize,
				const uint32_t     memoryTypeIndex
			);
		};

		const Device*  device;
		VkDeviceMemory vkMemory;

		Memory (const Device& device, const CreateInfo& createInfo);

		~Memory ();

		Memory (Memory&& memory);
		Memory (const Memory& memory) = delete;

		Memory& operator = (Memory&& memory);
		Memory& operator = (const Memory& memory) = delete;
	};
}

#endif
