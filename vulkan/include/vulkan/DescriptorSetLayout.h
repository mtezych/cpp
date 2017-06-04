
#ifndef VULKAN_DESCRIPTOR_SET_LAYOUT
#define VULKAN_DESCRIPTOR_SET_LAYOUT

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct DescriptorSetLayout
	{
		const Device*         device;
		VkDescriptorSetLayout vkDescriptorSetLayout;

		DescriptorSetLayout
		(
			const Device&                             device,
			std::vector<VkDescriptorSetLayoutBinding> bindings
		);

		~DescriptorSetLayout ();

		DescriptorSetLayout (DescriptorSetLayout&& descriptorSetLayout);
		DescriptorSetLayout (const DescriptorSetLayout& descriptorSetLayout) = delete;

		DescriptorSetLayout& operator = (DescriptorSetLayout&& descriptorSetLayout);
		DescriptorSetLayout& operator = (const DescriptorSetLayout& descriptorSetLayout) = delete;
	};
}

#endif
