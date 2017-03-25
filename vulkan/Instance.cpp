
#include "Instance.h"

#include "Symbols.h"
#include "Loader.h"

namespace vk
{
	Instance::Instance()
	:
		instance { VK_NULL_HANDLE },

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

	Instance::~Instance()
	{
		if (instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(instance, nullptr);
		}
	}

	Instance::Instance(Instance&& instance)
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

	Instance& Instance::operator =(Instance&& instance)
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

	std::vector<PhysicalDevice> Instance::EnumeratePhysicalDevices() const
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

		auto vulkanPhysicalDevices = std::vector<PhysicalDevice> { };
		for (const auto physicalDevice : physicalDevices)
		{
			vulkanPhysicalDevices.emplace_back(*this, physicalDevice);
		}
		return vulkanPhysicalDevices;
	}
}
