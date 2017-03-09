
#include "VulkanCommandBuffer.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
// #include "VulkanRenderPass.h"
// #include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"

#include <cassert>

namespace vk
{
	VulkanCommandBuffer::VulkanCommandBuffer()
	:
		commandBuffer { VK_NULL_HANDLE },
		commandPool   { VK_NULL_HANDLE },
		device        { VK_NULL_HANDLE },

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

	VulkanCommandBuffer::VulkanCommandBuffer
	(
		const VulkanDevice& device,
		const VulkanCommandPool& commandPool,
		const VkCommandBufferLevel level
	):
		commandBuffer { VK_NULL_HANDLE          },
		commandPool   { commandPool.commandPool },
		device        { device.device           },

		vkAllocateCommandBuffers
		{
			device.LoadDeviceProcedure<symbol::vkAllocateCommandBuffers>()
		},
		vkFreeCommandBuffers
		{
			device.LoadDeviceProcedure<symbol::vkFreeCommandBuffers>()
		},
		vkBeginCommandBuffer
		{
			device.LoadDeviceProcedure<symbol::vkBeginCommandBuffer>()
		},
		vkEndCommandBuffer
		{
			device.LoadDeviceProcedure<symbol::vkEndCommandBuffer>()
		},
		vkCmdClearColorImage
		{
			device.LoadDeviceProcedure<symbol::vkCmdClearColorImage>()
		},
		vkCmdPipelineBarrier
		{
			device.LoadDeviceProcedure<symbol::vkCmdPipelineBarrier>()
		},
		vkCmdBeginRenderPass
		{
			device.LoadDeviceProcedure<symbol::vkCmdBeginRenderPass>()
		},
		vkCmdEndRenderPass
		{
			device.LoadDeviceProcedure<symbol::vkCmdEndRenderPass>()
		},
		vkCmdBindPipeline
		{
			device.LoadDeviceProcedure<symbol::vkCmdBindPipeline>()
		},
		vkCmdDraw
		{
			device.LoadDeviceProcedure<symbol::vkCmdDraw>()
		}
	{
		const auto allocateInfo = VkCommandBufferAllocateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			commandPool.commandPool,
			level,
			1
		};
		const auto result = vkAllocateCommandBuffers
		(
			device.device,
			&allocateInfo, &commandBuffer
		);
		assert(result == VK_SUCCESS);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		if (commandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		}
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& commandBuffer)
	:
		commandBuffer { commandBuffer.commandBuffer },
		commandPool   { commandBuffer.commandPool   },
		device        { commandBuffer.device        },

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
		commandBuffer.commandBuffer = VK_NULL_HANDLE;
		commandBuffer.commandPool   = VK_NULL_HANDLE;
		commandBuffer.device        = VK_NULL_HANDLE;

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

	VulkanCommandBuffer& VulkanCommandBuffer::operator =(VulkanCommandBuffer&& commandBuffer)
	{
		if (this->commandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(this->device, this->commandPool, 1, &this->commandBuffer);
		}

		this->commandBuffer = commandBuffer.commandBuffer;
		this->commandPool   = commandBuffer.commandPool;
		this->device        = commandBuffer.device;

		this->vkAllocateCommandBuffers = commandBuffer.vkAllocateCommandBuffers;
		this->vkFreeCommandBuffers     = commandBuffer.vkFreeCommandBuffers;
		this->vkBeginCommandBuffer     = commandBuffer.vkBeginCommandBuffer;
		this->vkEndCommandBuffer       = commandBuffer.vkEndCommandBuffer;
		this->vkCmdClearColorImage     = commandBuffer.vkCmdClearColorImage;
		this->vkCmdPipelineBarrier     = commandBuffer.vkCmdPipelineBarrier;
		this->vkCmdBeginRenderPass     = commandBuffer.vkCmdBeginRenderPass;
		this->vkCmdEndRenderPass       = commandBuffer.vkCmdEndRenderPass;
		this->vkCmdBindPipeline        = commandBuffer.vkCmdBindPipeline;
		this->vkCmdDraw                = commandBuffer.vkCmdDraw;

		commandBuffer.commandBuffer = VK_NULL_HANDLE;
		commandBuffer.commandPool   = VK_NULL_HANDLE;
		commandBuffer.device        = VK_NULL_HANDLE;

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

	void VulkanCommandBuffer::BeginRecording()
	{
		const auto beginInfo = VkCommandBufferBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			nullptr,
		};
		const auto result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		assert(result == VK_SUCCESS);
	}

	void VulkanCommandBuffer::EndRecording()
	{
		const auto result = vkEndCommandBuffer(commandBuffer);
		assert(result == VK_SUCCESS);
	}

	void VulkanCommandBuffer::RecordCommandPipelineBarrier
	(
		const VkPipelineStageFlags  sourcePipelineStageMask,
		const VkPipelineStageFlags  destinationPipelineStageMask,
		const VkImageMemoryBarrier& imageMemoryBarrier
	)
	{
		vkCmdPipelineBarrier
		(
			commandBuffer,
			sourcePipelineStageMask,
			destinationPipelineStageMask,
			VkDependencyFlags { 0 },
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier
		);
	}

	void VulkanCommandBuffer::RecordCommandClearImage
	(
		VkImage&                       image,
		const VkImageLayout            imageLayout,
		const VkImageSubresourceRange& imageSubresourceRange,
		const VkClearColorValue&       clearColor
	)
	{
		vkCmdClearColorImage
		(
			commandBuffer,
			image, imageLayout,
			&clearColor,
			1, &imageSubresourceRange
		);
	}

//	void VulkanCommandBuffer::RecordCommandBeginRenderPass
//	(
//		const VulkanRenderPass&  renderPass,
//		const VulkanFramebuffer& framebuffer,
//		const VkRect2D&          renderArea,
//		const VkClearValue&      clear
//	)
//	{
//		const auto beginInfo = VkRenderPassBeginInfo
//		{
//			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
//			nullptr,
//			renderPass.renderPass,
//			framebuffer.framebuffer,
//			renderArea,
//			1, &clear
//		};
//		vkCmdBeginRenderPass
//		(
//			commandBuffer,
//			&beginInfo,
//			VkSubpassContents { VK_SUBPASS_CONTENTS_INLINE }
//		);
//	}

	void VulkanCommandBuffer::RecordCommandEndRenderPass()
	{
		vkCmdEndRenderPass(commandBuffer);
	}

	void VulkanCommandBuffer::RecordCommandBindPipeline(const VulkanPipeline& pipeline)
	{
		vkCmdBindPipeline
		(
			commandBuffer,
			pipeline.bindPoint,
			pipeline.pipeline
		);
	}

	void VulkanCommandBuffer::RecordCommandDraw
	(
		uint32_t vertexCount, uint32_t instanceCount,
		uint32_t firstVertex, uint32_t firstInstance
	)
	{
		vkCmdDraw
		(
			commandBuffer,
			vertexCount, instanceCount,
			firstVertex, firstInstance
		);
	}
}