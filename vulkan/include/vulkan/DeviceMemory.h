
#ifndef VULKAN_DEVICE_MEMORY
#define VULKAN_DEVICE_MEMORY

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct DeviceMemory
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
		VkDeviceMemory vkDeviceMemory;

		DeviceMemory (const Device& device, const CreateInfo& createInfo);

		~DeviceMemory ();

		DeviceMemory (DeviceMemory&& deviceMemory);
		DeviceMemory (const DeviceMemory& deviceMemory) = delete;

		DeviceMemory& operator = (DeviceMemory&& deviceMemory);
		DeviceMemory& operator = (const DeviceMemory& deviceMemory) = delete;
	};
}

#endif
