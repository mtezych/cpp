
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
	struct Vulkan
	{
		platform::Library library;

		PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;

		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
		PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;

		Vulkan();

		explicit Vulkan(const std::string& libraryPath);

		~Vulkan() = default;

		Vulkan(const Vulkan& vulkan) = delete;

		Vulkan& operator =(const Vulkan& vulkan) = delete;

		Vulkan(Vulkan&& vulkan);

		Vulkan& operator =(Vulkan&& vulkan);

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

		std::vector<VkLayerProperties> EnumerateInstanceLayerProperties() const;
	};
}

#endif
