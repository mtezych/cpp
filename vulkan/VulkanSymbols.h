
#ifndef VULKAN_SYMBOLS
#define VULKAN_SYMBOLS

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace symbol
{
	struct vkGetInstanceProcAddr
	{
		using                 type = PFN_vkGetInstanceProcAddr;
		static constexpr auto name =    "vkGetInstanceProcAddr";
	};

	struct vkCreateInstance
	{
		using                 type = PFN_vkCreateInstance;
		static constexpr auto name =    "vkCreateInstance";
	};

	struct vkEnumerateInstanceExtensionProperties
	{
		using                 type = PFN_vkEnumerateInstanceExtensionProperties;
		static constexpr auto name =    "vkEnumerateInstanceExtensionProperties";
	};

	struct vkEnumerateInstanceLayerProperties
	{
		using                 type = PFN_vkEnumerateInstanceLayerProperties;
		static constexpr auto name =    "vkEnumerateInstanceLayerProperties";
	};

	struct vkDestroyInstance
	{
		using                 type = PFN_vkDestroyInstance;
		static constexpr auto name =    "vkDestroyInstance";
	};

	struct vkEnumeratePhysicalDevices
	{
		using                 type = PFN_vkEnumeratePhysicalDevices;
		static constexpr auto name =    "vkEnumeratePhysicalDevices";
	};

	struct vkEnumerateDeviceExtensionProperties
	{
		using                 type = PFN_vkEnumerateDeviceExtensionProperties;
		static constexpr auto name =    "vkEnumerateDeviceExtensionProperties";
	};

	struct vkEnumerateDeviceLayerProperties
	{
		using                 type = PFN_vkEnumerateDeviceLayerProperties;
		static constexpr auto name =    "vkEnumerateDeviceLayerProperties";
	};

	struct vkGetPhysicalDeviceProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceProperties";
	};

	struct vkGetPhysicalDeviceFeatures
	{
		using                 type = PFN_vkGetPhysicalDeviceFeatures;
		static constexpr auto name =    "vkGetPhysicalDeviceFeatures";
	};

	struct vkGetPhysicalDeviceQueueFamilyProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceQueueFamilyProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceQueueFamilyProperties";
	};

	struct vkGetPhysicalDeviceFormatProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceFormatProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceFormatProperties";
	};

	struct vkGetPhysicalDeviceImageFormatProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceImageFormatProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceImageFormatProperties";
	};

	struct vkGetPhysicalDeviceMemoryProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceMemoryProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceMemoryProperties";
	};

	struct vkGetPhysicalDeviceSparseImageFormatProperties
	{
		using                 type = PFN_vkGetPhysicalDeviceSparseImageFormatProperties;
		static constexpr auto name =    "vkGetPhysicalDeviceSparseImageFormatProperties";
	};

	struct vkCreateDevice
	{
		using                 type = PFN_vkCreateDevice;
		static constexpr auto name =    "vkCreateDevice";
	};

	struct vkGetDeviceProcAddr
	{
		using                 type = PFN_vkGetDeviceProcAddr;
		static constexpr auto name =    "vkGetDeviceProcAddr";
	};

	struct vkDestroyDevice
	{
		using                 type = PFN_vkDestroyDevice;
		static constexpr auto name =    "vkDestroyDevice";
	};

	struct vkGetDeviceQueue
	{
		using                 type = PFN_vkGetDeviceQueue;
		static constexpr auto name =    "vkGetDeviceQueue";
	};

	struct vkDeviceWaitIdle
	{
		using                 type = PFN_vkDeviceWaitIdle;
		static constexpr auto name =    "vkDeviceWaitIdle";
	};

	struct vkQueueWaitIdle
	{
		using                 type = PFN_vkQueueWaitIdle;
		static constexpr auto name =    "vkQueueWaitIdle";
	};

	struct vkQueueSubmit
	{
		using                 type = PFN_vkQueueSubmit;
		static constexpr auto name =    "vkQueueSubmit";
	};

