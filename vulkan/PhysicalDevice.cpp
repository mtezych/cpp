
#include "PhysicalDevice.h"

#include "Symbols.h"
#include "Instance.h"
#include "Surface.h"

namespace vk
{
	PhysicalDevice::PhysicalDevice
	(
		const Instance& instance, const VkPhysicalDevice physicalDevice
	):
		physicalDevice { physicalDevice },

		vkEnumerateDeviceExtensionProperties
		{
			instance.LoadInstanceProcedure<symbol::vkEnumerateDeviceExtensionProperties>()
		},
		vkEnumerateDeviceLayerProperties
		{
			instance.LoadInstanceProcedure<symbol::vkEnumerateDeviceLayerProperties>()
		},

		vkGetPhysicalDeviceFeatures
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceFeatures>()
		},
		vkGetPhysicalDeviceProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceProperties>()
		},
		vkGetPhysicalDeviceQueueFamilyProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceQueueFamilyProperties>()
		},
		vkGetPhysicalDeviceFormatProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceFormatProperties>()
		},
		vkGetPhysicalDeviceImageFormatProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceImageFormatProperties>()
		},
		vkGetPhysicalDeviceSparseImageFormatProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSparseImageFormatProperties>()
		},
		vkGetPhysicalDeviceMemoryProperties
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceMemoryProperties>()
		},

		vkGetPhysicalDeviceSurfaceSupportKHR
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceSupportKHR>()
		},
		vkGetPhysicalDeviceSurfaceFormatsKHR
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceFormatsKHR>()
		},
		vkGetPhysicalDeviceSurfacePresentModesKHR
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfacePresentModesKHR>()
		},
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR
		{
			instance.LoadInstanceProcedure<symbol::vkGetPhysicalDeviceSurfaceCapabilitiesKHR>()
		}
	{
	}

	std::vector<VkExtensionProperties>
	PhysicalDevice::EnumerateDeviceExtensionProperties(const std::string& layerName) const
	{
		auto deviceExtensionPropertiesCount = uint32_t { 0 };
		auto result = vkEnumerateDeviceExtensionProperties
		(
			physicalDevice,
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
		result = vkEnumerateDeviceExtensionProperties
		(
			physicalDevice,
			layerName.empty() ? nullptr : layerName.c_str(),
			&deviceExtensionPropertiesCount,
			deviceExtensionProperties.data()
		);
		assert(result == VK_SUCCESS);

		return deviceExtensionProperties;
	}

	std::vector<VkLayerProperties>
	PhysicalDevice::EnumerateDeviceLayerProperties() const
	{
		auto deviceLayerPropertiesCount = uint32_t { 0 };
		auto result = vkEnumerateDeviceLayerProperties
		(
			physicalDevice,
			&deviceLayerPropertiesCount,
			nullptr
		);
		assert(result == VK_SUCCESS);

		auto deviceLayerProperties = std::vector<VkLayerProperties>
		(
			deviceLayerPropertiesCount,
			VkLayerProperties { }
		);
		result = vkEnumerateDeviceLayerProperties
		(
			physicalDevice,
			&deviceLayerPropertiesCount,
			deviceLayerProperties.data()
		);
		assert(result == VK_SUCCESS);

		return deviceLayerProperties;
	}

	VkPhysicalDeviceFeatures
	PhysicalDevice::GetPhysicalDeviceFeatures() const
	{
		auto physicalDeviceFeatures = VkPhysicalDeviceFeatures { };

		vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);

		return physicalDeviceFeatures;
	}

	VkPhysicalDeviceProperties
	PhysicalDevice::GetPhysicalDeviceProperties() const
	{
		auto physicalDeviceProperties = VkPhysicalDeviceProperties { };

		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		return physicalDeviceProperties;
	}

	std::vector<VkQueueFamilyProperties>
	PhysicalDevice::GetPhysicalDeviceQueueFamilyProperties() const
	{
		auto queueFamilyPropertiesCount = uint32_t { };
		vkGetPhysicalDeviceQueueFamilyProperties
		(
			physicalDevice,
			&queueFamilyPropertiesCount,
			nullptr
		);

		auto queueFamilyProperties = std::vector<VkQueueFamilyProperties>
		(
			queueFamilyPropertiesCount,
			VkQueueFamilyProperties { }
		);
		vkGetPhysicalDeviceQueueFamilyProperties
		(
			physicalDevice,
			&queueFamilyPropertiesCount,
			queueFamilyProperties.data()
		);

		return queueFamilyProperties;
	}

	VkFormatProperties
	PhysicalDevice::GetPhysicalDeviceFormatProperties(const VkFormat format) const
	{
		auto formatProperties = VkFormatProperties { };

		vkGetPhysicalDeviceFormatProperties
		(
			physicalDevice, format, &formatProperties
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

		const auto result = vkGetPhysicalDeviceImageFormatProperties
		(
			physicalDevice,
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
		vkGetPhysicalDeviceSparseImageFormatProperties
		(
			physicalDevice,
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
		vkGetPhysicalDeviceSparseImageFormatProperties
		(
			physicalDevice,
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
	PhysicalDevice::GetPhysicalDeviceMemoryProperties() const
	{
		auto memoryProperties = VkPhysicalDeviceMemoryProperties { };

		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		return memoryProperties;
	}

	bool PhysicalDevice::GetPhysicalDeviceSurfaceSupport
	(
		const uint32_t queueFamilyIndex,
		const Surface& surface
	) const
	{
		auto supported = VkBool32 { VK_FALSE };

		const auto result = vkGetPhysicalDeviceSurfaceSupportKHR
		(
			physicalDevice,
			queueFamilyIndex,
			surface.surface,
			&supported
		);
		assert(result == VK_SUCCESS);

		return supported == VK_TRUE;
	}

	std::vector<VkSurfaceFormatKHR>
	PhysicalDevice::GetPhysicalDeviceSurfaceFormats (const Surface& surface) const
	{
		auto surfaceFormatsCount = uint32_t { 0 };
		auto result = vkGetPhysicalDeviceSurfaceFormatsKHR
		(
			physicalDevice,
			surface.surface,
			&surfaceFormatsCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto surfaceFormats = std::vector<VkSurfaceFormatKHR>
		(
			surfaceFormatsCount,
			VkSurfaceFormatKHR { }
		);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR
		(
			physicalDevice,
			surface.surface,
			&surfaceFormatsCount, surfaceFormats.data()
		);
		assert(result == VK_SUCCESS);

		return surfaceFormats;
	}

	std::vector<VkPresentModeKHR>
	PhysicalDevice::GetPhysicalDeviceSurfacePresentModes (const Surface& surface) const
	{
		auto presentModesCount = uint32_t { 0 };
		auto result = vkGetPhysicalDeviceSurfacePresentModesKHR
		(
			physicalDevice,
			surface.surface,
			&presentModesCount, nullptr
		);
		assert(result == VK_SUCCESS);

		auto presentModes = std::vector<VkPresentModeKHR>
		(
			presentModesCount,
			VkPresentModeKHR { }
		);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR
		(
			physicalDevice,
			surface.surface,
			&presentModesCount, presentModes.data()
		);
		assert(result == VK_SUCCESS);

		return presentModes;
	}

	VkSurfaceCapabilitiesKHR
	PhysicalDevice::GetPhysicalDeviceSurfaceCapabilities (const Surface& surface) const
	{
		auto surfaceCapabilities = VkSurfaceCapabilitiesKHR { };

		const auto result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR
		(
			physicalDevice,
			surface.surface,
			&surfaceCapabilities
		);
		assert(result == VK_SUCCESS);

		return surfaceCapabilities;
	}
}
