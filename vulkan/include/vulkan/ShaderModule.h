
#ifndef VULKAN_SHADER_MODULE
#define VULKAN_SHADER_MODULE

#include <vulkan/vulkan.h>

#include <cassert>
#include <vector>

namespace vk
{
	struct Device;

	struct ShaderModule
	{
		const Device*  device;
		VkShaderModule vkShaderModule;

		ShaderModule
		(
			const Device&                device,
			const std::vector<uint32_t>& spirvCode
		);

		~ShaderModule();
	};
}

#endif
