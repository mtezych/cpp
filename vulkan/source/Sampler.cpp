
#include <vulkan/Sampler.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	Sampler::CreateInfo::CreateInfo
	(
		const VkFilter             magFilter,
		const VkFilter             minFilter,
		const VkSamplerMipmapMode  mipmapMode,
		const VkSamplerAddressMode addressModeU,
		const VkSamplerAddressMode addressModeV,
		const VkSamplerAddressMode addressModeW,
		const float                mipLodBias,
		const bool                 anisotropyEnable,
		const float                maxAnisotropy,
		const bool                 compareEnable,
		const VkCompareOp          compareOp,
		const float                minLod,
		const float                maxLod,
		const VkBorderColor        borderColor,
		const bool                 unnormalizedCoordinates
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			nullptr,
			0,
			magFilter,
			minFilter,
			mipmapMode,
			addressModeU,
			addressModeV,
			addressModeW,
			mipLodBias,
			anisotropyEnable,
			maxAnisotropy,
			compareEnable,
			compareOp,
			minLod,
			maxLod,
			borderColor,
			unnormalizedCoordinates,
		}
	{
	}

	Sampler::Sampler (const Device& device, const CreateInfo& createInfo)
	:
		device    { &device        },
		vkSampler { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateSampler
		(
			device.vkDevice,
			&createInfo.createInfo,
			nullptr,
			&vkSampler
		);
		assert(result == VK_SUCCESS);
	}

	Sampler::~Sampler ()
	{
		if (vkSampler != VK_NULL_HANDLE)
		{
			device->vkDestroySampler(device->vkDevice, vkSampler, nullptr);
		}
	}

	Sampler::Sampler (Sampler&& sampler)
	:
		device    { sampler.device    },
		vkSampler { sampler.vkSampler }
	{
		sampler.device    = nullptr;
		sampler.vkSampler = VK_NULL_HANDLE;
	}

	Sampler& Sampler::operator = (Sampler&& sampler)
	{
		if (vkSampler != VK_NULL_HANDLE)
		{
			device->vkDestroySampler(device->vkDevice, vkSampler, nullptr);
		}

		device    = sampler.device;
		vkSampler = sampler.vkSampler;

		sampler.device    = nullptr;
		sampler.vkSampler = VK_NULL_HANDLE;

		return *this;
	}
}