#ifdef VK_USE_PLATFORM_XLIB_KHR
	struct vkCreateXlibSurfaceKHR
	{
		using                 type = PFN_vkCreateXlibSurfaceKHR;
		static constexpr auto name =    "vkCreateXlibSurfaceKHR";
	};
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
	struct vkCreateXcbSurfaceKHR
	{
		using                 type = PFN_vkCreateXcbSurfaceKHR;
		static constexpr auto name =    "vkCreateXcbSurfaceKHR";
	};
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	struct vkCreateWaylandSurfaceKHR
	{
		using                 type = PFN_vkCreateWaylandSurfaceKHR;
		static constexpr auto name =    "vkCreateWaylandSurfaceKHR";
	};
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
	struct vkCreateMirSurfaceKHR
	{
		using                 type = PFN_vkCreateMirSurfaceKHR;
		static constexpr auto name =    "vkCreateMirSurfaceKHR";
	};
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
	struct vkCreateAndroidSurfaceKHR
	{
		using                 type = PFN_vkCreateAndroidSurfaceKHR;
		static constexpr auto name =    "vkCreateAndroidSurfaceKHR";
	};
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
	struct vkCreateWin32SurfaceKHR
	{
		using                 type = PFN_vkCreateWin32SurfaceKHR;
		static constexpr auto name =    "vkCreateWin32SurfaceKHR";
	};
