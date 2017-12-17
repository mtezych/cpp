
#ifndef VULKAN_COMMAND_BUFFER
#define VULKAN_COMMAND_BUFFER

#include <vulkan/vulkan.h>

namespace vk
{
	struct CommandPool;
	struct RenderPass;
	struct Framebuffer;
	struct Pipeline;
	struct Image;
	struct QueryPool;

	struct CommandBuffer
	{
		const CommandPool* commandPool;
		VkCommandBuffer    vkCommandBuffer;

		CommandBuffer
		(
			const CommandPool& commandPool, const VkCommandBufferLevel level
		);

		~CommandBuffer ();

		CommandBuffer (CommandBuffer&& commandBuffer);
		CommandBuffer (const CommandBuffer& commandBuffer) = delete;

		CommandBuffer& operator = (CommandBuffer&& commandBuffer);
		CommandBuffer& operator = (const CommandBuffer& commandBuffer) = delete;

		void BeginRecording ();
		void EndRecording ();

		void RecordCommandPipelineBarrier
		(
			const VkPipelineStageFlags  sourcePipelineStageMask,
			const VkPipelineStageFlags  destinationPipelineStageMask,
			const VkImageMemoryBarrier& imageMemoryBarrier
		);

		void RecordCommandClearImage
		(
			const Image&                   image,
			const VkImageLayout            imageLayout,
			const VkImageSubresourceRange& imageSubresourceRange,
			const VkClearColorValue&       clearColor
		);

		void RecordCommandBeginRenderPass
		(
			const RenderPass&   renderPass,
			const Framebuffer&  framebuffer,
			const VkRect2D&     renderArea,
			const VkClearValue& clear
		);

		void RecordCommandEndRenderPass ();

		void RecordCommandBindPipeline (const Pipeline& pipeline);

		void RecordCommandDraw
		(
			uint32_t vertexCount, uint32_t instanceCount,
			uint32_t firstVertex, uint32_t firstInstance
		);

		void RecordCommandResetQueryPool
		(
			const QueryPool& queryPool,
			const uint32_t firstQuery, const uint32_t queryCount
		);

		void RecordCommandBeginQuery
		(
			const QueryPool& queryPool, const uint32_t queryIndex,
			const VkQueryControlFlags controlFlags = 0
		);

		void RecordCommandEndQuery
		(
			const QueryPool& queryPool, const uint32_t queryIndex
		);

		void RecordCommandWriteTimestamp
		(
			const QueryPool& queryPool, const uint32_t queryIndex,
			const VkPipelineStageFlagBits pipelineStage
		);
	};
}

#endif
