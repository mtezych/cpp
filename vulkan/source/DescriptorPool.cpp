
#include <vulkan/DescriptorPool.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	DescriptorPool::CreateInfo::CreateInfo
	(
		const VkDescriptorPoolCreateFlags        flags,
		const uint32_t                           maxSets,
		const std::vector<VkDescriptorPoolSize>& poolSize
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			nullptr,
			flags,
			maxSets,
			static_cast<uint32_t>(poolSize.size()), poolSize.data()
		}
	{
		assert(maxSets > 0);
	}

	DescriptorPool::DescriptorPool
	(
		const Device& device, const CreateInfo& createInfo
	):
		device           { &device        },
		vkDescriptorPool { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateDescriptorPool
		(
			device.vkDevice, &createInfo.createInfo, nullptr, &vkDescriptorPool
		);
		assert(result == VK_SUCCESS);
	}

	DescriptorPool::~DescriptorPool ()
	{
		if (vkDescriptorPool != VK_NULL_HANDLE)
		{
			device->vkDestroyDescriptorPool(device->vkDevice, vkDescriptorPool, nullptr);
		}
	}

	DescriptorPool::DescriptorPool (DescriptorPool&& descriptorPool)
	:
		device           { descriptorPool.device           },
		vkDescriptorPool { descriptorPool.vkDescriptorPool }
	{
		descriptorPool.device           = nullptr;
		descriptorPool.vkDescriptorPool = VK_NULL_HANDLE;
	}

	DescriptorPool& DescriptorPool::operator = (DescriptorPool&& descriptorPool)
	{
		if (vkDescriptorPool != VK_NULL_HANDLE)
		{
			device->vkDestroyDescriptorPool(device->vkDevice, vkDescriptorPool, nullptr);
		}

		device           = descriptorPool.device;
		vkDescriptorPool = descriptorPool.vkDescriptorPool;

		descriptorPool.device           = nullptr;
		descriptorPool.vkDescriptorPool = VK_NULL_HANDLE;

		return *this;
	}
}
