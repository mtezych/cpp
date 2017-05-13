
#include <vulkan/Device.h>

#include <vulkan/Symbols.h>
#include <vulkan/Instance.h>

namespace vk
{
	Device::CreateInfo::CreateInfo
	(
		const std::vector<Queue::CreateInfo>& queueCreateInfos,
		const std::vector<const char*>&       layers,
		const std::vector<const char*>&       extensions,
		const VkPhysicalDeviceFeatures&       enabledFeatures
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(queueCreateInfos.size()),           nullptr,
			static_cast<uint32_t>(          layers.size()),     layers.data(),
			static_cast<uint32_t>(      extensions.size()), extensions.data(),
			&enabledFeatures,
		}
	{
		static_assert
		(
			sizeof(VkDeviceQueueCreateInfo) == sizeof(Queue::CreateInfo),
			"The same memory representations are required."
		);

		createInfo.pQueueCreateInfos =
		reinterpret_cast<const VkDeviceQueueCreateInfo*>(queueCreateInfos.data());
}

	Device::Device
	(
		const PhysicalDevice& physicalDevice,
		const CreateInfo&     createInfo
	):
		instance { physicalDevice.instance },
		vkDevice { VK_NULL_HANDLE },

		vkDestroyDevice           { nullptr },
		vkDeviceWaitIdle          { nullptr },
		vkCreateSemaphore         { nullptr },
		vkDestroySemaphore        { nullptr },
		vkCreateRenderPass        { nullptr },
		vkDestroyRenderPass       { nullptr },
		vkCreateFramebuffer       { nullptr },
		vkDestroyFramebuffer      { nullptr },
		vkCreateShaderModule      { nullptr },
		vkDestroyShaderModule     { nullptr },
		vkCreatePipelineCache     { nullptr },
		vkDestroyPipelineCache    { nullptr },
		vkCreatePipelineLayout    { nullptr },
		vkDestroyPipelineLayout   { nullptr },
		vkCreateComputePipelines  { nullptr },
		vkCreateGraphicsPipelines { nullptr },
		vkDestroyPipeline         { nullptr },
		vkCreateImage             { nullptr },
		vkDestroyImage            { nullptr },
		vkCreateImageView         { nullptr },
		vkDestroyImageView        { nullptr },
		vkCreateSwapchainKHR      { nullptr },
		vkDestroySwapchainKHR     { nullptr },
		vkGetSwapchainImagesKHR   { nullptr },
		vkAcquireNextImageKHR     { nullptr },
		vkCreateCommandPool       { nullptr },
		vkDestroyCommandPool      { nullptr },
		vkGetDeviceQueue          { nullptr },
		vkQueueSubmit             { nullptr },
		vkQueueWaitIdle           { nullptr },
		vkQueuePresentKHR         { nullptr },
		vkAllocateCommandBuffers  { nullptr },
		vkFreeCommandBuffers      { nullptr },
		vkBeginCommandBuffer      { nullptr },
		vkEndCommandBuffer        { nullptr },
		vkCmdBindPipeline         { nullptr },
		vkCmdDraw                 { nullptr },
		vkCmdClearColorImage      { nullptr },
		vkCmdPipelineBarrier      { nullptr },
		vkCmdBeginRenderPass      { nullptr },
		vkCmdEndRenderPass        { nullptr }
	{
		const auto result = instance->vkCreateDevice
		(
			physicalDevice.vkPhysicalDevice,
			&createInfo.createInfo,
			nullptr,
			&vkDevice
		);
		assert(result == VK_SUCCESS);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                    VkDevice                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkDestroyDevice           = LoadDeviceProcedure<symbol::vkDestroyDevice          >();

		vkDeviceWaitIdle          = LoadDeviceProcedure<symbol::vkDeviceWaitIdle         >();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkSemaphore                                   *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSemaphore         = LoadDeviceProcedure<symbol::vkCreateSemaphore        >();
		vkDestroySemaphore        = LoadDeviceProcedure<symbol::vkDestroySemaphore       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                  VkRenderPass                                   *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateRenderPass        = LoadDeviceProcedure<symbol::vkCreateRenderPass       >();
		vkDestroyRenderPass       = LoadDeviceProcedure<symbol::vkDestroyRenderPass      >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                  VkFramebuffer                                  *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateFramebuffer       = LoadDeviceProcedure<symbol::vkCreateFramebuffer      >();
		vkDestroyFramebuffer      = LoadDeviceProcedure<symbol::vkDestroyFramebuffer     >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkShaderModule                                  *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateShaderModule      = LoadDeviceProcedure<symbol::vkCreateShaderModule     >();
		vkDestroyShaderModule     = LoadDeviceProcedure<symbol::vkDestroyShaderModule    >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkPipelineCache                                 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineCache     = LoadDeviceProcedure<symbol::vkCreatePipelineCache    >();
		vkDestroyPipelineCache    = LoadDeviceProcedure<symbol::vkDestroyPipelineCache   >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkPipelineLayout                                *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineLayout    = LoadDeviceProcedure<symbol::vkCreatePipelineLayout   >();
		vkDestroyPipelineLayout   = LoadDeviceProcedure<symbol::vkDestroyPipelineLayout  >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkPipeline                                    *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateComputePipelines  = LoadDeviceProcedure<symbol::vkCreateComputePipelines >();
		vkCreateGraphicsPipelines = LoadDeviceProcedure<symbol::vkCreateGraphicsPipelines>();
		vkDestroyPipeline         = LoadDeviceProcedure<symbol::vkDestroyPipeline        >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkImage                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateImage             = LoadDeviceProcedure<symbol::vkCreateImage            >();
		vkDestroyImage            = LoadDeviceProcedure<symbol::vkDestroyImage           >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkImageView                                   *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateImageView         = LoadDeviceProcedure<symbol::vkCreateImageView        >();
		vkDestroyImageView        = LoadDeviceProcedure<symbol::vkDestroyImageView       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkSwapchain                                   *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSwapchainKHR      = LoadDeviceProcedure<symbol::vkCreateSwapchainKHR     >();
		vkDestroySwapchainKHR     = LoadDeviceProcedure<symbol::vkDestroySwapchainKHR    >();

		vkGetSwapchainImagesKHR   = LoadDeviceProcedure<symbol::vkGetSwapchainImagesKHR  >();
		vkAcquireNextImageKHR     = LoadDeviceProcedure<symbol::vkAcquireNextImageKHR    >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                  VkCommandPool                                  *
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

	Device::~Device ()
	{
		if (vkDevice != VK_NULL_HANDLE)
		{
			const auto result = vkDeviceWaitIdle(vkDevice);
			assert(result == VK_SUCCESS);

			vkDestroyDevice(vkDevice, nullptr);
		}
	}

	Device::Device (Device&& device)
	:
		instance { device.instance },
		vkDevice { device.vkDevice },

		vkDestroyDevice           { device.vkDestroyDevice           },
		vkDeviceWaitIdle          { device.vkDeviceWaitIdle          },
		vkCreateSemaphore         { device.vkCreateSemaphore         },
		vkDestroySemaphore        { device.vkDestroySemaphore        },
		vkCreateRenderPass        { device.vkCreateRenderPass        },
		vkDestroyRenderPass       { device.vkDestroyRenderPass       },
		vkCreateFramebuffer       { device.vkCreateFramebuffer       },
		vkDestroyFramebuffer      { device.vkDestroyFramebuffer      },
		vkCreateShaderModule      { device.vkCreateShaderModule      },
		vkDestroyShaderModule     { device.vkDestroyShaderModule     },
		vkCreatePipelineCache     { device.vkCreatePipelineCache     },
		vkDestroyPipelineCache    { device.vkDestroyPipelineCache    },
		vkCreatePipelineLayout    { device.vkCreatePipelineLayout    },
		vkDestroyPipelineLayout   { device.vkDestroyPipelineLayout   },
		vkCreateComputePipelines  { device.vkCreateComputePipelines  },
		vkCreateGraphicsPipelines { device.vkCreateGraphicsPipelines },
		vkDestroyPipeline         { device.vkDestroyPipeline         },
		vkCreateImage             { device.vkCreateImage             },
		vkDestroyImage            { device.vkDestroyImage            },
		vkCreateImageView         { device.vkCreateImageView         },
		vkDestroyImageView        { device.vkDestroyImageView        },
		vkCreateSwapchainKHR      { device.vkCreateSwapchainKHR      },
		vkDestroySwapchainKHR     { device.vkDestroySwapchainKHR     },
		vkGetSwapchainImagesKHR   { device.vkGetSwapchainImagesKHR   },
		vkAcquireNextImageKHR     { device.vkAcquireNextImageKHR     },
		vkCreateCommandPool       { device.vkCreateCommandPool       },
		vkDestroyCommandPool      { device.vkDestroyCommandPool      },
		vkGetDeviceQueue          { device.vkGetDeviceQueue          },
		vkQueueSubmit             { device.vkQueueSubmit             },
		vkQueueWaitIdle           { device.vkQueueWaitIdle           },
		vkQueuePresentKHR         { device.vkQueuePresentKHR         },
		vkAllocateCommandBuffers  { device.vkAllocateCommandBuffers  },
		vkFreeCommandBuffers      { device.vkFreeCommandBuffers      },
		vkBeginCommandBuffer      { device.vkBeginCommandBuffer      },
		vkEndCommandBuffer        { device.vkEndCommandBuffer        },
		vkCmdBindPipeline         { device.vkCmdBindPipeline         },
		vkCmdDraw                 { device.vkCmdDraw                 },
		vkCmdClearColorImage      { device.vkCmdClearColorImage      },
		vkCmdPipelineBarrier      { device.vkCmdPipelineBarrier      },
		vkCmdBeginRenderPass      { device.vkCmdBeginRenderPass      },
		vkCmdEndRenderPass        { device.vkCmdEndRenderPass        }
	{
		instance = nullptr;
		vkDevice = VK_NULL_HANDLE;

		vkDestroyDevice           = nullptr;
		vkDeviceWaitIdle          = nullptr;
		vkCreateSemaphore         = nullptr;
		vkDestroySemaphore        = nullptr;
		vkCreateRenderPass        = nullptr;
		vkDestroyRenderPass       = nullptr;
		vkCreateFramebuffer       = nullptr;
		vkDestroyFramebuffer      = nullptr;
		vkCreateShaderModule      = nullptr;
		vkDestroyShaderModule     = nullptr;
		vkCreatePipelineCache     = nullptr;
		vkDestroyPipelineCache    = nullptr;
		vkCreatePipelineLayout    = nullptr;
		vkDestroyPipelineLayout   = nullptr;
		vkCreateComputePipelines  = nullptr;
		vkCreateGraphicsPipelines = nullptr;
		vkDestroyPipeline         = nullptr;
		vkCreateImage             = nullptr;
		vkDestroyImage            = nullptr;
		vkCreateImageView         = nullptr;
		vkDestroyImageView        = nullptr;
		vkCreateSwapchainKHR      = nullptr;
		vkDestroySwapchainKHR     = nullptr;
		vkGetSwapchainImagesKHR   = nullptr;
		vkAcquireNextImageKHR     = nullptr;
		vkCreateCommandPool       = nullptr;
		vkDestroyCommandPool      = nullptr;
		vkGetDeviceQueue          = nullptr;
		vkQueueSubmit             = nullptr;
		vkQueueWaitIdle           = nullptr;
		vkQueuePresentKHR         = nullptr;
		vkAllocateCommandBuffers  = nullptr;
		vkFreeCommandBuffers      = nullptr;
		vkBeginCommandBuffer      = nullptr;
		vkEndCommandBuffer        = nullptr;
		vkCmdBindPipeline         = nullptr;
		vkCmdDraw                 = nullptr;
		vkCmdClearColorImage      = nullptr;
		vkCmdPipelineBarrier      = nullptr;
		vkCmdBeginRenderPass      = nullptr;
		vkCmdEndRenderPass        = nullptr;
	}

	Device& Device::operator = (Device&& device)
	{
		if (vkDevice != VK_NULL_HANDLE)
		{
			const auto result = vkDeviceWaitIdle(vkDevice);
			assert(result == VK_SUCCESS);

			vkDestroyDevice(vkDevice, nullptr);
		}

		instance = device.instance;
		vkDevice = device.vkDevice;

		vkDestroyDevice           = device.vkDestroyDevice;
		vkDeviceWaitIdle          = device.vkDeviceWaitIdle;
		vkCreateSemaphore         = device.vkCreateSemaphore;
		vkDestroySemaphore        = device.vkDestroySemaphore;
		vkCreateRenderPass        = device.vkCreateRenderPass;
		vkDestroyRenderPass       = device.vkDestroyRenderPass;
		vkCreateFramebuffer       = device.vkCreateFramebuffer;
		vkDestroyFramebuffer      = device.vkDestroyFramebuffer;
		vkCreateShaderModule      = device.vkCreateShaderModule;
		vkDestroyShaderModule     = device.vkDestroyShaderModule;
		vkCreatePipelineCache     = device.vkCreatePipelineCache;
		vkDestroyPipelineCache    = device.vkDestroyPipelineCache;
		vkCreatePipelineLayout    = device.vkCreatePipelineLayout;
		vkDestroyPipelineLayout   = device.vkDestroyPipelineLayout;
		vkCreateComputePipelines  = device.vkCreateComputePipelines;
		vkCreateGraphicsPipelines = device.vkCreateGraphicsPipelines;
		vkDestroyPipeline         = device.vkDestroyPipeline;
		vkCreateImage             = device.vkCreateImage;
		vkDestroyImage            = device.vkDestroyImage;
		vkCreateImageView         = device.vkCreateImageView;
		vkDestroyImageView        = device.vkDestroyImageView;
		vkCreateSwapchainKHR      = device.vkCreateSwapchainKHR;
		vkDestroySwapchainKHR     = device.vkDestroySwapchainKHR;
		vkGetSwapchainImagesKHR   = device.vkGetSwapchainImagesKHR;
		vkAcquireNextImageKHR     = device.vkAcquireNextImageKHR;
		vkCreateCommandPool       = device.vkCreateCommandPool;
		vkDestroyCommandPool      = device.vkDestroyCommandPool;
		vkGetDeviceQueue          = device.vkGetDeviceQueue;
		vkQueueSubmit             = device.vkQueueSubmit;
		vkQueueWaitIdle           = device.vkQueueWaitIdle;
		vkQueuePresentKHR         = device.vkQueuePresentKHR;
		vkAllocateCommandBuffers  = device.vkAllocateCommandBuffers;
		vkFreeCommandBuffers      = device.vkFreeCommandBuffers;
		vkBeginCommandBuffer      = device.vkBeginCommandBuffer;
		vkEndCommandBuffer        = device.vkEndCommandBuffer;
		vkCmdBindPipeline         = device.vkCmdBindPipeline;
		vkCmdDraw                 = device.vkCmdDraw;
		vkCmdClearColorImage      = device.vkCmdClearColorImage;
		vkCmdPipelineBarrier      = device.vkCmdPipelineBarrier;
		vkCmdBeginRenderPass      = device.vkCmdBeginRenderPass;
		vkCmdEndRenderPass        = device.vkCmdEndRenderPass;

		instance = nullptr;
		vkDevice = VK_NULL_HANDLE;

		vkDestroyDevice           = nullptr;
		vkDeviceWaitIdle          = nullptr;
		vkCreateSemaphore         = nullptr;
		vkDestroySemaphore        = nullptr;
		vkCreateRenderPass        = nullptr;
		vkDestroyRenderPass       = nullptr;
		vkCreateFramebuffer       = nullptr;
		vkDestroyFramebuffer      = nullptr;
		vkCreateShaderModule      = nullptr;
		vkDestroyShaderModule     = nullptr;
		vkCreatePipelineCache     = nullptr;
		vkDestroyPipelineCache    = nullptr;
		vkCreatePipelineLayout    = nullptr;
		vkDestroyPipelineLayout   = nullptr;
		vkCreateComputePipelines  = nullptr;
		vkCreateGraphicsPipelines = nullptr;
		vkDestroyPipeline         = nullptr;
		vkCreateImage             = nullptr;
		vkDestroyImage            = nullptr;
		vkCreateImageView         = nullptr;
		vkDestroyImageView        = nullptr;
		vkCreateSwapchainKHR      = nullptr;
		vkDestroySwapchainKHR     = nullptr;
		vkGetSwapchainImagesKHR   = nullptr;
		vkAcquireNextImageKHR     = nullptr;
		vkCreateCommandPool       = nullptr;
		vkDestroyCommandPool      = nullptr;
		vkGetDeviceQueue          = nullptr;
		vkQueueSubmit             = nullptr;
		vkQueueWaitIdle           = nullptr;
		vkQueuePresentKHR         = nullptr;
		vkAllocateCommandBuffers  = nullptr;
		vkFreeCommandBuffers      = nullptr;
		vkBeginCommandBuffer      = nullptr;
		vkEndCommandBuffer        = nullptr;
		vkCmdBindPipeline         = nullptr;
		vkCmdDraw                 = nullptr;
		vkCmdClearColorImage      = nullptr;
		vkCmdPipelineBarrier      = nullptr;
		vkCmdBeginRenderPass      = nullptr;
		vkCmdEndRenderPass        = nullptr;

		return *this;
	}

	Queue
	Device::Queues (const uint32_t familyIndex, const uint32_t queueIndex) const
	{
		auto queue = VkQueue { VK_NULL_HANDLE };

		vkGetDeviceQueue(vkDevice, familyIndex, queueIndex, &queue);

		return Queue { *this, queue };
	}
}
