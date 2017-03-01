
#ifndef VULKAN_COMMAND_BUFFER
#define VULKAN_COMMAND_BUFFER

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanDevice;
	struct VulkanCommandPool;
//	struct VulkanRenderPass;
//	struct VulkanFramebuffer;
	struct VulkanPipeline;

	struct VulkanCommandBuffer
	{
		VkCommandBuffer commandBuffer;
		VkCommandPool   commandPool;
		VkDevice        device;

		PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
		PFN_vkFreeCommandBuffers     vkFreeCommandBuffers;
		PFN_vkBeginCommandBuffer     vkBeginCommandBuffer;
		PFN_vkEndCommandBuffer       vkEndCommandBuffer;
		PFN_vkCmdClearColorImage     vkCmdClearColorImage;
		PFN_vkCmdPipelineBarrier     vkCmdPipelineBarrier;
		PFN_vkCmdBeginRenderPass     vkCmdBeginRenderPass;
		PFN_vkCmdEndRenderPass       vkCmdEndRenderPass;
		PFN_vkCmdBindPipeline        vkCmdBindPipeline;
		PFN_vkCmdDraw                vkCmdDraw;

		VulkanCommandBuffer();

		VulkanCommandBuffer
		(
			const VulkanDevice& device,
			const VulkanCommandPool& commandPool,
			const VkCommandBufferLevel level
		);

		~VulkanCommandBuffer();

		VulkanCommandBuffer(VulkanCommandBuffer&& commandBuffer);

		VulkanCommandBuffer(const VulkanCommandBuffer& commandBuffer) = delete;

		VulkanCommandBuffer&
		operator =(VulkanCommandBuffer&& commandBuffer);

		VulkanCommandBuffer&
		operator =(const VulkanCommandBuffer& commandBuffer) = delete;

		void BeginRecording();

		void EndRecording();

		void RecordCommandPipelineBarrier
		(
			const VkPipelineStageFlags  sourcePipelineStageMask,
			const VkPipelineStageFlags  destinationPipelineStageMask,
			const VkImageMemoryBarrier& imageMemoryBarrier
		);

		void RecordCommandClearImage
		(
			VkImage&                       image,
			const VkImageLayout            imageLayout,
			const VkImageSubresourceRange& imageSubresourceRange,
			const VkClearColorValue&       clearColor
		);

//		void RecordCommandBeginRenderPass
//		(
//			const VulkanRenderPass&  renderPass,
//			const VulkanFramebuffer& framebuffer,
//			const VkRect2D&          renderArea,
//			const VkClearValue&      clear
//		);

		void RecordCommandEndRenderPass();

		void RecordCommandBindPipeline(const VulkanPipeline& pipeline);

		void RecordCommandDraw
		(
			uint32_t vertexCount, uint32_t instanceCount,
			uint32_t firstVertex, uint32_t firstInstance
		);
	};
}

#endif
