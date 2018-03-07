
#ifndef VULKAN_QUEUE
#define VULKAN_QUEUE

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;
	struct CommandBuffer;
	struct Swapchain;
	struct Semaphore;

	struct Queue
	{
		struct CreateInfo
		{
			VkDeviceQueueCreateInfo createInfo;

			//
			// @note: Size of priorities is the queue count.
			//
			CreateInfo
			(
				const uint32_t            familyIndex,
				const std::vector<float>& priorities
			);
		};

		const Device* device;
		VkQueue       vkQueue;

		Queue (const Device& device, const VkQueue queue);

		//
		// @note: Semaphore will be signalled when
		//        execution of command buffer has been finished.
		//
		void Submit
		(
			const CommandBuffer&       commandBuffer,
			const Semaphore&           waitSemaphore,
			const VkPipelineStageFlags waitPipelineStage,
			      Semaphore&           signalSemaphore
		)
		const;

		void WaitIdle () const;

		void Present
		(
			const Swapchain& swapchain,
			const uint32_t   imageIndex,
			const Semaphore& imageReady
		)
		const;
	};
}

#endif
