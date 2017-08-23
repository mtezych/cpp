
#ifndef VULKAN_BUFFER
#define VULKAN_BUFFER

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;
	struct Memory;

	struct Buffer
	{
		struct CreateInfo
		{
			VkBufferCreateInfo createInfo;

			CreateInfo
			(
				const VkBufferCreateFlags    flags,
				const VkDeviceSize           size,
				const VkBufferUsageFlags     usage,
				const VkSharingMode          sharingMode,
				const std::vector<uint32_t>& queueFamilyIndices
			);
		};

		const Device* device;
		VkBuffer      vkBuffer;

		Buffer (const Device& device, const CreateInfo& createInfo);

		~Buffer ();

		Buffer (Buffer&& buffer);
		Buffer (const Buffer& buffer) = delete;

		Buffer& operator = (Buffer&& buffer);
		Buffer& operator = (const Buffer& buffer) = delete;

		void BindMemory (const Memory& memory, const VkDeviceSize offset);
	};
}

#endif
