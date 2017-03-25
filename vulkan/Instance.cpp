
#include "Instance.h"

#include "Symbols.h"
#include "Loader.h"

namespace vk
{
	Instance::Instance()
	:
		vkInstance { VK_NULL_HANDLE },

		vkGetInstanceProcAddr { nullptr },
		vkCreateInstance      { nullptr },

		vkDestroyInstance          { nullptr },
		vkEnumeratePhysicalDevices { nullptr }
	{
	}

	Instance::Instance
	(
		const Loader& loader, const VkInstanceCreateInfo& createInfo
	):
		vkInstance { VK_NULL_HANDLE },

		vkGetInstanceProcAddr { loader.LoadSymbol         <symbol::vkGetInstanceProcAddr>() },
		vkCreateInstance      { loader.LoadGlobalProcedure<symbol::vkCreateInstance     >() },

		vkDestroyInstance          { nullptr },
		vkEnumeratePhysicalDevices { nullptr }
	{
		const auto result = vkCreateInstance(&createInfo, nullptr, &vkInstance);
		assert(result == VK_SUCCESS);

		vkDestroyInstance          = LoadInstanceProcedure<symbol::vkDestroyInstance>();
		vkEnumeratePhysicalDevices = LoadInstanceProcedure<symbol::vkEnumeratePhysicalDevices>();
	}

	Instance::~Instance()
	{
		if (vkInstance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(vkInstance, nullptr);
		}
	}

	Instance::Instance(Instance&& instance)
	:
		vkInstance { instance.vkInstance },

		vkGetInstanceProcAddr { instance.vkGetInstanceProcAddr },
		vkCreateInstance      { instance.vkCreateInstance      },

		vkDestroyInstance          { instance.vkDestroyInstance          },
		vkEnumeratePhysicalDevices { instance.vkEnumeratePhysicalDevices }
	{
		instance.vkInstance = VK_NULL_HANDLE;

		instance.vkGetInstanceProcAddr      = nullptr;
		instance.vkCreateInstance           = nullptr;

		instance.vkDestroyInstance          = nullptr;
		instance.vkEnumeratePhysicalDevices = nullptr;
	}

	Instance& Instance::operator =(Instance&& instance)
	{
		if (vkInstance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(vkInstance, nullptr);
		}

		vkInstance = instance.vkInstance;

		vkGetInstanceProcAddr = instance.vkGetInstanceProcAddr;
		vkCreateInstance      = instance.vkCreateInstance;

		vkDestroyInstance          = instance.vkDestroyInstance;
		vkEnumeratePhysicalDevices = instance.vkEnumeratePhysicalDevices;

		instance.vkInstance = VK_NULL_HANDLE;

		instance.vkGetInstanceProcAddr      = nullptr;
		instance.vkCreateInstance           = nullptr;

		instance.vkDestroyInstance          = nullptr;
		instance.vkEnumeratePhysicalDevices = nullptr;

		return *this;
	}

	std::vector<PhysicalDevice> Instance::EnumeratePhysicalDevices() const
	{
		auto physicalDevicesCount = uint32_t { 0 };
		auto result = vkEnumeratePhysicalDevices
		(
			vkInstance, &physicalDevicesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto physicalDevices = std::vector<VkPhysicalDevice>
		(
			physicalDevicesCount, VK_NULL_HANDLE
		);
		result = vkEnumeratePhysicalDevices
		(
			vkInstance,
			&physicalDevicesCount, physicalDevices.data()
		);
		assert(result == VK_SUCCESS);

		auto vulkanPhysicalDevices = std::vector<PhysicalDevice> { };
		for (const auto physicalDevice : physicalDevices)
		{
			vulkanPhysicalDevices.emplace_back(*this, physicalDevice);
		}
		return vulkanPhysicalDevices;
	}
}
