
#ifndef VULKAN_DEVICE
#define VULKAN_DEVICE

#include <vulkan/Instance.h>
#include <vulkan/Queue.h>

#include <cassert>
#include <vector>

namespace vk
{
	struct Device
	{
		struct CreateInfo
		{
			VkDeviceCreateInfo createInfo;

			CreateInfo
			(
				const std::vector<Queue::CreateInfo>& queueCreateInfos,
				const std::vector<const char*>&       layers,
				const std::vector<const char*>&       extensions,
				const VkPhysicalDeviceFeatures&       enabledFeatures
			);
		};

		const PhysicalDevice* physicalDevice;
		VkDevice              vkDevice;

		/*
		┌────────────────────────────────────────────────────────────────────────┐
		│                                 VkDevice                               │
		├────────────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkDestroyDevice                vkDestroyDevice;                /*│
		│                                                                        │
		│*/ PFN_vkDeviceWaitIdle               vkDeviceWaitIdle;               /*│
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkSemaphore                            │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateSemaphore            vkCreateSemaphore;            /*│ │
		│ │*/ PFN_vkDestroySemaphore           vkDestroySemaphore;           /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                               VkFence                              │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateFence                vkCreateFence;                /*│ │
		│ │*/ PFN_vkDestroyFence               vkDestroyFence;               /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkGetFenceStatus             vkGetFenceStatus;             /*│ │
		│ │*/ PFN_vkResetFences                vkResetFences;                /*│ │
		│ │*/ PFN_vkWaitForFences              vkWaitForFences;              /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkQueryPool                            │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateQueryPool            vkCreateQueryPool;            /*│ │
		│ │*/ PFN_vkDestroyQueryPool           vkDestroyQueryPool;           /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkGetQueryPoolResults        vkGetQueryPoolResults;        /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkRenderPass                           │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateRenderPass           vkCreateRenderPass;           /*│ │
		│ │*/ PFN_vkDestroyRenderPass          vkDestroyRenderPass;          /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                            VkFramebuffer                           │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateFramebuffer          vkCreateFramebuffer;          /*│ │
		│ │*/ PFN_vkDestroyFramebuffer         vkDestroyFramebuffer;         /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                            VkShaderModule                          │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateShaderModule         vkCreateShaderModule;         /*│ │
		│ │*/ PFN_vkDestroyShaderModule        vkDestroyShaderModule;        /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                           VkPipelineCache                          │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreatePipelineCache        vkCreatePipelineCache;        /*│ │
		│ │*/ PFN_vkDestroyPipelineCache       vkDestroyPipelineCache;       /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                           VkPipelineLayout                         │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreatePipelineLayout       vkCreatePipelineLayout;       /*│ │
		│ │*/ PFN_vkDestroyPipelineLayout      vkDestroyPipelineLayout;      /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                              VkPipeline                            │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateComputePipelines     vkCreateComputePipelines;     /*│ │
		│ │*/ PFN_vkCreateGraphicsPipelines    vkCreateGraphicsPipelines;    /*│ │
		│ │*/ PFN_vkDestroyPipeline            vkDestroyPipeline;            /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                               VkMemory                             │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkAllocateMemory             vkAllocateMemory;             /*│ │
		│ │*/ PFN_vkFreeMemory                 vkFreeMemory;                 /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkMapMemory                  vkMapMemory;                  /*│ │
		│ │*/ PFN_vkUnmapMemory                vkUnmapMemory;                /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                               VkBuffer                             │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateBuffer               vkCreateBuffer;               /*│ │
		│ │*/ PFN_vkDestroyBuffer              vkDestroyBuffer;              /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkBindBufferMemory           vkBindBufferMemory;           /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkBufferView                           │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateBufferView           vkCreateBufferView;           /*│ │
		│ │*/ PFN_vkDestroyBufferView          vkDestroyBufferView;          /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                               VkImage                              │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateImage                vkCreateImage;                /*│ │
		│ │*/ PFN_vkDestroyImage               vkDestroyImage;               /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkBindImageMemory            vkBindImageMemory;            /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkGetImageSubresourceLayout  vkGetImageSubresourceLayout;  /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkImageView                            │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateImageView            vkCreateImageView;            /*│ │
		│ │*/ PFN_vkDestroyImageView           vkDestroyImageView;           /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                              VkSampler                             │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateSampler              vkCreateSampler;              /*│ │
		│ │*/ PFN_vkDestroySampler             vkDestroySampler;             /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                        VkDescriptorSetLayout                       │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateDescriptorSetLayout  vkCreateDescriptorSetLayout;  /*│ │
		│ │*/ PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout; /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                           VkDescriptorPool                         │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateDescriptorPool       vkCreateDescriptorPool;       /*│ │
		│ │*/ PFN_vkDestroyDescriptorPool      vkDestroyDescriptorPool;      /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkResetDescriptorPool        vkResetDescriptorPool;        /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                           VkDescriptorSet                          │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkAllocateDescriptorSets     vkAllocateDescriptorSets;     /*│ │
		│ │*/ PFN_vkFreeDescriptorSets         vkFreeDescriptorSets;         /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkUpdateDescriptorSets       vkUpdateDescriptorSets;       /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                             VkSwapchain                            │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateSwapchainKHR         vkCreateSwapchainKHR;         /*│ │
		│ │*/ PFN_vkDestroySwapchainKHR        vkDestroySwapchainKHR;        /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkGetSwapchainImagesKHR      vkGetSwapchainImagesKHR;      /*│ │
		│ │*/ PFN_vkAcquireNextImageKHR        vkAcquireNextImageKHR;        /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│ ┌────────────────────────────────────────────────────────────────────┐ │
		│ │                            VkCommandPool                           │ │
		│ ├────────────────────────────────────────────────────────────────────┤ │
		│ │*/ PFN_vkCreateCommandPool          vkCreateCommandPool;          /*│ │
		│ │*/ PFN_vkDestroyCommandPool         vkDestroyCommandPool;         /*│ │
		│ │                                                                    │ │
		│ │*/ PFN_vkResetCommandPool           vkResetCommandPool;           /*│ │
		│ └────────────────────────────────────────────────────────────────────┘ │
		│                                                                        │
		│*/ PFN_vkGetDeviceQueue               vkGetDeviceQueue;               /*│
		└────────────────────────────────────────────────────────────────────────┘

		┌────────────────────────────────────────────────────────────────────────┐
		│                                 VkQueue                                │
		├────────────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkQueueSubmit                  vkQueueSubmit;                  /*│
		│*/ PFN_vkQueueWaitIdle                vkQueueWaitIdle;                /*│
		│                                                                        │
		│*/ PFN_vkQueuePresentKHR              vkQueuePresentKHR;              /*│
		└────────────────────────────────────────────────────────────────────────┘

		┌────────────────────────────────────────────────────────────────────────┐
		│                             VkCommandBuffer                            │
		├────────────────────────────────────────────────────────────────────────┤
		│*/ PFN_vkAllocateCommandBuffers       vkAllocateCommandBuffers;       /*│
		│*/ PFN_vkFreeCommandBuffers           vkFreeCommandBuffers;           /*│
		│                                                                        │
		│*/ PFN_vkBeginCommandBuffer           vkBeginCommandBuffer;           /*│
		│*/ PFN_vkEndCommandBuffer             vkEndCommandBuffer;             /*│
		│                                                                        │
		│*/ PFN_vkCmdBindPipeline              vkCmdBindPipeline;              /*│
		│                                                                        │
		│*/ PFN_vkCmdDraw                      vkCmdDraw;                      /*│
		│                                                                        │
		│*/ PFN_vkCmdClearColorImage           vkCmdClearColorImage;           /*│
		│                                                                        │
		│*/ PFN_vkCmdResetQueryPool            vkCmdResetQueryPool;            /*│
		│*/ PFN_vkCmdBeginQuery                vkCmdBeginQuery;                /*│
		│*/ PFN_vkCmdEndQuery                  vkCmdEndQuery;                  /*│
		│*/ PFN_vkCmdWriteTimestamp            vkCmdWriteTimestamp;            /*│
		│*/ PFN_vkCmdCopyQueryPoolResults      vkCmdCopyQueryPoolResults;      /*│
		│                                                                        │
		│*/ PFN_vkCmdPipelineBarrier           vkCmdPipelineBarrier;           /*│
		│                                                                        │
		│*/ PFN_vkCmdBeginRenderPass           vkCmdBeginRenderPass;           /*│
		│*/ PFN_vkCmdEndRenderPass             vkCmdEndRenderPass;             /*│
		└────────────────────────────────────────────────────────────────────────┘
		*/

		Device
		(
			const PhysicalDevice& physicalDevice,
			const CreateInfo&     createInfo
		);
		~Device ();

		Device (Device&& device);
		Device (const Device& device) = delete;

		Device& operator = (Device&& device);
		Device& operator = (const Device& device) = delete;

		template <typename Procedure>
		typename Procedure::type LoadDeviceProcedure () const
		{
			const auto instance = physicalDevice->instance;

			const auto procedureAddress = instance->vkGetDeviceProcAddr
			(
				vkDevice, Procedure::name
			);
			assert(procedureAddress != nullptr);

			return reinterpret_cast<typename Procedure::type>(procedureAddress);
		}

		void WaitIdle() const;

		Queue
		GetQueue (const uint32_t familyIndex, const uint32_t queueIndex) const;
	};
}

#endif
