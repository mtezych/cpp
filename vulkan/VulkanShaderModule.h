
#ifndef VULKAN_SHADER_MODULE
#define VULKAN_SHADER_MODULE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cassert>
#include <vector>

namespace vk
{
	struct VulkanDevice;

	struct VulkanShaderModule
	{
		VkDevice       device;
		VkShaderModule shaderModule;

		PFN_vkCreateShaderModule  vkCreateShaderModule;
		PFN_vkDestroyShaderModule vkDestroyShaderModule;

		VulkanShaderModule
		(
			const VulkanDevice& device,
			const std::vector<uint32_t>& spirvCode
		);

		~VulkanShaderModule();
	};
}

#endif
