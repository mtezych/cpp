
#include "PhysicalDevice.h"

#include "Symbols.h"
#include "Instance.h"
#include "Surface.h"

namespace vk
{
	PhysicalDevice::PhysicalDevice
	(
		const Instance& instance, const VkPhysicalDevice vkPhysicalDevice
	):
		instance         { &instance },
		vkPhysicalDevice { vkPhysicalDevice }
	{
	}

	std::vector<VkExtensionProperties>
	PhysicalDevice::EnumerateDeviceExtensionProperties (const std::string& layerName) const
	{
		auto deviceExtensionPropertiesCount = uint32_t { 0 };
		auto result = instance->vkEnumerateDeviceExtensionProperties
		(
			vkPhysicalDevice,
			layerName.empty() ? nullptr : layerName.c_str(),
			&deviceExtensionPropertiesCount,
			nullptr
		);
		assert(result == VK_SUCCESS);

		auto deviceExtensionProperties = std::vector<VkExtensionProperties>
		(
			deviceExtensionPropertiesCount,
			VkExtensionProperties { }
		);
		result = instance->vkEnumerateDeviceExtensionProperties
		(
			vkPhysicalDevice,
			layerName.empty() ? nullptr : layerName.c_str(),
			&deviceExtensionPropertiesCount,
			deviceExtensionProperties.data()
		);
		assert(result == VK_SUCCESS);

		return deviceExtensionProperties;
	}

	std::vector<VkLayerProperties>
	PhysicalDevice::EnumerateDeviceLayerProperties () const
	{
		auto deviceLayerPropertiesCount = uint32_t { 0 };
		auto result = instance->vkEnumerateDeviceLayerProperties
		(
			vkPhysicalDevice,
			&deviceLayerPropertiesCount,
			nullptr
		);
		assert(result == VK_SUCCESS);

		auto deviceLayerProperties = std::vector<VkLayerProperties>
		(
			deviceLayerPropertiesCount,
			VkLayerProperties { }
		);
		result = instance->vkEnumerateDeviceLayerProperties
		(
			vkPhysicalDevice,
			&deviceLayerPropertiesCount,
			deviceLayerProperties.data()
		);
		assert(result == VK_SUCCESS);

		return deviceLayerProperties;
	}

	VkPhysicalDeviceFeatures
	PhysicalDevice::GetPhysicalDeviceFeatures () const
	{
		auto physicalDeviceFeatures = VkPhysicalDeviceFeatures { };

		instance->vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &physicalDeviceFeatures);

