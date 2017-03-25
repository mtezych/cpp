
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
		VkQueue vkQueue;

		PFN_vkGetDeviceQueue    vkGetDeviceQueue;
		PFN_vkQueueSubmit       vkQueueSubmit;
		PFN_vkQueueWaitIdle     vkQueueWaitIdle;

		PFN_vkQueuePresentKHR   vkQueuePresentKHR;

		Queue(const Device& device, const VkQueue queue);

		Semaphore Submit
		(
			const Device&              device,
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
