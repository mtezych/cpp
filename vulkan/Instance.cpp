
#include "Instance.h"

#include "Symbols.h"
#include "Loader.h"

namespace vk
{
	Instance::Instance (const Loader& loader, const VkInstanceCreateInfo& createInfo)
	:
		loader     { &loader },
		vkInstance { VK_NULL_HANDLE },

		vkDestroyInstance                              { nullptr },
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR                         { nullptr },
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR                      { nullptr },
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR                      { nullptr },
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR                        { nullptr },
#endif
		vkDestroySurfaceKHR                            { nullptr },
		vkEnumeratePhysicalDevices                     { nullptr },
		vkGetPhysicalDeviceFeatures                    { nullptr },
		vkGetPhysicalDeviceProperties                  { nullptr },
		vkGetPhysicalDeviceQueueFamilyProperties       { nullptr },
		vkGetPhysicalDeviceFormatProperties            { nullptr },
		vkGetPhysicalDeviceImageFormatProperties       { nullptr },
		vkGetPhysicalDeviceSparseImageFormatProperties { nullptr },
		vkGetPhysicalDeviceMemoryProperties            { nullptr },
		vkGetPhysicalDeviceSurfaceSupportKHR           { nullptr },
		vkGetPhysicalDeviceSurfaceFormatsKHR           { nullptr },
		vkGetPhysicalDeviceSurfacePresentModesKHR      { nullptr },
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR      { nullptr },
		vkEnumerateDeviceExtensionProperties           { nullptr },
		vkEnumerateDeviceLayerProperties               { nullptr },
		vkCreateDevice                                 { nullptr },
		vkGetDeviceProcAddr                            { nullptr }
	{
		const auto result = loader.vkCreateInstance
		(
			&createInfo, nullptr, &vkInstance
		);
		assert(result == VK_SUCCESS);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkInstance                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkDestroyInstance          = LoadInstanceProcedure<symbol::vkDestroyInstance         >();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkSurface                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR     = LoadInstanceProcedure<symbol::vkCreateXlibSurfaceKHR    >();
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR  = LoadInstanceProcedure<symbol::vkCreateWaylandSurfaceKHR >();
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR  = LoadInstanceProcedure<symbol::vkCreateAndroidSurfaceKHR >();
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR    = LoadInstanceProcedure<symbol::vkCreateWin32SurfaceKHR   >();
#endif
		vkDestroySurfaceKHR        = LoadInstanceProcedure<symbol::vkDestroySurfaceKHR       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vkEnumeratePhysicalDevices = LoadInstanceProcedure<symbol::vkEnumeratePhysicalDevices>();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                VkPhysicalDevice                                                                             *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkGetPhysicalDeviceFeatures                    = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceFeatures                   >();

		vkGetPhysicalDeviceProperties                  = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceProperties                 >();
		vkGetPhysicalDeviceQueueFamilyProperties       = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceQueueFamilyProperties      >();
		vkGetPhysicalDeviceFormatProperties            = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceFormatProperties           >();
		vkGetPhysicalDeviceImageFormatProperties       = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceImageFormatProperties      >();
		vkGetPhysicalDeviceSparseImageFormatProperties = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSparseImageFormatProperties>();
		vkGetPhysicalDeviceMemoryProperties            = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceMemoryProperties           >();

		vkGetPhysicalDeviceSurfaceSupportKHR           = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceSupportKHR          >();
		vkGetPhysicalDeviceSurfaceFormatsKHR           = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceFormatsKHR          >();
		vkGetPhysicalDeviceSurfacePresentModesKHR      = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfacePresentModesKHR     >();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR      = LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceCapabilitiesKHR     >();

		vkEnumerateDeviceExtensionProperties           = LoadInstanceProcedure<symbol::vkEnumerateDeviceExtensionProperties          >();
		vkEnumerateDeviceLayerProperties               = LoadInstanceProcedure<symbol::vkEnumerateDeviceLayerProperties              >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkDevice                                *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateDevice      = LoadInstanceProcedure<symbol::vkCreateDevice     >();
		vkGetDeviceProcAddr = LoadInstanceProcedure<symbol::vkGetDeviceProcAddr>();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	Instance::~Instance ()
	{
		if (vkInstance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(vkInstance, nullptr);
		}
	}

	Instance::Instance(Instance&& instance)
	:
		loader     { instance.loader     },
		vkInstance { instance.vkInstance },

		vkDestroyInstance                              { instance.vkDestroyInstance                              },
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR                         { instance.vkCreateXlibSurfaceKHR                         },
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR                      { instance.vkCreateWaylandSurfaceKHR                      },
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR                      { instance.vkCreateAndroidSurfaceKHR                      },
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR                        { instance.vkCreateWin32SurfaceKHR                        },
#endif
		vkDestroySurfaceKHR                            { instance.vkDestroySurfaceKHR                            },
		vkEnumeratePhysicalDevices                     { instance.vkEnumeratePhysicalDevices                     },
		vkGetPhysicalDeviceFeatures                    { instance.vkGetPhysicalDeviceFeatures                    },
		vkGetPhysicalDeviceProperties                  { instance.vkGetPhysicalDeviceProperties                  },
		vkGetPhysicalDeviceQueueFamilyProperties       { instance.vkGetPhysicalDeviceQueueFamilyProperties       },
		vkGetPhysicalDeviceFormatProperties            { instance.vkGetPhysicalDeviceFormatProperties            },
		vkGetPhysicalDeviceImageFormatProperties       { instance.vkGetPhysicalDeviceImageFormatProperties       },
		vkGetPhysicalDeviceSparseImageFormatProperties { instance.vkGetPhysicalDeviceSparseImageFormatProperties },
		vkGetPhysicalDeviceMemoryProperties            { instance.vkGetPhysicalDeviceMemoryProperties            },
		vkGetPhysicalDeviceSurfaceSupportKHR           { instance.vkGetPhysicalDeviceSurfaceSupportKHR           },
		vkGetPhysicalDeviceSurfaceFormatsKHR           { instance.vkGetPhysicalDeviceSurfaceFormatsKHR           },
		vkGetPhysicalDeviceSurfacePresentModesKHR      { instance.vkGetPhysicalDeviceSurfacePresentModesKHR      },
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR      { instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR      },
		vkEnumerateDeviceExtensionProperties           { instance.vkEnumerateDeviceExtensionProperties           },
		vkEnumerateDeviceLayerProperties               { instance.vkEnumerateDeviceLayerProperties               },
		vkCreateDevice                                 { instance.vkCreateDevice                                 },
		vkGetDeviceProcAddr                            { instance.vkGetDeviceProcAddr                            }
	{
		instance.loader     = nullptr;
		instance.vkInstance = VK_NULL_HANDLE;

		instance.vkDestroyInstance                              = nullptr;
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR                                  = nullptr;
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR                               = nullptr;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR                               = nullptr;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR                                 = nullptr;
#endif
		instance.vkDestroySurfaceKHR                            = nullptr;
		instance.vkEnumeratePhysicalDevices                     = nullptr;
		instance.vkGetPhysicalDeviceFeatures                    = nullptr;
		instance.vkGetPhysicalDeviceProperties                  = nullptr;
		instance.vkGetPhysicalDeviceQueueFamilyProperties       = nullptr;
		instance.vkGetPhysicalDeviceFormatProperties            = nullptr;
		instance.vkGetPhysicalDeviceImageFormatProperties       = nullptr;
		instance.vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
		instance.vkGetPhysicalDeviceMemoryProperties            = nullptr;
		instance.vkGetPhysicalDeviceSurfaceSupportKHR           = nullptr;
		instance.vkGetPhysicalDeviceSurfaceFormatsKHR           = nullptr;
		instance.vkGetPhysicalDeviceSurfacePresentModesKHR      = nullptr;
		instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR      = nullptr;
		instance.vkEnumerateDeviceExtensionProperties           = nullptr;
		instance.vkEnumerateDeviceLayerProperties               = nullptr;
		instance.vkCreateDevice                                 = nullptr;
		instance.vkGetDeviceProcAddr                            = nullptr;
	}

	Instance& Instance::operator = (Instance&& instance)
	{
		if (vkInstance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(vkInstance, nullptr);
		}

		loader     = instance.loader;
		vkInstance = instance.vkInstance;

		vkDestroyInstance                              = instance.vkDestroyInstance;
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR                         = instance.vkCreateXlibSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR                      = instance.vkCreateWaylandSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR                      = instance.vkCreateAndroidSurfaceKHR;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR                        = instance.vkCreateWin32SurfaceKHR;
#endif
		vkDestroySurfaceKHR                            = instance.vkDestroySurfaceKHR;
		vkEnumeratePhysicalDevices                     = instance.vkEnumeratePhysicalDevices;
		vkGetPhysicalDeviceFeatures                    = instance.vkGetPhysicalDeviceFeatures;
		vkGetPhysicalDeviceProperties                  = instance.vkGetPhysicalDeviceProperties;
		vkGetPhysicalDeviceQueueFamilyProperties       = instance.vkGetPhysicalDeviceQueueFamilyProperties;
		vkGetPhysicalDeviceFormatProperties            = instance.vkGetPhysicalDeviceFormatProperties;
		vkGetPhysicalDeviceImageFormatProperties       = instance.vkGetPhysicalDeviceImageFormatProperties;
		vkGetPhysicalDeviceSparseImageFormatProperties = instance.vkGetPhysicalDeviceSparseImageFormatProperties;
		vkGetPhysicalDeviceMemoryProperties            = instance.vkGetPhysicalDeviceMemoryProperties;
		vkGetPhysicalDeviceSurfaceSupportKHR           = instance.vkGetPhysicalDeviceSurfaceSupportKHR;
		vkGetPhysicalDeviceSurfaceFormatsKHR           = instance.vkGetPhysicalDeviceSurfaceFormatsKHR;
		vkGetPhysicalDeviceSurfacePresentModesKHR      = instance.vkGetPhysicalDeviceSurfacePresentModesKHR;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR      = instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
		vkEnumerateDeviceExtensionProperties           = instance.vkEnumerateDeviceExtensionProperties;
		vkEnumerateDeviceLayerProperties               = instance.vkEnumerateDeviceLayerProperties;
		vkCreateDevice                                 = instance.vkCreateDevice;
		vkGetDeviceProcAddr                            = instance.vkGetDeviceProcAddr;

		instance.loader     = nullptr;
		instance.vkInstance = VK_NULL_HANDLE;

		instance.vkDestroyInstance                              = nullptr;
#ifdef VK_USE_PLATFORM_XLIB_KHR
		vkCreateXlibSurfaceKHR                                  = nullptr;
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		vkCreateWaylandSurfaceKHR                               = nullptr;
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
		vkCreateAndroidSurfaceKHR                               = nullptr;
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vkCreateWin32SurfaceKHR                                 = nullptr;
#endif
		instance.vkDestroySurfaceKHR                            = nullptr;
		instance.vkEnumeratePhysicalDevices                     = nullptr;
		instance.vkGetPhysicalDeviceFeatures                    = nullptr;
		instance.vkGetPhysicalDeviceProperties                  = nullptr;
		instance.vkGetPhysicalDeviceQueueFamilyProperties       = nullptr;
		instance.vkGetPhysicalDeviceFormatProperties            = nullptr;
		instance.vkGetPhysicalDeviceImageFormatProperties       = nullptr;
		instance.vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
		instance.vkGetPhysicalDeviceMemoryProperties            = nullptr;
		instance.vkGetPhysicalDeviceSurfaceSupportKHR           = nullptr;
		instance.vkGetPhysicalDeviceSurfaceFormatsKHR           = nullptr;
		instance.vkGetPhysicalDeviceSurfacePresentModesKHR      = nullptr;
		instance.vkGetPhysicalDeviceSurfaceCapabilitiesKHR      = nullptr;
		instance.vkEnumerateDeviceExtensionProperties           = nullptr;
		instance.vkEnumerateDeviceLayerProperties               = nullptr;
		instance.vkCreateDevice                                 = nullptr;
		instance.vkGetDeviceProcAddr                            = nullptr;

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
