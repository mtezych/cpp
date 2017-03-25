
#include "CommandBuffer.h"

#include "Symbols.h"
#include "Device.h"
#include "CommandPool.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "Pipeline.h"

#include <cassert>

namespace vk
{
	CommandBuffer::CommandBuffer()
	:
		commandPool     { nullptr },
		vkCommandBuffer { VK_NULL_HANDLE },

		vkAllocateCommandBuffers { nullptr },
		vkFreeCommandBuffers     { nullptr },
		vkBeginCommandBuffer     { nullptr },
		vkEndCommandBuffer       { nullptr },
		vkCmdClearColorImage     { nullptr },
		vkCmdPipelineBarrier     { nullptr },
		vkCmdBeginRenderPass     { nullptr },
		vkCmdEndRenderPass       { nullptr },
		vkCmdBindPipeline        { nullptr },
		vkCmdDraw                { nullptr }
	{
	}

	CommandBuffer::CommandBuffer
	(
		const CommandPool&         commandPool,
		const VkCommandBufferLevel level
	):
		commandPool     { &commandPool },
		vkCommandBuffer { VK_NULL_HANDLE },

		vkAllocateCommandBuffers
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkAllocateCommandBuffers>()
		},
		vkFreeCommandBuffers
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkFreeCommandBuffers>()
		},
		vkBeginCommandBuffer
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkBeginCommandBuffer>()
		},
		vkEndCommandBuffer
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkEndCommandBuffer>()
		},
		vkCmdClearColorImage
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdClearColorImage>()
		},
		vkCmdPipelineBarrier
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdPipelineBarrier>()
		},
		vkCmdBeginRenderPass
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdBeginRenderPass>()
		},
		vkCmdEndRenderPass
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdEndRenderPass>()
		},
		vkCmdBindPipeline
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdBindPipeline>()
		},
		vkCmdDraw
		{
			commandPool.device->LoadDeviceProcedure<symbol::vkCmdDraw>()
		}
	{
		const auto allocateInfo = VkCommandBufferAllocateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			commandPool.vkCommandPool,
			level,
			1
		};
		const auto result = vkAllocateCommandBuffers
		(
			commandPool.device->vkDevice,
			&allocateInfo,
			&vkCommandBuffer
		);
		assert(result == VK_SUCCESS);
	}

	CommandBuffer::~CommandBuffer()
	{
		if (vkCommandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers
			(
				commandPool->device->vkDevice,
				commandPool->vkCommandPool,
				1, &vkCommandBuffer
			);
		}
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& commandBuffer)
	:
		commandPool     { commandBuffer.commandPool     },
		vkCommandBuffer { commandBuffer.vkCommandBuffer },

		vkAllocateCommandBuffers { commandBuffer.vkAllocateCommandBuffers },
		vkFreeCommandBuffers     { commandBuffer.vkFreeCommandBuffers     },
		vkBeginCommandBuffer     { commandBuffer.vkBeginCommandBuffer     },
		vkEndCommandBuffer       { commandBuffer.vkEndCommandBuffer       },
		vkCmdClearColorImage     { commandBuffer.vkCmdClearColorImage     },
		vkCmdPipelineBarrier     { commandBuffer.vkCmdPipelineBarrier     },
		vkCmdBeginRenderPass     { commandBuffer.vkCmdBeginRenderPass     },
		vkCmdEndRenderPass       { commandBuffer.vkCmdEndRenderPass       },
		vkCmdBindPipeline        { commandBuffer.vkCmdBindPipeline        },
		vkCmdDraw                { commandBuffer.vkCmdDraw                }
	{
		commandBuffer.commandPool     = nullptr;
		commandBuffer.vkCommandBuffer = VK_NULL_HANDLE;

		commandBuffer.vkAllocateCommandBuffers = nullptr;
		commandBuffer.vkFreeCommandBuffers     = nullptr;
		commandBuffer.vkBeginCommandBuffer     = nullptr;
		commandBuffer.vkEndCommandBuffer       = nullptr;
		commandBuffer.vkCmdClearColorImage     = nullptr;
		commandBuffer.vkCmdPipelineBarrier     = nullptr;
		commandBuffer.vkCmdBeginRenderPass     = nullptr;
		commandBuffer.vkCmdEndRenderPass       = nullptr;
		commandBuffer.vkCmdBindPipeline        = nullptr;
		commandBuffer.vkCmdDraw                = nullptr;
	}

	CommandBuffer& CommandBuffer::operator =(CommandBuffer&& commandBuffer)
	{
		if (vkCommandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers
			(
				commandPool->device->vkDevice,
				commandPool->vkCommandPool,
				1, &vkCommandBuffer
			);
		}

		commandPool     = commandBuffer.commandPool;
		vkCommandBuffer = commandBuffer.vkCommandBuffer;

		vkAllocateCommandBuffers = commandBuffer.vkAllocateCommandBuffers;
		vkFreeCommandBuffers     = commandBuffer.vkFreeCommandBuffers;
		vkBeginCommandBuffer     = commandBuffer.vkBeginCommandBuffer;
		vkEndCommandBuffer       = commandBuffer.vkEndCommandBuffer;
		vkCmdClearColorImage     = commandBuffer.vkCmdClearColorImage;
		vkCmdPipelineBarrier     = commandBuffer.vkCmdPipelineBarrier;
		vkCmdBeginRenderPass     = commandBuffer.vkCmdBeginRenderPass;
		vkCmdEndRenderPass       = commandBuffer.vkCmdEndRenderPass;
		vkCmdBindPipeline        = commandBuffer.vkCmdBindPipeline;
		vkCmdDraw                = commandBuffer.vkCmdDraw;

		commandBuffer.commandPool     = nullptr;
		commandBuffer.vkCommandBuffer = VK_NULL_HANDLE;

		commandBuffer.vkAllocateCommandBuffers = nullptr;
		commandBuffer.vkFreeCommandBuffers     = nullptr;
		commandBuffer.vkBeginCommandBuffer     = nullptr;
		commandBuffer.vkEndCommandBuffer       = nullptr;
		commandBuffer.vkCmdClearColorImage     = nullptr;
		commandBuffer.vkCmdPipelineBarrier     = nullptr;
		commandBuffer.vkCmdBeginRenderPass     = nullptr;
		commandBuffer.vkCmdEndRenderPass       = nullptr;
		commandBuffer.vkCmdBindPipeline        = nullptr;
		commandBuffer.vkCmdDraw                = nullptr;

		return *this;
	}

	void CommandBuffer::BeginRecording()
	{
		const auto beginInfo = VkCommandBufferBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			nullptr,
		};
		const auto result = vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);
		assert(result == VK_SUCCESS);
	}

	void CommandBuffer::EndRecording()
	{
		const auto result = vkEndCommandBuffer(vkCommandBuffer);
		assert(result == VK_SUCCESS);
	}

	void CommandBuffer::RecordCommandPipelineBarrier
	(
		const VkPipelineStageFlags  sourcePipelineStageMask,
		const VkPipelineStageFlags  destinationPipelineStageMask,
		const VkImageMemoryBarrier& imageMemoryBarrier
	)
	{
		vkCmdPipelineBarrier
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
		VkImage&                       image,
		const VkImageLayout            imageLayout,
		const VkImageSubresourceRange& imageSubresourceRange,
		const VkClearColorValue&       clearColor
	)
	{
		vkCmdClearColorImage
		(
			vkCommandBuffer,
			image, imageLayout,
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
		vkCmdBeginRenderPass
		(
			vkCommandBuffer,
			&beginInfo,
			VkSubpassContents { VK_SUBPASS_CONTENTS_INLINE }
		);
	}

	void CommandBuffer::RecordCommandEndRenderPass()
	{
		vkCmdEndRenderPass(vkCommandBuffer);
	}

	void CommandBuffer::RecordCommandBindPipeline(const Pipeline& pipeline)
	{
		vkCmdBindPipeline
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
		vkCmdDraw
		(
			vkCommandBuffer,
			vertexCount, instanceCount,
			firstVertex, firstInstance
		);
	}
}
