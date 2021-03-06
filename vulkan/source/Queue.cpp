
#include <vulkan/Queue.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/CommandBuffer.h>
#include <vulkan/Swapchain.h>

#include <cassert>
#include <vector>

namespace vk
{
	Queue::CreateInfo::CreateInfo
	(
		const uint32_t            familyIndex,
		const std::vector<float>& priorities
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			familyIndex,
			static_cast<uint32_t>(priorities.size()), priorities.data(),
		}
	{
	}

	Queue::Queue(const Device& device, const VkQueue vkQueue)
	:
		device  { &device },
		vkQueue { vkQueue }
	{
	}

	void Queue::Submit
	(
		const CommandBuffer&       commandBuffer,
		const Semaphore&           waitSemaphore,
		const VkPipelineStageFlags waitPipelineStage,
		      Semaphore&           signalSemaphore
	) const
	{
		const auto submitInfo = VkSubmitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,
			1, &waitSemaphore.vkSemaphore, &waitPipelineStage,
			1, &commandBuffer.vkCommandBuffer,
			1, &signalSemaphore.vkSemaphore
		};
		const auto result = device->vkQueueSubmit
		(
			vkQueue,
			1, &submitInfo,
			VkFence { VK_NULL_HANDLE }
		);
		assert(result == VK_SUCCESS);
	}

	void Queue::WaitIdle () const
	{
		const auto result = device->vkQueueWaitIdle(vkQueue);
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
		const auto result = device->vkQueuePresentKHR(vkQueue, &presentInfo);
		assert(result == VK_SUCCESS);
	}
}
