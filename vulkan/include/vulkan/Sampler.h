
#ifndef VULKAN_SAMPLER
#define VULKAN_SAMPLER

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct Sampler
	{
		struct CreateInfo
		{
			VkSamplerCreateInfo createInfo;

			CreateInfo
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
			);
		};

		const Device* device;
		VkSampler     vkSampler;

		Sampler (const Device& device, const CreateInfo& createInfo);

		~Sampler ();

		Sampler (Sampler&& sampler);
		Sampler (const Sampler& sampler) = delete;

		Sampler& operator = (Sampler&& sampler);
		Sampler& operator = (const Sampler& sampler) = delete;
	};
}

#endif
