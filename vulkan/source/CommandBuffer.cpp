
#include <vulkan/CommandBuffer.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/CommandPool.h>
#include <vulkan/RenderPass.h>
#include <vulkan/Framebuffer.h>
#include <vulkan/Pipeline.h>
#include <vulkan/Image.h>
#include <vulkan/QueryPool.h>

#include <cassert>

namespace vk
{
	CommandBuffer::CommandBuffer
	(
		const CommandPool&         commandPool,
		const VkCommandBufferLevel level
	):
		commandPool     { &commandPool },
		vkCommandBuffer { VK_NULL_HANDLE }
	{
		const auto allocateInfo = VkCommandBufferAllocateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			commandPool.vkCommandPool,
			level,
			1
		};
		const auto result = commandPool.device->vkAllocateCommandBuffers
		(
			commandPool.device->vkDevice,
			&allocateInfo,
			&vkCommandBuffer
		);
		assert(result == VK_SUCCESS);
	}

	CommandBuffer::~CommandBuffer ()
	{
		if (vkCommandBuffer != VK_NULL_HANDLE)
		{
			commandPool->device->vkFreeCommandBuffers
			(
				commandPool->device->vkDevice,
				commandPool->vkCommandPool,
				1, &vkCommandBuffer
			);
		}
	}

	CommandBuffer::CommandBuffer (CommandBuffer&& commandBuffer)
	:
		commandPool     { commandBuffer.commandPool     },
		vkCommandBuffer { commandBuffer.vkCommandBuffer }
	{
		commandBuffer.commandPool     = nullptr;
		commandBuffer.vkCommandBuffer = VK_NULL_HANDLE;
	}

	CommandBuffer& CommandBuffer::operator = (CommandBuffer&& commandBuffer)
	{
		if (vkCommandBuffer != VK_NULL_HANDLE)
		{
			commandPool->device->vkFreeCommandBuffers
			(
				commandPool->device->vkDevice,
				commandPool->vkCommandPool,
				1, &vkCommandBuffer
			);
		}

		commandPool     = commandBuffer.commandPool;
		vkCommandBuffer = commandBuffer.vkCommandBuffer;

		commandBuffer.commandPool     = nullptr;
		commandBuffer.vkCommandBuffer = VK_NULL_HANDLE;

		return *this;
	}

	void CommandBuffer::BeginRecording ()
	{
		const auto beginInfo = VkCommandBufferBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			nullptr,
		};
		const auto result = commandPool->device->vkBeginCommandBuffer
		(
			vkCommandBuffer, &beginInfo
		);
		assert(result == VK_SUCCESS);
	}

	void CommandBuffer::EndRecording ()
	{
		const auto result = commandPool->device->vkEndCommandBuffer
		(
			vkCommandBuffer
		);
		assert(result == VK_SUCCESS);
	}

	void CommandBuffer::RecordCommandPipelineBarrier
	(
		const VkPipelineStageFlags  sourcePipelineStageMask,
		const VkPipelineStageFlags  destinationPipelineStageMask,
		const VkImageMemoryBarrier& imageMemoryBarrier
	)
	{
		commandPool->device->vkCmdPipelineBarrier
		(
			vkCommandBuffer,
			sourcePipelineStageMask,
			destinationPipelineStageMask,
			VkDependencyFlags { 0 },
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier
		);
	}

	void CommandBuffer::RecordCommandClearImage
	(
		const Image&                   image,
		const VkImageLayout            imageLayout,
		const VkImageSubresourceRange& imageSubresourceRange,
		const VkClearColorValue&       clearColor
	)
	{
		commandPool->device->vkCmdClearColorImage
		(
			vkCommandBuffer,
			image.vkImage, imageLayout,
			&clearColor,
			1, &imageSubresourceRange
		);
	}

	void CommandBuffer::RecordCommandBeginRenderPass
	(
		const RenderPass&   renderPass,
		const Framebuffer&  framebuffer,
		const VkRect2D&     renderArea,
		const VkClearValue& clear
	)
	{
		const auto beginInfo = VkRenderPassBeginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			renderPass.vkRenderPass,
			framebuffer.vkFramebuffer,
			renderArea,
			1, &clear
		};
		commandPool->device->vkCmdBeginRenderPass
		(
			vkCommandBuffer,
			&beginInfo,
			VkSubpassContents { VK_SUBPASS_CONTENTS_INLINE }
		);
	}

	void CommandBuffer::RecordCommandEndRenderPass ()
	{
		commandPool->device->vkCmdEndRenderPass(vkCommandBuffer);
	}

	void CommandBuffer::RecordCommandBindPipeline (const Pipeline& pipeline)
	{
		commandPool->device->vkCmdBindPipeline
		(
			vkCommandBuffer,
			pipeline.vkBindPoint,
			pipeline.vkPipeline
		);
	}

	void CommandBuffer::RecordCommandDraw
	(
		uint32_t vertexCount, uint32_t instanceCount,
		uint32_t firstVertex, uint32_t firstInstance
	)
	{
		commandPool->device->vkCmdDraw
		(
			vkCommandBuffer,
			vertexCount, instanceCount,
			firstVertex, firstInstance
		);
	}

	void CommandBuffer::RecordCommandResetQueryPool
	(
		const QueryPool& queryPool,
		const uint32_t firstQuery, const uint32_t queryCount
	)
	{
		commandPool->device->vkCmdResetQueryPool
		(
			vkCommandBuffer,
			queryPool.vkQueryPool,
			firstQuery, queryCount
		);
	}

	void CommandBuffer::RecordCommandBeginQuery
	(
		const QueryPool& queryPool, const uint32_t queryIndex,
		const VkQueryControlFlags controlFlags
	)
	{
//		assert
//		(
//			(controlFlags == VK_QUERY_CONTROL_PRECISE_BIT) &&
//			(queryPool.type == VK_QUERY_TYPE_OCCLUSION)
//		);

		commandPool->device->vkCmdBeginQuery
		(
			vkCommandBuffer,
			queryPool.vkQueryPool, queryIndex,
			controlFlags
		);
	}

	void CommandBuffer::RecordCommandEndQuery
	(
		const QueryPool& queryPool, const uint32_t queryIndex
	)
	{
		commandPool->device->vkCmdEndQuery
		(
			vkCommandBuffer,
			queryPool.vkQueryPool, queryIndex
		);
	}

	void CommandBuffer::RecordCommandWriteTimestamp
	(
		const QueryPool& queryPool, const uint32_t queryIndex,
		const VkPipelineStageFlagBits pipelineStage
	)
	{
		commandPool->device->vkCmdWriteTimestamp
		(
			vkCommandBuffer,
			pipelineStage,
			queryPool.vkQueryPool, queryIndex
		);
	}
}
