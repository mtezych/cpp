
#ifndef VULKAN_SEMAPHORE
#define VULKAN_SEMAPHORE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct Semaphore
	{
		const Device* device;
		VkSemaphore   vkSemaphore;

		PFN_vkCreateSemaphore  vkCreateSemaphore;
		PFN_vkDestroySemaphore vkDestroySemaphore;

		Semaphore();

		explicit Semaphore(const Device& device);

		Semaphore(Semaphore&& semaphore);
		Semaphore& operator = (Semaphore&& semaphore);

		~Semaphore();
	};
}

#endif
