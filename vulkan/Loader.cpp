
#include "Loader.h"

#include "Symbols.h"

namespace vk
{
	Loader::Loader(const std::string& path)
	:
		library { path },

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                          VkInstance                               *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkGetInstanceProcAddr
		{
			LoadSymbol<symbol::vkGetInstanceProcAddr>()
		},
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                       VkLoader                                    *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkEnumerateInstanceExtensionProperties
		{
			LoadGlobalProcedure<symbol::vkEnumerateInstanceExtensionProperties>()
		},
		vkEnumerateInstanceLayerProperties
		{
			LoadGlobalProcedure<symbol::vkEnumerateInstanceLayerProperties>()
		},
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                VkInstance                         *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateInstance
		{
			LoadGlobalProcedure<symbol::vkCreateInstance>()
		}
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	{
	}

	Loader::Loader(Loader&& loader)
	:
		library { std::move(loader.library) },

		vkGetInstanceProcAddr                  { loader.vkGetInstanceProcAddr                  },
		vkEnumerateInstanceExtensionProperties { loader.vkEnumerateInstanceExtensionProperties },
		vkEnumerateInstanceLayerProperties     { loader.vkEnumerateInstanceLayerProperties     },
		vkCreateInstance                       { loader.vkCreateInstance                       }
	{
		loader.vkGetInstanceProcAddr                  = nullptr;
		loader.vkEnumerateInstanceExtensionProperties = nullptr;
		loader.vkEnumerateInstanceLayerProperties     = nullptr;
		loader.vkCreateInstance                       = nullptr;
	}

	Loader& Loader::operator =(Loader&& loader)
	{
		library = std::move(loader.library);

		vkGetInstanceProcAddr                  = loader.vkGetInstanceProcAddr;
		vkEnumerateInstanceExtensionProperties = loader.vkEnumerateInstanceExtensionProperties;
		vkEnumerateInstanceLayerProperties     = loader.vkEnumerateInstanceLayerProperties;
		vkCreateInstance                       = loader.vkCreateInstance;

		loader.vkGetInstanceProcAddr                  = nullptr;
		loader.vkEnumerateInstanceExtensionProperties = nullptr;
		loader.vkEnumerateInstanceLayerProperties     = nullptr;
		loader.vkCreateInstance                       = nullptr;

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
