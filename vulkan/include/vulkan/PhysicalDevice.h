
#ifndef VULKAN_PHYSICAL_DEVICE
#define VULKAN_PHYSICAL_DEVICE

#include <vulkan/vulkan.h>

#include <vector>
#include <string>
#include <cassert>

namespace vk
{
	struct Instance;
	struct Surface;

	struct PhysicalDevice
	{
		const Instance*  instance;
		VkPhysicalDevice vkPhysicalDevice;

		PhysicalDevice
		(
			const Instance& instance, const VkPhysicalDevice vkPhysicalDevice
		);

		std::vector<VkExtensionProperties>
		EnumerateDeviceExtensionProperties (const std::string& layerName) const;

		std::vector<VkLayerProperties>
		EnumerateDeviceLayerProperties () const;

		VkPhysicalDeviceFeatures
		GetPhysicalDeviceFeatures () const;

		VkPhysicalDeviceProperties
		GetPhysicalDeviceProperties () const;

		std::vector<VkQueueFamilyProperties>
		GetPhysicalDeviceQueueFamilyProperties () const;

		VkFormatProperties
		GetPhysicalDeviceFormatProperties (const VkFormat format) const;

		VkImageFormatProperties GetPhysicalDeviceImageFormatProperties
		(
			const VkFormat           format,
			const VkImageType        imageType,
			const VkImageTiling      imageTiling,
			const VkImageUsageFlags  imageUsageFlags,
			const VkImageCreateFlags imageCreateFlags
		)
		const;

		std::vector<VkSparseImageFormatProperties>
		GetPhysicalDeviceSparseImageFormatProperties
		(
			const VkFormat              format,
			const VkImageType           imageType,
			const VkSampleCountFlagBits sampleCountFlagBits,
			const VkImageUsageFlags     imageUsageFlags,
			const VkImageTiling         imageTiling
		)
		const;

		VkPhysicalDeviceMemoryProperties
		GetPhysicalDeviceMemoryProperties () const;

		bool GetPhysicalDeviceSurfaceSupport
		(
			const uint32_t queueFamilyIndex,
			const Surface& surface
		)
		const;

		std::vector<VkSurfaceFormatKHR>
		GetPhysicalDeviceSurfaceFormats (const Surface& surface) const;

		std::vector<VkPresentModeKHR>
		GetPhysicalDeviceSurfacePresentModes (const Surface& surface) const;

		VkSurfaceCapabilitiesKHR
		GetPhysicalDeviceSurfaceCapabilities (const Surface& surface) const;
	};
}

#endif
