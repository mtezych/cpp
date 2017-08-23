
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
		physicalDevice { &physicalDevice },
		vkDevice       { VK_NULL_HANDLE  },

		vkDestroyDevice              { nullptr },
		vkDeviceWaitIdle             { nullptr },
		vkCreateSemaphore            { nullptr },
		vkDestroySemaphore           { nullptr },
		vkCreateFence                { nullptr },
		vkDestroyFence               { nullptr },
		vkGetFenceStatus             { nullptr },
		vkResetFences                { nullptr },
		vkWaitForFences              { nullptr },
		vkCreateRenderPass           { nullptr },
		vkDestroyRenderPass          { nullptr },
		vkCreateFramebuffer          { nullptr },
		vkDestroyFramebuffer         { nullptr },
		vkCreateShaderModule         { nullptr },
		vkDestroyShaderModule        { nullptr },
		vkCreatePipelineCache        { nullptr },
		vkDestroyPipelineCache       { nullptr },
		vkCreatePipelineLayout       { nullptr },
		vkDestroyPipelineLayout      { nullptr },
		vkCreateComputePipelines     { nullptr },
		vkCreateGraphicsPipelines    { nullptr },
		vkDestroyPipeline            { nullptr },
		vkAllocateMemory             { nullptr },
		vkFreeMemory                 { nullptr },
		vkMapMemory                  { nullptr },
		vkUnmapMemory                { nullptr },
		vkCreateBuffer               { nullptr },
		vkDestroyBuffer              { nullptr },
		vkBindBufferMemory           { nullptr },
		vkCreateBufferView           { nullptr },
		vkDestroyBufferView          { nullptr },
		vkCreateImage                { nullptr },
		vkDestroyImage               { nullptr },
		vkBindImageMemory            { nullptr },
		vkGetImageSubresourceLayout  { nullptr },
		vkCreateImageView            { nullptr },
		vkDestroyImageView           { nullptr },
		vkCreateSampler              { nullptr },
		vkDestroySampler             { nullptr },
		vkCreateDescriptorSetLayout  { nullptr },
		vkDestroyDescriptorSetLayout { nullptr },
		vkCreateDescriptorPool       { nullptr },
		vkDestroyDescriptorPool      { nullptr },
		vkResetDescriptorPool        { nullptr },
		vkAllocateDescriptorSets     { nullptr },
		vkFreeDescriptorSets         { nullptr },
		vkUpdateDescriptorSets       { nullptr },
		vkCreateSwapchainKHR         { nullptr },
		vkDestroySwapchainKHR        { nullptr },
		vkGetSwapchainImagesKHR      { nullptr },
		vkAcquireNextImageKHR        { nullptr },
		vkCreateCommandPool          { nullptr },
		vkDestroyCommandPool         { nullptr },
		vkResetCommandPool           { nullptr },
		vkGetDeviceQueue             { nullptr },
		vkQueueSubmit                { nullptr },
		vkQueueWaitIdle              { nullptr },
		vkQueuePresentKHR            { nullptr },
		vkAllocateCommandBuffers     { nullptr },
		vkFreeCommandBuffers         { nullptr },
		vkBeginCommandBuffer         { nullptr },
		vkEndCommandBuffer           { nullptr },
		vkCmdBindPipeline            { nullptr },
		vkCmdDraw                    { nullptr },
		vkCmdClearColorImage         { nullptr },
		vkCmdPipelineBarrier         { nullptr },
		vkCmdBeginRenderPass         { nullptr },
		vkCmdEndRenderPass           { nullptr }
	{
		const auto result = physicalDevice.instance->vkCreateDevice
		(
			physicalDevice.vkPhysicalDevice,
			&createInfo.createInfo,
			nullptr,
			&vkDevice
		);
		assert(result == VK_SUCCESS);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                       VkDevice                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkDestroyDevice              = LoadDeviceProcedure<symbol::vkDestroyDevice             >();

		vkDeviceWaitIdle             = LoadDeviceProcedure<symbol::vkDeviceWaitIdle            >();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkSemaphore                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSemaphore            = LoadDeviceProcedure<symbol::vkCreateSemaphore           >();
		vkDestroySemaphore           = LoadDeviceProcedure<symbol::vkDestroySemaphore          >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                        VkFence                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateFence                = LoadDeviceProcedure<symbol::vkCreateFence               >();
		vkDestroyFence               = LoadDeviceProcedure<symbol::vkDestroyFence              >();

		vkGetFenceStatus             = LoadDeviceProcedure<symbol::vkGetFenceStatus            >();
		vkResetFences                = LoadDeviceProcedure<symbol::vkResetFences               >();
		vkWaitForFences              = LoadDeviceProcedure<symbol::vkWaitForFences             >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkRenderPass                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateRenderPass           = LoadDeviceProcedure<symbol::vkCreateRenderPass          >();
		vkDestroyRenderPass          = LoadDeviceProcedure<symbol::vkDestroyRenderPass         >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkFramebuffer                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateFramebuffer          = LoadDeviceProcedure<symbol::vkCreateFramebuffer         >();
		vkDestroyFramebuffer         = LoadDeviceProcedure<symbol::vkDestroyFramebuffer        >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                    VkShaderModule                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateShaderModule         = LoadDeviceProcedure<symbol::vkCreateShaderModule        >();
		vkDestroyShaderModule        = LoadDeviceProcedure<symbol::vkDestroyShaderModule       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                    VkPipelineCache                                    *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineCache        = LoadDeviceProcedure<symbol::vkCreatePipelineCache       >();
		vkDestroyPipelineCache       = LoadDeviceProcedure<symbol::vkDestroyPipelineCache      >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                    VkPipelineLayout                                   *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreatePipelineLayout       = LoadDeviceProcedure<symbol::vkCreatePipelineLayout      >();
		vkDestroyPipelineLayout      = LoadDeviceProcedure<symbol::vkDestroyPipelineLayout     >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkPipeline                                       *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateComputePipelines     = LoadDeviceProcedure<symbol::vkCreateComputePipelines    >();
		vkCreateGraphicsPipelines    = LoadDeviceProcedure<symbol::vkCreateGraphicsPipelines   >();
		vkDestroyPipeline            = LoadDeviceProcedure<symbol::vkDestroyPipeline           >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                       VkMemory                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkAllocateMemory             = LoadDeviceProcedure<symbol::vkAllocateMemory            >();
		vkFreeMemory                 = LoadDeviceProcedure<symbol::vkFreeMemory                >();

		vkMapMemory                  = LoadDeviceProcedure<symbol::vkMapMemory                 >();
		vkUnmapMemory                = LoadDeviceProcedure<symbol::vkUnmapMemory               >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                       VkBuffer                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateBuffer               = LoadDeviceProcedure<symbol::vkCreateBuffer              >();
		vkDestroyBuffer              = LoadDeviceProcedure<symbol::vkDestroyBuffer             >();

		vkBindBufferMemory           = LoadDeviceProcedure<symbol::vkBindBufferMemory          >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkBufferView                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateBufferView           = LoadDeviceProcedure<symbol::vkCreateBufferView          >();
		vkDestroyBufferView          = LoadDeviceProcedure<symbol::vkDestroyBufferView         >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                        VkImage                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateImage                = LoadDeviceProcedure<symbol::vkCreateImage               >();
		vkDestroyImage               = LoadDeviceProcedure<symbol::vkDestroyImage              >();

		vkBindImageMemory            = LoadDeviceProcedure<symbol::vkBindImageMemory           >();

		vkGetImageSubresourceLayout  = LoadDeviceProcedure<symbol::vkGetImageSubresourceLayout >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkImageView                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateImageView            = LoadDeviceProcedure<symbol::vkCreateImageView           >();
		vkDestroyImageView           = LoadDeviceProcedure<symbol::vkDestroyImageView          >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                       VkSampler                                       *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSampler              = LoadDeviceProcedure<symbol::vkCreateSampler             >();
		vkDestroySampler             = LoadDeviceProcedure<symbol::vkDestroySampler            >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                 VkDescriptorSetLayout                                 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateDescriptorSetLayout  = LoadDeviceProcedure<symbol::vkCreateDescriptorSetLayout >();
		vkDestroyDescriptorSetLayout = LoadDeviceProcedure<symbol::vkDestroyDescriptorSetLayout>();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkDescriptorPool                                    *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateDescriptorPool       = LoadDeviceProcedure<symbol::vkCreateDescriptorPool      >();
		vkDestroyDescriptorPool      = LoadDeviceProcedure<symbol::vkDestroyDescriptorPool     >();

		vkResetDescriptorPool        = LoadDeviceProcedure<symbol::vkResetDescriptorPool       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkDescriptorSet                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkAllocateDescriptorSets     = LoadDeviceProcedure<symbol::vkAllocateDescriptorSets    >();
		vkFreeDescriptorSets         = LoadDeviceProcedure<symbol::vkFreeDescriptorSets        >();

		vkUpdateDescriptorSets       = LoadDeviceProcedure<symbol::vkUpdateDescriptorSets      >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                      VkSwapchain                                      *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateSwapchainKHR         = LoadDeviceProcedure<symbol::vkCreateSwapchainKHR        >();
		vkDestroySwapchainKHR        = LoadDeviceProcedure<symbol::vkDestroySwapchainKHR       >();

		vkGetSwapchainImagesKHR      = LoadDeviceProcedure<symbol::vkGetSwapchainImagesKHR     >();
		vkAcquireNextImageKHR        = LoadDeviceProcedure<symbol::vkAcquireNextImageKHR       >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                     VkCommandPool                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkCreateCommandPool          = LoadDeviceProcedure<symbol::vkCreateCommandPool         >();
		vkDestroyCommandPool         = LoadDeviceProcedure<symbol::vkDestroyCommandPool        >();

		vkResetCommandPool           = LoadDeviceProcedure<symbol::vkResetCommandPool          >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vkGetDeviceQueue             = LoadDeviceProcedure<symbol::vkGetDeviceQueue            >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                        VkQueue                                        *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkQueueSubmit                = LoadDeviceProcedure<symbol::vkQueueSubmit               >();
		vkQueueWaitIdle              = LoadDeviceProcedure<symbol::vkQueueWaitIdle             >();

		vkQueuePresentKHR            = LoadDeviceProcedure<symbol::vkQueuePresentKHR           >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *                                   VkCommandBuffer                                     *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		vkAllocateCommandBuffers     = LoadDeviceProcedure<symbol::vkAllocateCommandBuffers    >();
		vkFreeCommandBuffers         = LoadDeviceProcedure<symbol::vkFreeCommandBuffers        >();

		vkBeginCommandBuffer         = LoadDeviceProcedure<symbol::vkBeginCommandBuffer        >();
		vkEndCommandBuffer           = LoadDeviceProcedure<symbol::vkEndCommandBuffer          >();

		vkCmdBindPipeline            = LoadDeviceProcedure<symbol::vkCmdBindPipeline           >();

		vkCmdDraw                    = LoadDeviceProcedure<symbol::vkCmdDraw                   >();

		vkCmdClearColorImage         = LoadDeviceProcedure<symbol::vkCmdClearColorImage        >();

		vkCmdPipelineBarrier         = LoadDeviceProcedure<symbol::vkCmdPipelineBarrier        >();

		vkCmdBeginRenderPass         = LoadDeviceProcedure<symbol::vkCmdBeginRenderPass        >();
		vkCmdEndRenderPass           = LoadDeviceProcedure<symbol::vkCmdEndRenderPass          >();
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
		physicalDevice { device.physicalDevice },
		vkDevice       { device.vkDevice       },

		vkDestroyDevice              { device.vkDestroyDevice              },
		vkDeviceWaitIdle             { device.vkDeviceWaitIdle             },
		vkCreateSemaphore            { device.vkCreateSemaphore            },
		vkDestroySemaphore           { device.vkDestroySemaphore           },
		vkCreateFence                { device.vkCreateFence                },
		vkDestroyFence               { device.vkDestroyFence               },
		vkGetFenceStatus             { device.vkGetFenceStatus             },
		vkResetFences                { device.vkResetFences                },
		vkWaitForFences              { device.vkWaitForFences              },
		vkCreateRenderPass           { device.vkCreateRenderPass           },
		vkDestroyRenderPass          { device.vkDestroyRenderPass          },
		vkCreateFramebuffer          { device.vkCreateFramebuffer          },
		vkDestroyFramebuffer         { device.vkDestroyFramebuffer         },
		vkCreateShaderModule         { device.vkCreateShaderModule         },
		vkDestroyShaderModule        { device.vkDestroyShaderModule        },
		vkCreatePipelineCache        { device.vkCreatePipelineCache        },
		vkDestroyPipelineCache       { device.vkDestroyPipelineCache       },
		vkCreatePipelineLayout       { device.vkCreatePipelineLayout       },
		vkDestroyPipelineLayout      { device.vkDestroyPipelineLayout      },
		vkCreateComputePipelines     { device.vkCreateComputePipelines     },
		vkCreateGraphicsPipelines    { device.vkCreateGraphicsPipelines    },
		vkDestroyPipeline            { device.vkDestroyPipeline            },
		vkAllocateMemory             { device.vkAllocateMemory             },
		vkFreeMemory                 { device.vkFreeMemory                 },
		vkMapMemory                  { device.vkMapMemory                  },
		vkUnmapMemory                { device.vkUnmapMemory                },
		vkCreateBuffer               { device.vkCreateBuffer               },
		vkDestroyBuffer              { device.vkDestroyBuffer              },
		vkBindBufferMemory           { device.vkBindBufferMemory           },
		vkCreateBufferView           { device.vkCreateBufferView           },
		vkDestroyBufferView          { device.vkDestroyBufferView          },
		vkCreateImage                { device.vkCreateImage                },
		vkDestroyImage               { device.vkDestroyImage               },
		vkBindImageMemory            { device.vkBindImageMemory            },
		vkGetImageSubresourceLayout  { device.vkGetImageSubresourceLayout  },
		vkCreateImageView            { device.vkCreateImageView            },
		vkDestroyImageView           { device.vkDestroyImageView           },
		vkCreateSampler              { device.vkCreateSampler              },
		vkDestroySampler             { device.vkDestroySampler             },
		vkCreateDescriptorSetLayout  { device.vkCreateDescriptorSetLayout  },
		vkDestroyDescriptorSetLayout { device.vkDestroyDescriptorSetLayout },
		vkCreateDescriptorPool       { device.vkCreateDescriptorPool       },
		vkDestroyDescriptorPool      { device.vkDestroyDescriptorPool      },
		vkResetDescriptorPool        { device.vkResetDescriptorPool        },
		vkAllocateDescriptorSets     { device.vkAllocateDescriptorSets     },
		vkFreeDescriptorSets         { device.vkFreeDescriptorSets         },
		vkUpdateDescriptorSets       { device.vkUpdateDescriptorSets       },
		vkCreateSwapchainKHR         { device.vkCreateSwapchainKHR         },
		vkDestroySwapchainKHR        { device.vkDestroySwapchainKHR        },
		vkGetSwapchainImagesKHR      { device.vkGetSwapchainImagesKHR      },
		vkAcquireNextImageKHR        { device.vkAcquireNextImageKHR        },
		vkCreateCommandPool          { device.vkCreateCommandPool          },
		vkDestroyCommandPool         { device.vkDestroyCommandPool         },
		vkResetCommandPool           { device.vkResetCommandPool           },
		vkGetDeviceQueue             { device.vkGetDeviceQueue             },
		vkQueueSubmit                { device.vkQueueSubmit                },
		vkQueueWaitIdle              { device.vkQueueWaitIdle              },
		vkQueuePresentKHR            { device.vkQueuePresentKHR            },
		vkAllocateCommandBuffers     { device.vkAllocateCommandBuffers     },
		vkFreeCommandBuffers         { device.vkFreeCommandBuffers         },
		vkBeginCommandBuffer         { device.vkBeginCommandBuffer         },
		vkEndCommandBuffer           { device.vkEndCommandBuffer           },
		vkCmdBindPipeline            { device.vkCmdBindPipeline            },
		vkCmdDraw                    { device.vkCmdDraw                    },
		vkCmdClearColorImage         { device.vkCmdClearColorImage         },
		vkCmdPipelineBarrier         { device.vkCmdPipelineBarrier         },
		vkCmdBeginRenderPass         { device.vkCmdBeginRenderPass         },
		vkCmdEndRenderPass           { device.vkCmdEndRenderPass           }
	{
		device.physicalDevice = nullptr;
		device.vkDevice       = VK_NULL_HANDLE;

		device.vkDestroyDevice              = nullptr;
		device.vkDeviceWaitIdle             = nullptr;
		device.vkCreateSemaphore            = nullptr;
		device.vkDestroySemaphore           = nullptr;
		device.vkCreateFence                = nullptr;
		device.vkDestroyFence               = nullptr;
		device.vkGetFenceStatus             = nullptr;
		device.vkResetFences                = nullptr;
		device.vkWaitForFences              = nullptr;
		device.vkCreateRenderPass           = nullptr;
		device.vkDestroyRenderPass          = nullptr;
		device.vkCreateFramebuffer          = nullptr;
		device.vkDestroyFramebuffer         = nullptr;
		device.vkCreateShaderModule         = nullptr;
		device.vkDestroyShaderModule        = nullptr;
		device.vkCreatePipelineCache        = nullptr;
		device.vkDestroyPipelineCache       = nullptr;
		device.vkCreatePipelineLayout       = nullptr;
		device.vkDestroyPipelineLayout      = nullptr;
		device.vkCreateComputePipelines     = nullptr;
		device.vkCreateGraphicsPipelines    = nullptr;
		device.vkDestroyPipeline            = nullptr;
		device.vkAllocateMemory             = nullptr;
		device.vkFreeMemory                 = nullptr;
		device.vkMapMemory                  = nullptr;
		device.vkUnmapMemory                = nullptr;
		device.vkCreateBuffer               = nullptr;
		device.vkDestroyBuffer              = nullptr;
		device.vkBindBufferMemory           = nullptr;
		device.vkCreateBufferView           = nullptr;
		device.vkDestroyBufferView          = nullptr;
		device.vkCreateImage                = nullptr;
		device.vkDestroyImage               = nullptr;
		device.vkBindImageMemory            = nullptr;
		device.vkGetImageSubresourceLayout  = nullptr;
		device.vkCreateImageView            = nullptr;
		device.vkDestroyImageView           = nullptr;
		device.vkCreateSampler              = nullptr;
		device.vkDestroySampler             = nullptr;
		device.vkCreateDescriptorSetLayout  = nullptr;
		device.vkDestroyDescriptorSetLayout = nullptr;
		device.vkCreateDescriptorPool       = nullptr;
		device.vkDestroyDescriptorPool      = nullptr;
		device.vkResetDescriptorPool        = nullptr;
		device.vkAllocateDescriptorSets     = nullptr;
		device.vkFreeDescriptorSets         = nullptr;
		device.vkUpdateDescriptorSets       = nullptr;
		device.vkCreateSwapchainKHR         = nullptr;
		device.vkDestroySwapchainKHR        = nullptr;
		device.vkGetSwapchainImagesKHR      = nullptr;
		device.vkAcquireNextImageKHR        = nullptr;
		device.vkCreateCommandPool          = nullptr;
		device.vkDestroyCommandPool         = nullptr;
		device.vkResetCommandPool           = nullptr;
		device.vkGetDeviceQueue             = nullptr;
		device.vkQueueSubmit                = nullptr;
		device.vkQueueWaitIdle              = nullptr;
		device.vkQueuePresentKHR            = nullptr;
		device.vkAllocateCommandBuffers     = nullptr;
		device.vkFreeCommandBuffers         = nullptr;
		device.vkBeginCommandBuffer         = nullptr;
		device.vkEndCommandBuffer           = nullptr;
		device.vkCmdBindPipeline            = nullptr;
		device.vkCmdDraw                    = nullptr;
		device.vkCmdClearColorImage         = nullptr;
		device.vkCmdPipelineBarrier         = nullptr;
		device.vkCmdBeginRenderPass         = nullptr;
		device.vkCmdEndRenderPass           = nullptr;
	}

	Device& Device::operator = (Device&& device)
	{
		if (vkDevice != VK_NULL_HANDLE)
		{
			const auto result = vkDeviceWaitIdle(vkDevice);
			assert(result == VK_SUCCESS);

			vkDestroyDevice(vkDevice, nullptr);
		}

		physicalDevice = device.physicalDevice;
		vkDevice       = device.vkDevice;

		vkDestroyDevice              = device.vkDestroyDevice;
		vkDeviceWaitIdle             = device.vkDeviceWaitIdle;
		vkCreateSemaphore            = device.vkCreateSemaphore;
		vkDestroySemaphore           = device.vkDestroySemaphore;
		vkCreateFence                = device.vkCreateFence;
		vkDestroyFence               = device.vkDestroyFence;
		vkGetFenceStatus             = device.vkGetFenceStatus;
		vkResetFences                = device.vkResetFences;
		vkWaitForFences              = device.vkWaitForFences;
		vkCreateRenderPass           = device.vkCreateRenderPass;
		vkDestroyRenderPass          = device.vkDestroyRenderPass;
		vkCreateFramebuffer          = device.vkCreateFramebuffer;
		vkDestroyFramebuffer         = device.vkDestroyFramebuffer;
		vkCreateShaderModule         = device.vkCreateShaderModule;
		vkDestroyShaderModule        = device.vkDestroyShaderModule;
		vkCreatePipelineCache        = device.vkCreatePipelineCache;
		vkDestroyPipelineCache       = device.vkDestroyPipelineCache;
		vkCreatePipelineLayout       = device.vkCreatePipelineLayout;
		vkDestroyPipelineLayout      = device.vkDestroyPipelineLayout;
		vkCreateComputePipelines     = device.vkCreateComputePipelines;
		vkCreateGraphicsPipelines    = device.vkCreateGraphicsPipelines;
		vkDestroyPipeline            = device.vkDestroyPipeline;
		vkAllocateMemory             = device.vkAllocateMemory;
		vkFreeMemory                 = device.vkFreeMemory;
		vkMapMemory                  = device.vkMapMemory;
		vkUnmapMemory                = device.vkUnmapMemory;
		vkCreateBuffer               = device.vkCreateBuffer;
		vkDestroyBuffer              = device.vkDestroyBuffer;
		vkBindBufferMemory           = device.vkBindBufferMemory;
		vkCreateBufferView           = device.vkCreateBufferView;
		vkDestroyBufferView          = device.vkDestroyBufferView;
		vkCreateImage                = device.vkCreateImage;
		vkDestroyImage               = device.vkDestroyImage;
		vkBindImageMemory            = device.vkBindImageMemory;
		vkGetImageSubresourceLayout  = device.vkGetImageSubresourceLayout;
		vkCreateImageView            = device.vkCreateImageView;
		vkDestroyImageView           = device.vkDestroyImageView;
		vkCreateSampler              = device.vkCreateSampler;
		vkDestroySampler             = device.vkDestroySampler;
		vkCreateDescriptorSetLayout  = device.vkCreateDescriptorSetLayout;
		vkDestroyDescriptorSetLayout = device.vkDestroyDescriptorSetLayout;
		vkCreateDescriptorPool       = device.vkCreateDescriptorPool;
		vkDestroyDescriptorPool      = device.vkDestroyDescriptorPool;
		vkResetDescriptorPool        = device.vkResetDescriptorPool;
		vkAllocateDescriptorSets     = device.vkAllocateDescriptorSets;
		vkFreeDescriptorSets         = device.vkFreeDescriptorSets;
		vkUpdateDescriptorSets       = device.vkUpdateDescriptorSets;
		vkCreateSwapchainKHR         = device.vkCreateSwapchainKHR;
		vkDestroySwapchainKHR        = device.vkDestroySwapchainKHR;
		vkGetSwapchainImagesKHR      = device.vkGetSwapchainImagesKHR;
		vkAcquireNextImageKHR        = device.vkAcquireNextImageKHR;
		vkCreateCommandPool          = device.vkCreateCommandPool;
		vkDestroyCommandPool         = device.vkDestroyCommandPool;
		vkResetCommandPool           = device.vkResetCommandPool;
		vkGetDeviceQueue             = device.vkGetDeviceQueue;
		vkQueueSubmit                = device.vkQueueSubmit;
		vkQueueWaitIdle              = device.vkQueueWaitIdle;
		vkQueuePresentKHR            = device.vkQueuePresentKHR;
		vkAllocateCommandBuffers     = device.vkAllocateCommandBuffers;
		vkFreeCommandBuffers         = device.vkFreeCommandBuffers;
		vkBeginCommandBuffer         = device.vkBeginCommandBuffer;
		vkEndCommandBuffer           = device.vkEndCommandBuffer;
		vkCmdBindPipeline            = device.vkCmdBindPipeline;
		vkCmdDraw                    = device.vkCmdDraw;
		vkCmdClearColorImage         = device.vkCmdClearColorImage;
		vkCmdPipelineBarrier         = device.vkCmdPipelineBarrier;
		vkCmdBeginRenderPass         = device.vkCmdBeginRenderPass;
		vkCmdEndRenderPass           = device.vkCmdEndRenderPass;

		device.physicalDevice = nullptr;
		device.vkDevice       = VK_NULL_HANDLE;

		device.vkDestroyDevice              = nullptr;
		device.vkDeviceWaitIdle             = nullptr;
		device.vkCreateSemaphore            = nullptr;
		device.vkDestroySemaphore           = nullptr;
		device.vkCreateFence                = nullptr;
		device.vkDestroyFence               = nullptr;
		device.vkGetFenceStatus             = nullptr;
		device.vkResetFences                = nullptr;
		device.vkWaitForFences              = nullptr;
		device.vkCreateRenderPass           = nullptr;
		device.vkDestroyRenderPass          = nullptr;
		device.vkCreateFramebuffer          = nullptr;
		device.vkDestroyFramebuffer         = nullptr;
		device.vkCreateShaderModule         = nullptr;
		device.vkDestroyShaderModule        = nullptr;
		device.vkCreatePipelineCache        = nullptr;
		device.vkDestroyPipelineCache       = nullptr;
		device.vkCreatePipelineLayout       = nullptr;
		device.vkDestroyPipelineLayout      = nullptr;
		device.vkCreateComputePipelines     = nullptr;
		device.vkCreateGraphicsPipelines    = nullptr;
		device.vkDestroyPipeline            = nullptr;
		device.vkAllocateMemory             = nullptr;
		device.vkFreeMemory                 = nullptr;
		device.vkMapMemory                  = nullptr;
		device.vkUnmapMemory                = nullptr;
		device.vkCreateBuffer               = nullptr;
		device.vkDestroyBuffer              = nullptr;
		device.vkBindBufferMemory           = nullptr;
		device.vkCreateBufferView           = nullptr;
		device.vkDestroyBufferView          = nullptr;
		device.vkCreateImage                = nullptr;
		device.vkDestroyImage               = nullptr;
		device.vkBindImageMemory            = nullptr;
		device.vkGetImageSubresourceLayout  = nullptr;
		device.vkCreateImageView            = nullptr;
		device.vkDestroyImageView           = nullptr;
		device.vkCreateSampler              = nullptr;
		device.vkDestroySampler             = nullptr;
		device.vkCreateDescriptorSetLayout  = nullptr;
		device.vkDestroyDescriptorSetLayout = nullptr;
		device.vkCreateDescriptorPool       = nullptr;
		device.vkDestroyDescriptorPool      = nullptr;
		device.vkResetDescriptorPool        = nullptr;
		device.vkAllocateDescriptorSets     = nullptr;
		device.vkFreeDescriptorSets         = nullptr;
		device.vkUpdateDescriptorSets       = nullptr;
		device.vkCreateSwapchainKHR         = nullptr;
		device.vkDestroySwapchainKHR        = nullptr;
		device.vkGetSwapchainImagesKHR      = nullptr;
		device.vkAcquireNextImageKHR        = nullptr;
		device.vkCreateCommandPool          = nullptr;
		device.vkDestroyCommandPool         = nullptr;
		device.vkResetCommandPool           = nullptr;
		device.vkGetDeviceQueue             = nullptr;
		device.vkQueueSubmit                = nullptr;
		device.vkQueueWaitIdle              = nullptr;
		device.vkQueuePresentKHR            = nullptr;
		device.vkAllocateCommandBuffers     = nullptr;
		device.vkFreeCommandBuffers         = nullptr;
		device.vkBeginCommandBuffer         = nullptr;
		device.vkEndCommandBuffer           = nullptr;
		device.vkCmdBindPipeline            = nullptr;
		device.vkCmdDraw                    = nullptr;
		device.vkCmdClearColorImage         = nullptr;
		device.vkCmdPipelineBarrier         = nullptr;
		device.vkCmdBeginRenderPass         = nullptr;
		device.vkCmdEndRenderPass           = nullptr;

		return *this;
	}

	void Device::WaitIdle () const
	{
		const auto result = vkDeviceWaitIdle(vkDevice);
		assert(result == VK_SUCCESS);
	}

	Queue
	Device::GetQueue (const uint32_t familyIndex, const uint32_t queueIndex) const
	{
		auto queue = VkQueue { VK_NULL_HANDLE };

		vkGetDeviceQueue(vkDevice, familyIndex, queueIndex, &queue);

		return Queue { *this, queue };
	}
}
