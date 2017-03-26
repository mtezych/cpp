
#ifndef VULKAN_QUEUE
#define VULKAN_QUEUE

#include "Semaphore.h"

namespace vk
{
	struct Device;
	struct CommandBuffer;
	struct Swapchain;

	struct Queue
	{
		const Device* device;
		VkQueue       vkQueue;

		Queue(const Device& device, const VkQueue queue);

		Semaphore Submit
		(
			const Semaphore&           imageAvaliable,
			const VkPipelineStageFlags waitPipelineStage,
			const CommandBuffer&       commandBuffer
		) const;

		void WaitIdle();

		void Present
		(
			const Swapchain& swapchain,
			const uint32_t   imageIndex,
			const Semaphore& imageReady
		) const;
	};
}

#endif
