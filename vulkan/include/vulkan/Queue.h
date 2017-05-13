
#ifndef VULKAN_QUEUE
#define VULKAN_QUEUE

#include <vulkan/Semaphore.h>

#include <vector>

namespace vk
{
	struct Device;
	struct CommandBuffer;
	struct Swapchain;

	struct Queue
	{
		struct CreateInfo
		{
			VkDeviceQueueCreateInfo createInfo;

			//
			// @note: Size of priorities is queue count.
			//
			CreateInfo
			(
				const uint32_t            familyIndex,
				const std::vector<float>& priorities
			);
		};

		const Device* device;
		VkQueue       vkQueue;

		Queue(const Device& device, const VkQueue queue);

		Semaphore Submit
		(
			const Semaphore&           imageAvaliable,
			const VkPipelineStageFlags waitPipelineStage,
			const CommandBuffer&       commandBuffer
		)
		const;

		void WaitIdle ();

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
