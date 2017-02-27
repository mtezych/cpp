
#ifndef VULKAN_DEVICE
#define VULKAN_DEVICE

#include "VulkanQueue.h"

#include <cassert>

namespace vk
{
	struct VulkanInstance;
	struct VulkanPhysicalDevice;

	struct VulkanDevice
	{
		VkDevice device;

		PFN_vkCreateDevice      vkCreateDevice;
		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

		PFN_vkGetDeviceQueue    vkGetDeviceQueue;
		PFN_vkDeviceWaitIdle    vkDeviceWaitIdle;
		PFN_vkDestroyDevice     vkDestroyDevice;

		VulkanDevice
		(
			const VulkanInstance&       instance,
			const VulkanPhysicalDevice& physicalDevice,
			const VkDeviceCreateInfo&   deviceCreateInfo
		);

		~VulkanDevice();

		template <typename Procedure>
		typename Procedure::type LoadDeviceProcedure() const
		{
			const auto procedureAddress = vkGetDeviceProcAddr
			(
				device, Procedure::name
			);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		VulkanQueue
		Queues(const uint32_t familyIndex, const uint32_t queueIndex) const;
	};
}

#endif
