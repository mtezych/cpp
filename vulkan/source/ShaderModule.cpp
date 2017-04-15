
#include <vulkan/ShaderModule.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	ShaderModule::ShaderModule
	(
		const Device&                device,
		const std::vector<uint32_t>& spirvCode
	):
		device         { &device },
		vkShaderModule { VK_NULL_HANDLE }
	{
		const auto createInfo = VkShaderModuleCreateInfo
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			sizeof(uint32_t) * spirvCode.size(), spirvCode.data()
		};

		const auto result = device.vkCreateShaderModule
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkShaderModule
		);
		assert(result == VK_SUCCESS);
	}

	ShaderModule::~ShaderModule()
	{
		if (vkShaderModule != VK_NULL_HANDLE)
		{
			device->vkDestroyShaderModule(device->vkDevice, vkShaderModule, nullptr);
		}
	}
}
