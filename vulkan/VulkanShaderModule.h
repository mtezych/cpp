
#ifndef VULKAN_SHADER_MODULE
#define VULKAN_SHADER_MODULE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cassert>
#include <vector>

namespace vk
{
	struct Device;

	struct ShaderModule
	{
		VkDevice       device;
		VkShaderModule shaderModule;

		PFN_vkCreateShaderModule  vkCreateShaderModule;
		PFN_vkDestroyShaderModule vkDestroyShaderModule;

		ShaderModule
		(
			const Device&                device,
			const std::vector<uint32_t>& spirvCode
		);

		~ShaderModule();
	};
}

#endif
