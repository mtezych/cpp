
#include "VulkanInstance.h"

#include "VulkanSymbols.h"
#include "VulkanLoader.h"

namespace vk
{
	VulkanInstance::VulkanInstance()
	:
		instance { VK_NULL_HANDLE },

		vkGetInstanceProcAddr { nullptr },
		vkCreateInstance      { nullptr },

		vkDestroyInstance          { nullptr },
		vkEnumeratePhysicalDevices { nullptr }
	{
	}

	VulkanInstance::VulkanInstance
	(
		const VulkanLoader& loader, const VkInstanceCreateInfo& createInfo
	):
		instance { VK_NULL_HANDLE },

		vkGetInstanceProcAddr { loader.LoadSymbol         <symbol::vkGetInstanceProcAddr>() },
		vkCreateInstance      { loader.LoadGlobalProcedure<symbol::vkCreateInstance     >() },

		vkDestroyInstance          { nullptr },
		vkEnumeratePhysicalDevices { nullptr }
	{
		const auto result = vkCreateInstance(&createInfo, nullptr, &instance);
		assert(result == VK_SUCCESS);

		vkDestroyInstance          = LoadInstanceProcedure<symbol::vkDestroyInstance>();
		vkEnumeratePhysicalDevices = LoadInstanceProcedure<symbol::vkEnumeratePhysicalDevices>();
	}

	VulkanInstance::~VulkanInstance()
	{
		if (instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(instance, nullptr);
		}
	}

	VulkanInstance::VulkanInstance(VulkanInstance&& instance)
	:
		instance { instance.instance },

		vkGetInstanceProcAddr { instance.vkGetInstanceProcAddr },
		vkCreateInstance      { instance.vkCreateInstance      },

		vkDestroyInstance          { instance.vkDestroyInstance          },
		vkEnumeratePhysicalDevices { instance.vkEnumeratePhysicalDevices }
	{
		instance.instance = VK_NULL_HANDLE;

		instance.vkGetInstanceProcAddr      = nullptr;
		instance.vkCreateInstance           = nullptr;

		instance.vkDestroyInstance          = nullptr;
		instance.vkEnumeratePhysicalDevices = nullptr;
	}

	VulkanInstance& VulkanInstance::operator =(VulkanInstance&& instance)
	{
		if (this->instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(this->instance, nullptr);
		}

		this->instance = instance.instance;

		vkGetInstanceProcAddr = instance.vkGetInstanceProcAddr;
		vkCreateInstance      = instance.vkCreateInstance;

		vkDestroyInstance          = instance.vkDestroyInstance;
		vkEnumeratePhysicalDevices = instance.vkEnumeratePhysicalDevices;

		instance.instance = VK_NULL_HANDLE;

		instance.vkGetInstanceProcAddr      = nullptr;
		instance.vkCreateInstance           = nullptr;

		instance.vkDestroyInstance          = nullptr;
		instance.vkEnumeratePhysicalDevices = nullptr;

		return *this;
	}

	std::vector<VulkanPhysicalDevice>
	VulkanInstance::EnumeratePhysicalDevices() const
	{
		auto physicalDevicesCount = uint32_t { 0 };
		auto result = vkEnumeratePhysicalDevices
		(
			instance, &physicalDevicesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto physicalDevices = std::vector<VkPhysicalDevice>
		(
			physicalDevicesCount, VK_NULL_HANDLE
		);
		result = vkEnumeratePhysicalDevices
		(
			instance,
			&physicalDevicesCount, physicalDevices.data()
		);
		assert(result == VK_SUCCESS);

		auto vulkanPhysicalDevices = std::vector<VulkanPhysicalDevice> { };
		for (const auto physicalDevice : physicalDevices)
		{
			vulkanPhysicalDevices.emplace_back(*this, physicalDevice);
		}
		return vulkanPhysicalDevices;
	}
}
