
#ifndef VULKAN_DESCRIPTOR_POOL
#define VULKAN_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct DescriptorPool
	{
		struct CreateInfo
		{
			VkDescriptorPoolCreateInfo createInfo;

			CreateInfo
			(
				const VkDescriptorPoolCreateFlags        flags,
				const uint32_t                           maxSets,
				const std::vector<VkDescriptorPoolSize>& poolSize
			);
		};

		const Device*    device;
		VkDescriptorPool vkDescriptorPool;

		DescriptorPool (const Device& device, const CreateInfo& createInfo);

		~DescriptorPool ();

		DescriptorPool (DescriptorPool&& descriptorPool);
		DescriptorPool (const DescriptorPool& descriptorPool) = delete;

		DescriptorPool& operator = (DescriptorPool&& descriptorPool);
		DescriptorPool& operator = (const DescriptorPool& descriptorPool) = delete;
	};
}

#endif
