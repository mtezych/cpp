
#ifndef VULKAN_PHYSICAL_DEVICE
#define VULKAN_PHYSICAL_DEVICE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>
#include <string>
#include <cassert>

namespace vk
{
	struct VulkanInstance;
	struct VulkanSurface;

	struct VulkanPhysicalDevice
	{
		VkPhysicalDevice physicalDevice;

		PFN_vkEnumerateDeviceExtensionProperties           vkEnumerateDeviceExtensionProperties;
		PFN_vkEnumerateDeviceLayerProperties               vkEnumerateDeviceLayerProperties;
		PFN_vkGetPhysicalDeviceFeatures                    vkGetPhysicalDeviceFeatures;
		PFN_vkGetPhysicalDeviceProperties                  vkGetPhysicalDeviceProperties;
		PFN_vkGetPhysicalDeviceQueueFamilyProperties       vkGetPhysicalDeviceQueueFamilyProperties;
		PFN_vkGetPhysicalDeviceFormatProperties            vkGetPhysicalDeviceFormatProperties;
		PFN_vkGetPhysicalDeviceImageFormatProperties       vkGetPhysicalDeviceImageFormatProperties;
		PFN_vkGetPhysicalDeviceMemoryProperties            vkGetPhysicalDeviceMemoryProperties;
		PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;

		PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR;
		PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR;
		PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
		PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;

		VulkanPhysicalDevice(const VulkanInstance& instance, const VkPhysicalDevice physicalDevice);

		std::vector<VkExtensionProperties>
		EnumerateDeviceExtensionProperties(const std::string& layerName) const;

		std::vector<VkLayerProperties>
		EnumerateDeviceLayerProperties() const;

		VkPhysicalDeviceFeatures
		GetPhysicalDeviceFeatures() const;

		VkPhysicalDeviceProperties
		GetPhysicalDeviceProperties() const;

		std::vector<VkQueueFamilyProperties>
		GetPhysicalDeviceQueueFamilyProperties() const;

		VkFormatProperties
		GetPhysicalDeviceFormatProperties(const VkFormat format) const;

		VkImageFormatProperties GetPhysicalDeviceImageFormatProperties
		(
			const VkFormat           format,
			const VkImageType        imageType,
			const VkImageTiling      imageTiling,
			const VkImageUsageFlags  imageUsageFlags,
			const VkImageCreateFlags imageCreateFlags
		) const;

		VkPhysicalDeviceMemoryProperties
		GetPhysicalDeviceMemoryProperties() const;

		std::vector<VkSparseImageFormatProperties>
		GetPhysicalDeviceSparseImageFormatProperties
		(
			const VkFormat              format,
			const VkImageType           imageType,
			const VkSampleCountFlagBits sampleCountFlagBits,
			const VkImageUsageFlags     imageUsageFlags,
			const VkImageTiling         imageTiling
		) const;

		bool GetPhysicalDeviceSurfaceSupport
		(
			const uint32_t       queueFamilyIndex,
			const VulkanSurface& surface
		) const;

		std::vector<VkSurfaceFormatKHR>
		GetPhysicalDeviceSurfaceFormats(const VulkanSurface& surface) const;

		std::vector<VkPresentModeKHR>
		GetPhysicalDeviceSurfacePresentModes(const VulkanSurface& surface)const;

		VkSurfaceCapabilitiesKHR
		GetPhysicalDeviceSurfaceCapabilities(const VulkanSurface& surface)const;
	};
}

#endif