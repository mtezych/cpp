
#ifndef VULKAN_COMMAND_POOL
#define VULKAN_COMMAND_POOL

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct CommandPool
	{
		const Device* device;
		VkCommandPool vkCommandPool;

		CommandPool (const Device& device, const uint32_t queueFamilyIndex);

		~CommandPool ();

		CommandPool (CommandPool&& commandPool);
		CommandPool (const CommandPool& commandPool) = delete;

		CommandPool& operator = (CommandPool&& commandPool);
		CommandPool& operator = (const CommandPool& commandPool) = delete;
	};
}

#endif
