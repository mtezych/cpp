
#ifndef VULKAN_QUEUE
#define VULKAN_QUEUE

#include "VulkanSemaphore.h"

namespace vk
{
	struct VulkanDevice;
	struct VulkanCommandBuffer;
	struct VulkanSwapchain;

	struct VulkanQueue
	{
		VkQueue queue;

		PFN_vkGetDeviceQueue    vkGetDeviceQueue;
		PFN_vkQueueSubmit       vkQueueSubmit;
		PFN_vkQueueWaitIdle     vkQueueWaitIdle;

		PFN_vkQueuePresentKHR   vkQueuePresentKHR;

		VulkanQueue(const VulkanDevice& device, const VkQueue queue);

		VulkanSemaphore Submit
		(
			const VulkanDevice&        device,
			const VulkanSemaphore&     imageAvaliable,
			const VkPipelineStageFlags waitPipelineStage,
			const VulkanCommandBuffer& commandBuffer
		) const;

		void WaitIdle();

		void Present
		(
			const VulkanSwapchain& swapchain,
			const uint32_t         imageIndex,
			const VulkanSemaphore& imageReady
		) const;
	};
}

#endif
