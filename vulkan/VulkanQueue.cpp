
#include "VulkanQueue.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapchain.h"

#include <cassert>
#include <vector>

namespace vk
{
	VulkanQueue::VulkanQueue(const VulkanDevice& device, const VkQueue queue)
	:
		queue { queue },

		vkGetDeviceQueue  { device.LoadDeviceProcedure<symbol::vkGetDeviceQueue >() },
		vkQueueSubmit     { device.LoadDeviceProcedure<symbol::vkQueueSubmit    >() },
		vkQueueWaitIdle   { device.LoadDeviceProcedure<symbol::vkQueueWaitIdle  >() },

		vkQueuePresentKHR { device.LoadDeviceProcedure<symbol::vkQueuePresentKHR>() }
	{
	}

	VulkanSemaphore VulkanQueue::Submit
	(
		const VulkanDevice&        device,
		const VulkanSemaphore&     imageAvaliable,
		const VkPipelineStageFlags waitPipelineStage,
		const VulkanCommandBuffer& commandBuffer
	) const
	{
		auto renderingFinished = VulkanSemaphore { device };

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

	void VulkanQueue::WaitIdle()
	{
		const auto result = vkQueueWaitIdle(queue);
		assert(result == VK_SUCCESS);
	}

	void VulkanQueue::Present
	(
		const VulkanSwapchain& swapchain,
		const uint32_t         imageIndex,
		const VulkanSemaphore& imageReady
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
