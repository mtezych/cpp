
#ifndef VULKAN_INSTANCE
#define VULKAN_INSTANCE

#include "VulkanPhysicalDevice.h"

namespace vk
{
	struct VulkanLoader;

	struct VulkanInstance
	{
		VkInstance instance;

		PFN_vkGetInstanceProcAddr      vkGetInstanceProcAddr;

		PFN_vkCreateInstance           vkCreateInstance;
		PFN_vkDestroyInstance          vkDestroyInstance;
		PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;

		VulkanInstance();

		VulkanInstance
		(
			const VulkanLoader& loader, const VkInstanceCreateInfo& createInfo
		);

		~VulkanInstance();

		VulkanInstance(VulkanInstance&& instance);

		VulkanInstance(const VulkanInstance& instance) = delete;

		VulkanInstance& operator =(VulkanInstance&& instance);

		VulkanInstance& operator =(const VulkanInstance& instance) = delete;

		template <typename Procedure>
		typename Procedure::type LoadInstanceProcedure() const
		{
			const auto procedureAddress = vkGetInstanceProcAddr
			(
				instance, Procedure::name
			);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		std::vector<VulkanPhysicalDevice> EnumeratePhysicalDevices() const;
	};
}

#endif
