
#ifndef VULKAN_DESCRIPTOR_SET
#define VULKAN_DESCRIPTOR_SET

#include <vulkan/vulkan.h>

namespace vk
{
	struct DescriptorPool;
	struct DescriptorSetLayout;

	struct DescriptorSet
	{
		const DescriptorPool* descriptorPool;
		VkDescriptorSet       vkDescriptorSet;

		DescriptorSet
		(
			const DescriptorPool&      descriptorPool,
			const DescriptorSetLayout& descriptorSetLayout
		);

		~DescriptorSet ();

		DescriptorSet (DescriptorSet&& descriptorSet);
		DescriptorSet (const DescriptorSet& descriptorSet) = delete;

		DescriptorSet& operator = (DescriptorSet&& descriptorSet);
		DescriptorSet& operator = (const DescriptorSet& descriptorSet) = delete;
	};
}

#endif
