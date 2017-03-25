
#include "ShaderModule.h"

#include "Symbols.h"
#include "Device.h"

namespace vk
{
	ShaderModule::ShaderModule
	(
		const Device&                device,
		const std::vector<uint32_t>& spirvCode
	):
		device         { &device },
		vkShaderModule { VK_NULL_HANDLE },

		vkCreateShaderModule
		{
			device.LoadDeviceProcedure<symbol::vkCreateShaderModule>()
		},
		vkDestroyShaderModule
		{
			device.LoadDeviceProcedure<symbol::vkDestroyShaderModule>()
		}
	{
		const auto createInfo = VkShaderModuleCreateInfo
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			sizeof(uint32_t) * spirvCode.size(), spirvCode.data()
		};

		const auto result = vkCreateShaderModule
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
			vkDestroyShaderModule(device->vkDevice, vkShaderModule, nullptr);
		}
	}
}
