
#ifndef VULKAN
#define VULKAN

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cassert>
#include <string>
#include <vector>

#include <library.h>

namespace vk
{
	struct Loader
	{
		platform::Library library;

		PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;

		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
		PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;

		Loader();

		explicit Loader(const std::string& libraryPath);

		~Loader() = default;

		Loader(const Loader& loader) = delete;

		Loader& operator =(const Loader& loader) = delete;

		Loader(Loader&& loader);

		Loader& operator =(Loader&& loader);

		template <typename Symbol>
		typename Symbol::type LoadSymbol() const
		{
			return library.LoadSymbol<typename Symbol::type>(Symbol::name);
		}

		template <typename Procedure>
		typename Procedure::type LoadGlobalProcedure() const
		{
			const auto procedureAddress = vkGetInstanceProcAddr(nullptr, Procedure::name);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		std::vector<VkExtensionProperties>
		EnumerateInstanceExtensionProperties(const std::string& layerName) const;

		std::vector<VkLayerProperties>
		EnumerateInstanceLayerProperties() const;
	};
}

#endif
