
#include <vulkan/DescriptorSet.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/DescriptorPool.h>
#include <vulkan/DescriptorSetLayout.h>

#include <cassert>

namespace vk
{
	DescriptorSet::DescriptorSet
	(
		const DescriptorPool&      descriptorPool,
		const DescriptorSetLayout& descriptorSetLayout
	):
		descriptorPool  { &descriptorPool },
		vkDescriptorSet { VK_NULL_HANDLE  }
	{
		const auto allocateInfo = VkDescriptorSetAllocateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			descriptorPool.vkDescriptorPool,
			1,
			&descriptorSetLayout.vkDescriptorSetLayout,
		};
		const auto result = descriptorPool.device->vkAllocateDescriptorSets
		(
			descriptorPool.device->vkDevice,
			&allocateInfo,
			&vkDescriptorSet
		);
		assert(result == VK_SUCCESS);
	}

	DescriptorSet::~DescriptorSet ()
	{
		if (vkDescriptorSet != VK_NULL_HANDLE)
		{
			descriptorPool->device->vkFreeDescriptorSets
			(
				descriptorPool->device->vkDevice,
				descriptorPool->vkDescriptorPool,
				1, &vkDescriptorSet
			);
		}
	}

	DescriptorSet::DescriptorSet (DescriptorSet&& descriptorSet)
	:
		descriptorPool  { descriptorSet.descriptorPool  },
		vkDescriptorSet { descriptorSet.vkDescriptorSet }
	{
		descriptorSet.descriptorPool  = nullptr;
		descriptorSet.vkDescriptorSet = VK_NULL_HANDLE;
	}

	DescriptorSet& DescriptorSet::operator = (DescriptorSet&& descriptorSet)
	{
		if (vkDescriptorSet != VK_NULL_HANDLE)
		{
			descriptorPool->device->vkFreeDescriptorSets
			(
				descriptorPool->device->vkDevice,
				descriptorPool->vkDescriptorPool,
				1, &vkDescriptorSet
			);
		}

		descriptorPool  = descriptorSet.descriptorPool;
		vkDescriptorSet = descriptorSet.vkDescriptorSet;

		descriptorSet.descriptorPool  = nullptr;
		descriptorSet.vkDescriptorSet = VK_NULL_HANDLE;

		return *this;
	}
}
