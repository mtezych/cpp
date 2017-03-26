
#include "Device.h"

#include "Symbols.h"
#include "Instance.h"

namespace vk
{
	Device::Device
	(
		const Instance&           instance,
		const PhysicalDevice&     physicalDevice,
		const VkDeviceCreateInfo& deviceCreateInfo
	):
		vkDevice { VK_NULL_HANDLE },

		vkCreateDevice
		{
			instance.LoadInstanceProcedure<symbol::vkCreateDevice>()
		},
		vkGetDeviceProcAddr
		{
			instance.LoadInstanceProcedure<symbol::vkGetDeviceProcAddr>()
		},

		vkGetDeviceQueue { nullptr },
		vkDeviceWaitIdle { nullptr },
		vkDestroyDevice  { nullptr }
	{
		const auto result = vkCreateDevice
		(
			physicalDevice.vkPhysicalDevice,
			&deviceCreateInfo,
			nullptr,
			&vkDevice
		);
		assert(result == VK_SUCCESS);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkDevice                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkDestroyDevice           = LoadDeviceProcedure<symbol::vkDestroyDevice          >();

		vkDeviceWaitIdle          = LoadDeviceProcedure<symbol::vkDeviceWaitIdle         >();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                VkSemaphore                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSemaphore         = LoadDeviceProcedure<symbol::vkCreateSemaphore        >();
		vkDestroySemaphore        = LoadDeviceProcedure<symbol::vkDestroySemaphore       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkRenderPass                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateRenderPass        = LoadDeviceProcedure<symbol::vkCreateRenderPass       >();
		vkDestroyRenderPass       = LoadDeviceProcedure<symbol::vkDestroyRenderPass      >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkFramebuffer                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateFramebuffer       = LoadDeviceProcedure<symbol::vkCreateFramebuffer      >();
		vkDestroyFramebuffer      = LoadDeviceProcedure<symbol::vkDestroyFramebuffer     >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                             VkShaderModule                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateShaderModule      = LoadDeviceProcedure<symbol::vkCreateShaderModule     >();
		vkDestroyShaderModule     = LoadDeviceProcedure<symbol::vkDestroyShaderModule    >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                            VkPipelineCache                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineCache     = LoadDeviceProcedure<symbol::vkCreatePipelineCache    >();
		vkDestroyPipelineCache    = LoadDeviceProcedure<symbol::vkDestroyPipelineCache   >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                            VkPipelineLayout                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineLayout    = LoadDeviceProcedure<symbol::vkCreatePipelineLayout   >();
		vkDestroyPipelineLayout   = LoadDeviceProcedure<symbol::vkDestroyPipelineLayout  >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkPipeline                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateComputePipelines  = LoadDeviceProcedure<symbol::vkCreateComputePipelines >();
		vkCreateGraphicsPipelines = LoadDeviceProcedure<symbol::vkCreateGraphicsPipelines>();
		vkDestroyPipeline         = LoadDeviceProcedure<symbol::vkDestroyPipeline        >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkImageView                                       *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateImageView         = LoadDeviceProcedure<symbol::vkCreateImageView        >();
		vkDestroyImageView        = LoadDeviceProcedure<symbol::vkDestroyImageView       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkSwapchain                                       *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSwapchainKHR      = LoadDeviceProcedure<symbol::vkCreateSwapchainKHR     >();
		vkDestroySwapchainKHR     = LoadDeviceProcedure<symbol::vkDestroySwapchainKHR    >();

		vkGetSwapchainImagesKHR   = LoadDeviceProcedure<symbol::vkGetSwapchainImagesKHR  >();
		vkAcquireNextImageKHR     = LoadDeviceProcedure<symbol::vkAcquireNextImageKHR    >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                               VkCommandPool                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateCommandPool       = LoadDeviceProcedure<symbol::vkCreateCommandPool      >();
		vkDestroyCommandPool      = LoadDeviceProcedure<symbol::vkDestroyCommandPool     >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vkGetDeviceQueue          = LoadDeviceProcedure<symbol::vkGetDeviceQueue         >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkQueue                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkQueueSubmit             = LoadDeviceProcedure<symbol::vkQueueSubmit            >();
		vkQueueWaitIdle           = LoadDeviceProcedure<symbol::vkQueueWaitIdle          >();

		vkQueuePresentKHR         = LoadDeviceProcedure<symbol::vkQueuePresentKHR        >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkCommandBuffer                                 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkAllocateCommandBuffers  = LoadDeviceProcedure<symbol::vkAllocateCommandBuffers >();
		vkFreeCommandBuffers      = LoadDeviceProcedure<symbol::vkFreeCommandBuffers     >();

		vkBeginCommandBuffer      = LoadDeviceProcedure<symbol::vkBeginCommandBuffer     >();
		vkEndCommandBuffer        = LoadDeviceProcedure<symbol::vkEndCommandBuffer       >();

		vkCmdBindPipeline         = LoadDeviceProcedure<symbol::vkCmdBindPipeline        >();

		vkCmdDraw                 = LoadDeviceProcedure<symbol::vkCmdDraw                >();

		vkCmdClearColorImage      = LoadDeviceProcedure<symbol::vkCmdClearColorImage     >();

		vkCmdPipelineBarrier      = LoadDeviceProcedure<symbol::vkCmdPipelineBarrier     >();

		vkCmdBeginRenderPass      = LoadDeviceProcedure<symbol::vkCmdBeginRenderPass     >();
		vkCmdEndRenderPass        = LoadDeviceProcedure<symbol::vkCmdEndRenderPass       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	Device::~Device()
	{
		const auto result = vkDeviceWaitIdle(vkDevice);
		assert(result == VK_SUCCESS);

		vkDestroyDevice(vkDevice, nullptr);
	}

	Queue
	Device::Queues(const uint32_t familyIndex, const uint32_t queueIndex) const
	{
		auto queue = VkQueue { VK_NULL_HANDLE };

		vkGetDeviceQueue(vkDevice, familyIndex, queueIndex, &queue);

		return Queue { *this, queue };
	}
}
