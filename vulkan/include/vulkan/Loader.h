
#ifndef VULKAN
#define VULKAN

#include <vulkan/vulkan.h>

#include <cassert>
#include <string>
#include <vector>

#include <platform/Library.h>

namespace vk
{
	struct Loader
	{
		platform::Library library;

		/*
		┌──────────────────────────────────────────────────────┐
		│                      VkInstance                      │
		├──────────────────────────────────────────────────────┤
		│*/ PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr; /*│
		└──────────────────────────────────────────────────────┘
		┌────────────────────────────────────────────────────────────────────────────────────────┐
		│                                        VkLoader                                        │
		├────────────────────────────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties; /*│
		│*/ PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;     /*│
		└────────────────────────────────────────────────────────────────────────────────────────┘
		┌──────────────────────────────────────────────────────┐
		│                      VkInstance                      │
		├──────────────────────────────────────────────────────┤
		│*/ PFN_vkCreateInstance      vkCreateInstance;      /*│
		└──────────────────────────────────────────────────────┘
		*/

		Loader ();

		explicit Loader (const std::string& path);

		~Loader () = default;

		Loader (Loader&& loader);
		Loader (const Loader& loader) = delete;

		Loader& operator = (Loader&& loader);
		Loader& operator = (const Loader& loader) = delete;

		template <typename Symbol>
		typename Symbol::type LoadSymbol () const
		{
			return library.LoadSymbol<typename Symbol::type>(Symbol::name);
		}

		template <typename Procedure>
		typename Procedure::type LoadGlobalProcedure () const
		{
			const auto procedureAddress = vkGetInstanceProcAddr(nullptr, Procedure::name);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		std::vector<VkExtensionProperties>
		EnumerateInstanceExtensionProperties (const std::string& layerName) const;

		std::vector<VkLayerProperties>
		EnumerateInstanceLayerProperties () const;
	};
}

#endif
