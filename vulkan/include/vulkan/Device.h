
#ifndef VULKAN_DEVICE
#define VULKAN_DEVICE

#include <vulkan/Instance.h>
#include <vulkan/Queue.h>

#include <cassert>

namespace vk
{
	struct Device
	{
		const Instance* instance;
		VkDevice        vkDevice;

		/*
		┌──────────────────────────────────────────────────────────────────┐
		│                             VkDevice                             │
		├──────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkDestroyDevice             vkDestroyDevice;             /*│
		│                                                                  │
		│*/ PFN_vkDeviceWaitIdle            vkDeviceWaitIdle;            /*│
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                         VkSemaphore                          │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateSemaphore         vkCreateSemaphore;         /*│ │
		│ │*/ PFN_vkDestroySemaphore        vkDestroySemaphore;        /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                         VkRenderPass                         │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateRenderPass        vkCreateRenderPass;        /*│ │
		│ │*/ PFN_vkDestroyRenderPass       vkDestroyRenderPass;       /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                        VkFramebuffer                         │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateFramebuffer       vkCreateFramebuffer;       /*│ │
		│ │*/ PFN_vkDestroyFramebuffer      vkDestroyFramebuffer;      /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                        VkShaderModule                        │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateShaderModule      vkCreateShaderModule;      /*│ │
		│ │*/ PFN_vkDestroyShaderModule     vkDestroyShaderModule;     /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                        VkPipelineCache                       │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreatePipelineCache     vkCreatePipelineCache;     /*│ │
		│ │*/ PFN_vkDestroyPipelineCache    vkDestroyPipelineCache;    /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                       VkPipelineLayout                       │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreatePipelineLayout    vkCreatePipelineLayout;    /*│ │
		│ │*/ PFN_vkDestroyPipelineLayout   vkDestroyPipelineLayout;   /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                          VkPipeline                          │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateComputePipelines  vkCreateComputePipelines;  /*│ │
		│ │*/ PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines; /*│ │
		│ │*/ PFN_vkDestroyPipeline         vkDestroyPipeline;         /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                           VkImage                            │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateImage             vkCreateImage;             /*│ │
		│ │*/ PFN_vkDestroyImage            vkDestroyImage;            /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                         VkImageView                          │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateImageView         vkCreateImageView;         /*│ │
		│ │*/ PFN_vkDestroyImageView        vkDestroyImageView;        /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                         VkSwapchain                          │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateSwapchainKHR      vkCreateSwapchainKHR;      /*│ │
		│ │*/ PFN_vkDestroySwapchainKHR     vkDestroySwapchainKHR;     /*│ │
		│ │                                                              │ │
		│ │*/ PFN_vkGetSwapchainImagesKHR   vkGetSwapchainImagesKHR;   /*│ │
		│ │*/ PFN_vkAcquireNextImageKHR     vkAcquireNextImageKHR;     /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│ ┌──────────────────────────────────────────────────────────────┐ │
		│ │                         VkCommandPool                        │ │
		│ ├──────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateCommandPool       vkCreateCommandPool;       /*│ │
		│ │*/ PFN_vkDestroyCommandPool      vkDestroyCommandPool;      /*│ │
		│ └──────────────────────────────────────────────────────────────┘ │
		│                                                                  │
		│*/ PFN_vkGetDeviceQueue            vkGetDeviceQueue;            /*│
		└──────────────────────────────────────────────────────────────────┘

		┌──────────────────────────────────────────────────────────────────┐
		│                             VkQueue                              │
		├──────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkQueueSubmit               vkQueueSubmit;               /*│
		│*/ PFN_vkQueueWaitIdle             vkQueueWaitIdle;             /*│
		│                                                                  │
		│*/ PFN_vkQueuePresentKHR           vkQueuePresentKHR;           /*│
		└──────────────────────────────────────────────────────────────────┘

		┌──────────────────────────────────────────────────────────────────┐
		│                         VkCommandBuffer                          │
		├──────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkAllocateCommandBuffers    vkAllocateCommandBuffers;    /*│
		│*/ PFN_vkFreeCommandBuffers        vkFreeCommandBuffers;        /*│
		│                                                                  │
		│*/ PFN_vkBeginCommandBuffer        vkBeginCommandBuffer;        /*│
		│*/ PFN_vkEndCommandBuffer          vkEndCommandBuffer;          /*│
		│                                                                  │
		│*/ PFN_vkCmdBindPipeline           vkCmdBindPipeline;           /*│
		│                                                                  │
		│*/ PFN_vkCmdDraw                   vkCmdDraw;                   /*│
		│                                                                  │
		│*/ PFN_vkCmdClearColorImage        vkCmdClearColorImage;        /*│
		│                                                                  │
		│*/ PFN_vkCmdPipelineBarrier        vkCmdPipelineBarrier;        /*│
		│                                                                  │
		│*/ PFN_vkCmdBeginRenderPass        vkCmdBeginRenderPass;        /*│
		│*/ PFN_vkCmdEndRenderPass          vkCmdEndRenderPass;          /*│
		└──────────────────────────────────────────────────────────────────┘
		*/

		Device
		(
			const PhysicalDevice&     physicalDevice,
			const VkDeviceCreateInfo& deviceCreateInfo
		);
		~Device();

		Device(Device&& device);
		Device(const Device& device) = delete;

		Device& operator = (Device&& device);
		Device& operator = (const Device& device) = delete;

		template <typename Procedure>
		typename Procedure::type LoadDeviceProcedure() const
		{
			const auto procedureAddress = instance->vkGetDeviceProcAddr
			(
				vkDevice, Procedure::name
			);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		Queue
		Queues(const uint32_t familyIndex, const uint32_t queueIndex) const;
	};
}

#endif