#endif

	struct vkDestroySurfaceKHR
	{
		using                 type = PFN_vkDestroySurfaceKHR;
		static constexpr auto name =    "vkDestroySurfaceKHR";
	};

	struct vkGetPhysicalDeviceSurfaceSupportKHR
	{
		using                 type = PFN_vkGetPhysicalDeviceSurfaceSupportKHR;
		static constexpr auto name =    "vkGetPhysicalDeviceSurfaceSupportKHR";
	};

	struct vkGetPhysicalDeviceSurfaceFormatsKHR
	{
		using                 type = PFN_vkGetPhysicalDeviceSurfaceFormatsKHR;
		static constexpr auto name =    "vkGetPhysicalDeviceSurfaceFormatsKHR";
	};

	struct vkGetPhysicalDeviceSurfacePresentModesKHR
	{
		using                 type = PFN_vkGetPhysicalDeviceSurfacePresentModesKHR;
		static constexpr auto name =    "vkGetPhysicalDeviceSurfacePresentModesKHR";
	};

	struct vkGetPhysicalDeviceSurfaceCapabilitiesKHR
	{
		using                 type = PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
		static constexpr auto name =    "vkGetPhysicalDeviceSurfaceCapabilitiesKHR";
	};

	struct vkCreateSemaphore
	{
		using                 type = PFN_vkCreateSemaphore;
		static constexpr auto name =    "vkCreateSemaphore";
	};

	struct vkDestroySemaphore
	{
		using                 type = PFN_vkDestroySemaphore;
		static constexpr auto name =    "vkDestroySemaphore";
	};

	struct vkCreateSwapchainKHR
	{
		using                 type = PFN_vkCreateSwapchainKHR;
		static constexpr auto name =    "vkCreateSwapchainKHR";
	};

	struct vkDestroySwapchainKHR
	{
		using                 type = PFN_vkDestroySwapchainKHR;
		static constexpr auto name =    "vkDestroySwapchainKHR";
	};

	struct vkGetSwapchainImagesKHR
	{
		using                 type = PFN_vkGetSwapchainImagesKHR;
		static constexpr auto name =    "vkGetSwapchainImagesKHR";
	};

	struct vkAcquireNextImageKHR
	{
		using                 type = PFN_vkAcquireNextImageKHR;
		static constexpr auto name =    "vkAcquireNextImageKHR";
	};

	struct vkQueuePresentKHR
	{
		using                 type = PFN_vkQueuePresentKHR;
		static constexpr auto name =    "vkQueuePresentKHR";
	};

	struct vkCreateCommandPool
	{
		using                 type = PFN_vkCreateCommandPool;
		static constexpr auto name =    "vkCreateCommandPool";
	};

	struct vkDestroyCommandPool
	{
		using                 type = PFN_vkDestroyCommandPool;
		static constexpr auto name =    "vkDestroyCommandPool";
	};

	struct vkAllocateCommandBuffers
	{
		using                 type = PFN_vkAllocateCommandBuffers;
		static constexpr auto name =    "vkAllocateCommandBuffers";
	};

	struct vkFreeCommandBuffers
	{
		using                 type = PFN_vkFreeCommandBuffers;
		static constexpr auto name =    "vkFreeCommandBuffers";
	};

	struct vkBeginCommandBuffer
	{
		using                 type = PFN_vkBeginCommandBuffer;
		static constexpr auto name =    "vkBeginCommandBuffer";
	};

	struct vkEndCommandBuffer
	{
		using                 type = PFN_vkEndCommandBuffer;
		static constexpr auto name =    "vkEndCommandBuffer";
	};

	struct vkCmdClearColorImage
	{
		using                 type = PFN_vkCmdClearColorImage;
		static constexpr auto name =    "vkCmdClearColorImage";
	};

	struct vkCmdPipelineBarrier
	{
		using                 type = PFN_vkCmdPipelineBarrier;
		static constexpr auto name =    "vkCmdPipelineBarrier";
	};

	struct vkCreateRenderPass
	{
		using                 type = PFN_vkCreateRenderPass;
		static constexpr auto name =    "vkCreateRenderPass";
	};

	struct vkDestroyRenderPass
	{
		using                 type = PFN_vkDestroyRenderPass;
		static constexpr auto name =    "vkDestroyRenderPass";
	};

	struct vkCreateImageView
	{
		using                 type = PFN_vkCreateImageView;
		static constexpr auto name =    "vkCreateImageView";
	};

	struct vkDestroyImageView
	{
		using                 type = PFN_vkDestroyImageView;
		static constexpr auto name =    "vkDestroyImageView";
	};

	struct vkCreateFramebuffer
	{
		using                 type = PFN_vkCreateFramebuffer;
		static constexpr auto name =    "vkCreateFramebuffer";
	};

	struct vkDestroyFramebuffer
	{
		using                 type = PFN_vkDestroyFramebuffer;
		static constexpr auto name =    "vkDestroyFramebuffer";
	};

	struct vkCreateShaderModule
	{
		using                 type = PFN_vkCreateShaderModule;
		static constexpr auto name =    "vkCreateShaderModule";
	};

	struct vkDestroyShaderModule
	{
		using                 type = PFN_vkDestroyShaderModule;
		static constexpr auto name =    "vkDestroyShaderModule";
	};

	struct vkCreateGraphicsPipelines
	{
		using                 type = PFN_vkCreateGraphicsPipelines;
		static constexpr auto name =    "vkCreateGraphicsPipelines";
	};

	struct vkCreateComputePipelines
	{
		using                 type = PFN_vkCreateComputePipelines;
		static constexpr auto name =    "vkCreateComputePipelines";
	};

	struct vkDestroyPipeline
	{
		using                 type = PFN_vkDestroyPipeline;
		static constexpr auto name =    "vkDestroyPipeline";
	};

	struct vkCreatePipelineCache
	{
		using                 type = PFN_vkCreatePipelineCache;
		static constexpr auto name =    "vkCreatePipelineCache";
	};

	struct vkDestroyPipelineCache
	{
		using                 type = PFN_vkDestroyPipelineCache;
		static constexpr auto name =    "vkDestroyPipelineCache";
	};

	struct vkCreatePipelineLayout
	{
		using                 type = PFN_vkCreatePipelineLayout;
		static constexpr auto name =    "vkCreatePipelineLayout";
	};

	struct vkDestroyPipelineLayout
	{
		using                 type = PFN_vkDestroyPipelineLayout;
		static constexpr auto name =    "vkDestroyPipelineLayout";
	};

	struct vkCmdBeginRenderPass
	{
		using                 type = PFN_vkCmdBeginRenderPass;
		static constexpr auto name =    "vkCmdBeginRenderPass";
	};

	struct vkCmdEndRenderPass
	{
		using                 type = PFN_vkCmdEndRenderPass;
		static constexpr auto name =    "vkCmdEndRenderPass";
	};

	struct vkCmdBindPipeline
	{
		using                 type = PFN_vkCmdBindPipeline;
		static constexpr auto name =    "vkCmdBindPipeline";
	};

	struct vkCmdDraw
	{
		using                 type = PFN_vkCmdDraw;
		static constexpr auto name =    "vkCmdDraw";
	};
}

#endif