		return physicalDeviceFeatures;
	}

	VkPhysicalDeviceProperties
	PhysicalDevice::GetPhysicalDeviceProperties () const
	{
		auto physicalDeviceProperties = VkPhysicalDeviceProperties { };

		instance->vkGetPhysicalDeviceProperties(vkPhysicalDevice, &physicalDeviceProperties);

		return physicalDeviceProperties;
	}

	std::vector<VkQueueFamilyProperties>
	PhysicalDevice::GetPhysicalDeviceQueueFamilyProperties () const
	{
		auto queueFamilyPropertiesCount = uint32_t { };
		instance->vkGetPhysicalDeviceQueueFamilyProperties
		(
			vkPhysicalDevice,
			&queueFamilyPropertiesCount,
			nullptr
		);

		auto queueFamilyProperties = std::vector<VkQueueFamilyProperties>
		(
			queueFamilyPropertiesCount,
			VkQueueFamilyProperties { }
		);
		instance->vkGetPhysicalDeviceQueueFamilyProperties
		(
			vkPhysicalDevice,
			&queueFamilyPropertiesCount,
			queueFamilyProperties.data()
		);

		return queueFamilyProperties;
	}

	VkFormatProperties
	PhysicalDevice::GetPhysicalDeviceFormatProperties (const VkFormat format) const
	{
		auto formatProperties = VkFormatProperties { };

		instance->vkGetPhysicalDeviceFormatProperties
		(
			vkPhysicalDevice, format, &formatProperties
		);

		return formatProperties;
	}

	VkImageFormatProperties
	PhysicalDevice::GetPhysicalDeviceImageFormatProperties
	(
		const VkFormat           format,
		const VkImageType        imageType,
		const VkImageTiling      imageTiling,
		const VkImageUsageFlags  imageUsageFlags,
		const VkImageCreateFlags imageCreateFlags
	) const
	{
		auto imageFormatProperties = VkImageFormatProperties { };

		const auto result = instance->vkGetPhysicalDeviceImageFormatProperties
		(
			vkPhysicalDevice,
			format,
			imageType,
			imageTiling,
			imageUsageFlags,
			imageCreateFlags,
			&imageFormatProperties
		);
		assert(result == VK_SUCCESS);

		return imageFormatProperties;
	}

	std::vector<VkSparseImageFormatProperties>
	PhysicalDevice::GetPhysicalDeviceSparseImageFormatProperties
	(
		const VkFormat              format,
		const VkImageType           imageType,
		const VkSampleCountFlagBits sampleCountFlagBits,
		const VkImageUsageFlags     imageUsageFlags,
		const VkImageTiling         imageTiling
	) const
	{
		auto sparseImageFormatPropertiesCount = uint32_t { };
		instance->vkGetPhysicalDeviceSparseImageFormatProperties
		(
			vkPhysicalDevice,
			format,
			imageType,
			sampleCountFlagBits,
			imageUsageFlags,
			imageTiling,
			&sparseImageFormatPropertiesCount,
			nullptr
		);

		auto sparseImageFormatProperties = std::vector<VkSparseImageFormatProperties>
		(
			sparseImageFormatPropertiesCount,
			VkSparseImageFormatProperties { }
		);
		instance->vkGetPhysicalDeviceSparseImageFormatProperties
		(
			vkPhysicalDevice,
			format,
			imageType,
			sampleCountFlagBits,
			imageUsageFlags,
			imageTiling,
			&sparseImageFormatPropertiesCount,
			sparseImageFormatProperties.data()
		);

		return sparseImageFormatProperties;
	}

	VkPhysicalDeviceMemoryProperties
	PhysicalDevice::GetPhysicalDeviceMemoryProperties () const
	{
		auto memoryProperties = VkPhysicalDeviceMemoryProperties { };

		instance->vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);

		return memoryProperties;
	}

	bool PhysicalDevice::GetPhysicalDeviceSurfaceSupport
	(
		const uint32_t queueFamilyIndex,
		const Surface& surface
	) const
	{
		auto supported = VkBool32 { VK_FALSE };

		const auto result = instance->vkGetPhysicalDeviceSurfaceSupportKHR
		(
			vkPhysicalDevice,
			queueFamilyIndex,
			surface.vkSurface,
			&supported
		);
		assert(result == VK_SUCCESS);

		return supported == VK_TRUE;
	}

	std::vector<VkSurfaceFormatKHR>
	PhysicalDevice::GetPhysicalDeviceSurfaceFormats (const Surface& surface) const
	{
		auto surfaceFormatsCount = uint32_t { 0 };
		auto result = instance->vkGetPhysicalDeviceSurfaceFormatsKHR
		(
			vkPhysicalDevice,
			surface.vkSurface,
			&surfaceFormatsCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto surfaceFormats = std::vector<VkSurfaceFormatKHR>
		(
			surfaceFormatsCount,
			VkSurfaceFormatKHR { }
		);
		result = instance->vkGetPhysicalDeviceSurfaceFormatsKHR
		(
			vkPhysicalDevice,
			surface.vkSurface,
			&surfaceFormatsCount, surfaceFormats.data()
		);
		assert(result == VK_SUCCESS);

		return surfaceFormats;
	}

	std::vector<VkPresentModeKHR>
	PhysicalDevice::GetPhysicalDeviceSurfacePresentModes (const Surface& surface) const
	{
		auto presentModesCount = uint32_t { 0 };
		auto result = instance->vkGetPhysicalDeviceSurfacePresentModesKHR
		(
			vkPhysicalDevice,
			surface.vkSurface,
			&presentModesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto presentModes = std::vector<VkPresentModeKHR>
		(
			presentModesCount,
			VkPresentModeKHR { }
		);
		result = instance->vkGetPhysicalDeviceSurfacePresentModesKHR
		(
			vkPhysicalDevice,
			surface.vkSurface,
			&presentModesCount, presentModes.data()
		);
		assert(result == VK_SUCCESS);

		return presentModes;
	}

	VkSurfaceCapabilitiesKHR
	PhysicalDevice::GetPhysicalDeviceSurfaceCapabilities (const Surface& surface) const
	{
		auto surfaceCapabilities = VkSurfaceCapabilitiesKHR { };

		const auto result = instance->vkGetPhysicalDeviceSurfaceCapabilitiesKHR
		(
			vkPhysicalDevice,
			surface.vkSurface,
			&surfaceCapabilities
		);
		assert(result == VK_SUCCESS);

		return surfaceCapabilities;
	}
}
