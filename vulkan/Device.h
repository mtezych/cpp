
#ifndef VULKAN_DEVICE
#define VULKAN_DEVICE

#include "Queue.h"

#include <cassert>

namespace vk
{
	struct Instance;
	struct PhysicalDevice;

	struct Device
	{
		VkDevice device;

		PFN_vkCreateDevice      vkCreateDevice;
		PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

		PFN_vkGetDeviceQueue    vkGetDeviceQueue;
		PFN_vkDeviceWaitIdle    vkDeviceWaitIdle;
		PFN_vkDestroyDevice     vkDestroyDevice;

		Device
		(
			const Instance&           instance,
			const PhysicalDevice&     physicalDevice,
			const VkDeviceCreateInfo& deviceCreateInfo
		);

		~Device();

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

		Queue
		Queues(const uint32_t familyIndex, const uint32_t queueIndex) const;
	};
}

#endif
