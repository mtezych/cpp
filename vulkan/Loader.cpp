
#include "Loader.h"

#include "Symbols.h"

namespace vk
{
	Loader::Loader()
	:
		library { },

		vkGetInstanceProcAddr                  { nullptr },
		vkEnumerateInstanceExtensionProperties { nullptr },
		vkEnumerateInstanceLayerProperties     { nullptr }
	{
	}

	Loader::Loader(const std::string& libraryPath)
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

	Loader::Loader(Loader&& loader)
	:
		library { std::move(loader.library) },

		vkGetInstanceProcAddr                  { loader.vkGetInstanceProcAddr                  },
		vkEnumerateInstanceExtensionProperties { loader.vkEnumerateInstanceExtensionProperties },
		vkEnumerateInstanceLayerProperties     { loader.vkEnumerateInstanceLayerProperties     }
	{
		loader.vkGetInstanceProcAddr                  = nullptr;
		loader.vkEnumerateInstanceExtensionProperties = nullptr;
		loader.vkEnumerateInstanceLayerProperties     = nullptr;
	}

	Loader& Loader::operator =(Loader&& loader)
	{
		library = std::move(loader.library);

		vkGetInstanceProcAddr                  = loader.vkGetInstanceProcAddr;
		vkEnumerateInstanceExtensionProperties = loader.vkEnumerateInstanceExtensionProperties;
		vkEnumerateInstanceLayerProperties     = loader.vkEnumerateInstanceLayerProperties;

		loader.vkGetInstanceProcAddr                  = nullptr;
		loader.vkEnumerateInstanceExtensionProperties = nullptr;
		loader.vkEnumerateInstanceLayerProperties     = nullptr;

		return *this;
	}

	std::vector<VkExtensionProperties>
	Loader::EnumerateInstanceExtensionProperties(const std::string& layerName) const
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
	Loader::EnumerateInstanceLayerProperties() const
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
