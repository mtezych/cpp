
#include "Vulkan.h"
#include "VulkanSymbols.h"

namespace vk
{
	Vulkan::Vulkan()
	:
		library { },

		vkGetInstanceProcAddr                  { nullptr },
		vkEnumerateInstanceExtensionProperties { nullptr },
		vkEnumerateInstanceLayerProperties     { nullptr }
	{
	}

	Vulkan::Vulkan(const std::string& libraryPath)
	:
		library { libraryPath },

		vkGetInstanceProcAddr
		{
			LoadSymbol<symbol::vkGetInstanceProcAddr>()
		},
		vkEnumerateInstanceExtensionProperties
		{
			LoadGlobalProcedure<symbol::vkEnumerateInstanceExtensionProperties>()
		},
		vkEnumerateInstanceLayerProperties
		{
			LoadGlobalProcedure<symbol::vkEnumerateInstanceLayerProperties>()
		}
	{
	}

	Vulkan::Vulkan(Vulkan&& vulkan)
	:
		library { std::move(vulkan.library) },

		vkGetInstanceProcAddr                  { vulkan.vkGetInstanceProcAddr                  },
		vkEnumerateInstanceExtensionProperties { vulkan.vkEnumerateInstanceExtensionProperties },
		vkEnumerateInstanceLayerProperties     { vulkan.vkEnumerateInstanceLayerProperties     }
	{
		vulkan.vkGetInstanceProcAddr                  = nullptr;
		vulkan.vkEnumerateInstanceExtensionProperties = nullptr;
		vulkan.vkEnumerateInstanceLayerProperties     = nullptr;
	}

	Vulkan& Vulkan::operator =(Vulkan&& vulkan)
	{
		library = std::move(vulkan.library);

		vkGetInstanceProcAddr                  = vulkan.vkGetInstanceProcAddr;
		vkEnumerateInstanceExtensionProperties = vulkan.vkEnumerateInstanceExtensionProperties;
		vkEnumerateInstanceLayerProperties     = vulkan.vkEnumerateInstanceLayerProperties;

		vulkan.vkGetInstanceProcAddr                  = nullptr;
		vulkan.vkEnumerateInstanceExtensionProperties = nullptr;
		vulkan.vkEnumerateInstanceLayerProperties     = nullptr;

		return *this;
	}

	std::vector<VkExtensionProperties>
	Vulkan::EnumerateInstanceExtensionProperties(const std::string& layerName) const
	{
		auto instanceExtensionPropertiesCount = uint32_t { 0 };
		auto result = vkEnumerateInstanceExtensionProperties
		(
			layerName.empty() ? nullptr : layerName.c_str(),
			&instanceExtensionPropertiesCount,
			nullptr
		);
		assert(result == VK_SUCCESS);

		auto instanceExtensionProperties = std::vector<VkExtensionProperties>
		(
			instanceExtensionPropertiesCount,
			VkExtensionProperties { }
		);
		result = vkEnumerateInstanceExtensionProperties
		(
			layerName.empty() ? nullptr : layerName.c_str(),
			&instanceExtensionPropertiesCount,
			instanceExtensionProperties.data()
		);
		assert(result == VK_SUCCESS);

		return instanceExtensionProperties;
	}

	std::vector<VkLayerProperties>
	Vulkan::EnumerateInstanceLayerProperties() const
	{
		auto instanceLayerPropertiesCount = uint32_t { 0 };
		auto result = vkEnumerateInstanceLayerProperties
		(
			&instanceLayerPropertiesCount,
			nullptr
		);
		assert(result == VK_SUCCESS);

		auto instanceLayerProperties = std::vector<VkLayerProperties>
		(
			instanceLayerPropertiesCount,
			VkLayerProperties { }
		);
		result = vkEnumerateInstanceLayerProperties
		(
			&instanceLayerPropertiesCount,
			instanceLayerProperties.data()
		);
		assert(result == VK_SUCCESS);

		return instanceLayerProperties;
	}
}
