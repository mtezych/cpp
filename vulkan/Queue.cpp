
#include "Queue.h"

#include "Symbols.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "Swapchain.h"

#include <cassert>
#include <vector>

namespace vk
{
	Queue::Queue(const Device& device, const VkQueue vkQueue)
	:
		vkQueue { vkQueue },

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
			1, &imageAvaliable.vkSemaphore, &waitPipelineStage,
			1, &commandBuffer.vkCommandBuffer,
			1, &renderingFinished.vkSemaphore
		};
		const auto result = vkQueueSubmit
		(
			vkQueue,
			1, &submitInfo,
			VkFence { VK_NULL_HANDLE }
		);
		assert(result == VK_SUCCESS);

		return renderingFinished;
	}

	void Queue::WaitIdle()
	{
		const auto result = vkQueueWaitIdle(vkQueue);
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
			1, &imageReady.vkSemaphore,
			1, &swapchain.vkSwapchain, &imageIndex,
			nullptr
		};
		const auto result = vkQueuePresentKHR(vkQueue, &presentInfo);
		assert(result == VK_SUCCESS);
	}
}
