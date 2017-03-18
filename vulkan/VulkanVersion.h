
#ifndef VULKAN_VERSION
#define VULKAN_VERSION

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanVersion
	{
	private:
		uint32_t version;

	public:
		explicit VulkanVersion (const uint32_t version)
		:
			version { version }
		{
		}

		VulkanVersion (const uint32_t major, const uint32_t minor, const uint32_t patch)
		:
			version { VK_MAKE_VERSION(major, minor, patch) }
		{
		}

		uint32_t Version () const
		{
			return version;
		}

		uint32_t Major () const
		{
			return VK_VERSION_MAJOR(version);
		}

		uint32_t Minor () const
		{
			return VK_VERSION_MINOR(version);
		}

		uint32_t Patch () const
		{
			return VK_VERSION_PATCH(version);
		}
	};
}

#endif
