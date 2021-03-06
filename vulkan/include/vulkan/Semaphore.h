
#ifndef VULKAN_SEMAPHORE
#define VULKAN_SEMAPHORE

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;

	struct Semaphore
	{
		const Device* device;
		VkSemaphore   vkSemaphore;

		explicit Semaphore (const Device& device);

		~Semaphore();

		Semaphore (Semaphore&& semaphore);
		Semaphore (const Semaphore& semaphore) = delete;

		Semaphore& operator = (Semaphore&& semaphore);
		Semaphore& operator = (const Semaphore& semaphore) = delete;
	};
}

#endif
