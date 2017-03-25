
#include "VulkanQueue.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapchain.h"

#include <cassert>
#include <vector>

namespace vk
{
	Queue::Queue(const Device& device, const VkQueue queue)
	:
		queue { queue },

		vkGetDeviceQueue  { device.LoadDeviceProcedure<symbol::vkGetDeviceQueue >() },
		vkQueueSubmit     { device.LoadDeviceProcedure<symbol::vkQueueSubmit    >() },
		vkQueueWaitIdle   { device.LoadDeviceProcedure<symbol::vkQueueWaitIdle  >() },

		vkQueuePresentKHR { device.LoadDeviceProcedure<symbol::vkQueuePresentKHR>() }
	{
	}

	Semaphore Queue::Submit
	(
		const Device&              device,
		const Semaphore&           imageAvaliable,
		const VkPipelineStageFlags waitPipelineStage,
		const CommandBuffer&       commandBuffer
	) const
	{
		auto renderingFinished = Semaphore { device };

		const auto submitInfo = VkSubmitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,
			1, &imageAvaliable.semaphore, &waitPipelineStage,
			1, &commandBuffer.commandBuffer,
			1, &renderingFinished.semaphore
		};
		const auto result = vkQueueSubmit
		(
			queue,
			1, &submitInfo,
			VkFence { VK_NULL_HANDLE }
		);
		assert(result == VK_SUCCESS);

		return renderingFinished;
	}

	void Queue::WaitIdle()
	{
		const auto result = vkQueueWaitIdle(queue);
		assert(result == VK_SUCCESS);
	}

	void Queue::Present
	(
		const Swapchain& swapchain,
		const uint32_t   imageIndex,
		const Semaphore& imageReady
	) const
	{
		const auto presentInfo = VkPresentInfoKHR
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			nullptr,
			1, &imageReady.semaphore,
			1, &swapchain.swapchain, &imageIndex,
			nullptr
		};
		const auto result = vkQueuePresentKHR(queue, &presentInfo);
		assert(result == VK_SUCCESS);
	}
}
