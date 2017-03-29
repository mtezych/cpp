
#ifndef VULKAN_INSTANCE
#define VULKAN_INSTANCE

#include "Loader.h"
#include "PhysicalDevice.h"

namespace vk
{
	struct Instance
	{
		const Loader* loader;
		VkInstance    vkInstance;

		/*
		┌────────────────────────────────────────────────────────────────┐
		│                           VkInstance                           │
		├────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkDestroyInstance          vkDestroyInstance;          /*│
		│                                                                │
		│ ┌────────────────────────────────────────────────────────────┐ │
		│ │                         VkSurface                          │ │
		│ ├────────────────────────────────────────────────────────────┤ │
		│ │ vk Create  Xlib    Surface KHR                             │ │
		│ │ vk Create  Xcb     Surface KHR                             │ │
		│ │ vk Create  Wayland Surface KHR                             │ │
		│ │ vk Create  Android Surface KHR                             │ │
		│ │ vk Create  Win32   Surface KHR                             │ │
		│ │*/ PFN_vkDestroySurfaceKHR      vkDestroySurfaceKHR;      /*│ │
		│ └────────────────────────────────────────────────────────────┘ │
		│                                                                │
		│*/ PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices; /*│
		└────────────────────────────────────────────────────────────────┘
		┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
		│                                            VkPhysicalDevice                                            │
		├────────────────────────────────────────────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkGetPhysicalDeviceFeatures                    vkGetPhysicalDeviceFeatures;                    /*│
		│                                                                                                        │
		│*/ PFN_vkGetPhysicalDeviceProperties                  vkGetPhysicalDeviceProperties;                  /*│
		│*/ PFN_vkGetPhysicalDeviceQueueFamilyProperties       vkGetPhysicalDeviceQueueFamilyProperties;       /*│
		│*/ PFN_vkGetPhysicalDeviceFormatProperties            vkGetPhysicalDeviceFormatProperties;            /*│
		│*/ PFN_vkGetPhysicalDeviceImageFormatProperties       vkGetPhysicalDeviceImageFormatProperties;       /*│
		│*/ PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties; /*│
		│*/ PFN_vkGetPhysicalDeviceMemoryProperties            vkGetPhysicalDeviceMemoryProperties;            /*│
		│                                                                                                        │
		│*/ PFN_vkGetPhysicalDeviceSurfaceSupportKHR           vkGetPhysicalDeviceSurfaceSupportKHR;           /*│
		│*/ PFN_vkGetPhysicalDeviceSurfaceFormatsKHR           vkGetPhysicalDeviceSurfaceFormatsKHR;           /*│
		│*/ PFN_vkGetPhysicalDeviceSurfacePresentModesKHR      vkGetPhysicalDeviceSurfacePresentModesKHR;      /*│
		│*/ PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR      vkGetPhysicalDeviceSurfaceCapabilitiesKHR;      /*│
		│                                                                                                        │
		│*/ PFN_vkEnumerateDeviceExtensionProperties           vkEnumerateDeviceExtensionProperties;           /*│
		│*/ PFN_vkEnumerateDeviceLayerProperties               vkEnumerateDeviceLayerProperties;               /*│
		└────────────────────────────────────────────────────────────────────────────────────────────────────────┘
		┌──────────────────────────────────────────────────┐
		│                     VkDevice                     │
		├──────────────────────────────────────────────────┤
		│*/ PFN_vkCreateDevice      vkCreateDevice;      /*│
		│                                                  │
		│*/ PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr; /*│
		└──────────────────────────────────────────────────┘
		*/

		Instance ();
		Instance (const Loader& loader, const VkInstanceCreateInfo& createInfo);

		~Instance ();

		Instance(Instance&& instance);
		Instance(const Instance& instance) = delete;

		Instance& operator =(Instance&& instance);
		Instance& operator =(const Instance& instance) = delete;

		template <typename Procedure>
		typename Procedure::type LoadInstanceProcedure() const
		{
			const auto procedureAddress = loader->vkGetInstanceProcAddr
			(
				vkInstance, Procedure::name
			);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		std::vector<PhysicalDevice> EnumeratePhysicalDevices() const;
	};
}

#endif
