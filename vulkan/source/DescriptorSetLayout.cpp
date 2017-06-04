
#include <vulkan/DescriptorSetLayout.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

#include <cassert>

namespace vk
{
	DescriptorSetLayout::DescriptorSetLayout
	(
		const Device&                             device,
		std::vector<VkDescriptorSetLayoutBinding> bindings
	):
		device                { &device        },
		vkDescriptorSetLayout { VK_NULL_HANDLE }
	{
		const auto createInfo = VkDescriptorSetLayoutCreateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			nullptr,
			0,
			uint32_t ( bindings.size() ), bindings.data(),
		};
		const auto result = device.vkCreateDescriptorSetLayout
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkDescriptorSetLayout
		);
		assert(result == VK_SUCCESS);
	}

	DescriptorSetLayout::~DescriptorSetLayout ()
	{
		if (vkDescriptorSetLayout != VK_NULL_HANDLE)
		{
			device->vkDestroyDescriptorSetLayout
			(
				device->vkDevice,
				vkDescriptorSetLayout,
				nullptr
			);
		}
	}

	DescriptorSetLayout::DescriptorSetLayout (DescriptorSetLayout&& descriptorSetLayout)
	:
		device                { descriptorSetLayout.device                },
		vkDescriptorSetLayout { descriptorSetLayout.vkDescriptorSetLayout }
	{
		descriptorSetLayout.device                = nullptr;
		descriptorSetLayout.vkDescriptorSetLayout = VK_NULL_HANDLE;
	}

	DescriptorSetLayout&
	DescriptorSetLayout::operator = (DescriptorSetLayout&& descriptorSetLayout)
	{
		if (vkDescriptorSetLayout != VK_NULL_HANDLE)
		{
			device->vkDestroyDescriptorSetLayout
			(
				device->vkDevice,
				vkDescriptorSetLayout,
				nullptr
			);
		}

		device                = descriptorSetLayout.device;
		vkDescriptorSetLayout = descriptorSetLayout.vkDescriptorSetLayout;

		descriptorSetLayout.device                = nullptr;
		descriptorSetLayout.vkDescriptorSetLayout = VK_NULL_HANDLE;

		return *this;
	}
}
