
#ifndef VULKAN_INSTANCE
#define VULKAN_INSTANCE

#include "PhysicalDevice.h"

namespace vk
{
	struct Loader;

	struct Instance
	{
		VkInstance instance;

		PFN_vkGetInstanceProcAddr      vkGetInstanceProcAddr;

		PFN_vkCreateInstance           vkCreateInstance;
		PFN_vkDestroyInstance          vkDestroyInstance;
		PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;

		Instance();

		Instance
		(
			const Loader& loader, const VkInstanceCreateInfo& createInfo
		);

		~Instance();

		Instance(Instance&& instance);

		Instance(const Instance& instance) = delete;

		Instance& operator =(Instance&& instance);

		Instance& operator =(const Instance& instance) = delete;

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

		std::vector<PhysicalDevice> EnumeratePhysicalDevices() const;
	};
}

#endif
