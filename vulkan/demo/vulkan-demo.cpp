
#include <vulkan/Loader.h>
#include <vulkan/Instance.h>
#include <vulkan/Device.h>
#include <vulkan/Surface.h>
#include <vulkan/Swapchain.h>
#include <vulkan/RenderPass.h>
#include <vulkan/ShaderModule.h>
#include <vulkan/PipelineCache.h>
#include <vulkan/PipelineLayout.h>
#include <vulkan/Pipeline.h>
#include <vulkan/ImageView.h>
#include <vulkan/Framebuffer.h>
#include <vulkan/CommandPool.h>
#include <vulkan/CommandBuffer.h>

#include <thread>
#include <utility>

auto MakeRenderPass (const vk::Device& device)
{
	const auto attachmentDescription = VkAttachmentDescription
	{
		// flags (no aliasing between attachments)
		0,

		// format of Vk::Image
		VK_FORMAT_B8G8R8A8_UNORM,

		// sample count per fragment for MSAA
		VK_SAMPLE_COUNT_1_BIT,

		// load operation at the beginning of vk::RenderPass for color & depth
		VK_ATTACHMENT_LOAD_OP_CLEAR,

		// store operation at the end of vk::RenderPass for color & depth
		VK_ATTACHMENT_STORE_OP_STORE,

		// load operation at the beginning of vk::RenderPass for stencil
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,

		// store operation at the end of vk::RenderPass for stencil
		VK_ATTACHMENT_STORE_OP_DONT_CARE,

		// layout before vk::RenderPass
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,

		// layout after vk::RenderPass
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};

	const auto attachmentReference = VkAttachmentReference
	{
		0,                                        // index
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // layout
	};

	const auto subpassDescription = VkSubpassDescription
	{
		// flags
		0,

		// graphics or compute bind point (only graphics bind points are supported)
		VK_PIPELINE_BIND_POINT_GRAPHICS,

		// input attachments (read only vk::Images)
		0, nullptr,

		// output attachments (write only vk::Images)
		//
		// ~ color attachments & resolves
		1, &attachmentReference, nullptr,
		//
		// ~ depth & stencil attachment
		nullptr,

		// preserved attachments (unused in current subpass, but used in later subpasses)
		0, nullptr,
	};

	return vk::RenderPass
	{
		device,
		std::vector<VkAttachmentDescription> { attachmentDescription },
		std::vector<VkSubpassDescription   > {    subpassDescription },
		std::vector<VkSubpassDependency    > {                       },
	};
}

auto MakeGraphicsPipeline (const vk::Device& device, const vk::RenderPass& renderPass)
{
	// #version 400
	//
	// void main()
	// {
	//     vec4 position[3] = vec4[3]
	//     (
	//         vec4(-0.7,  0.7, 0.0, 1.0),
	//         vec4( 0.7,  0.7, 0.0, 1.0),
	//         vec4( 0.0, -0.7, 0.0, 1.0)
	//     );
	//     gl_Position = position[gl_VertexIndex];
	// }
	const auto vertexShaderSpirvCode = std::vector<uint32_t>
	{
		0x07230203, 0x00010000, 0x00080001, 0x00000028, 0x00000000, 0x00020011,
		0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
		0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0007000f, 0x00000000,
		0x00000004, 0x6e69616d, 0x00000000, 0x00000019, 0x0000001d, 0x00030003,
		0x00000002, 0x00000190, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
		0x00030005, 0x0000000c, 0x00736f70, 0x00060005, 0x00000017, 0x505f6c67,
		0x65567265, 0x78657472, 0x00000000, 0x00060006, 0x00000017, 0x00000000,
		0x505f6c67, 0x7469736f, 0x006e6f69, 0x00070006, 0x00000017, 0x00000001,
		0x505f6c67, 0x746e696f, 0x657a6953, 0x00000000, 0x00070006, 0x00000017,
		0x00000002, 0x435f6c67, 0x4470696c, 0x61747369, 0x0065636e, 0x00030005,
		0x00000019, 0x00000000, 0x00060005, 0x0000001d, 0x565f6c67, 0x65747265,
		0x646e4978, 0x00007865, 0x00050048, 0x00000017, 0x00000000, 0x0000000b,
		0x00000000, 0x00050048, 0x00000017, 0x00000001, 0x0000000b, 0x00000001,
		0x00050048, 0x00000017, 0x00000002, 0x0000000b, 0x00000003, 0x00030047,
		0x00000017, 0x00000002, 0x00040047, 0x0000001d, 0x0000000b, 0x0000002a,
		0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016,
		0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000002,
		0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008,
		0x00000009, 0x00000003, 0x0004001c, 0x0000000a, 0x00000007, 0x00000009,
		0x00040020, 0x0000000b, 0x00000007, 0x0000000a, 0x0004002b, 0x00000006,
		0x0000000d, 0xbf333333, 0x0004002b, 0x00000006, 0x0000000e, 0x3f333333,
		0x0005002c, 0x00000007, 0x0000000f, 0x0000000d, 0x0000000e, 0x0005002c,
		0x00000007, 0x00000010, 0x0000000e, 0x0000000e, 0x0004002b, 0x00000006,
		0x00000011, 0x00000000, 0x0005002c, 0x00000007, 0x00000012, 0x00000011,
		0x0000000d, 0x0006002c, 0x0000000a, 0x00000013, 0x0000000f, 0x00000010,
		0x00000012, 0x00040017, 0x00000014, 0x00000006, 0x00000004, 0x0004002b,
		0x00000008, 0x00000015, 0x00000001, 0x0004001c, 0x00000016, 0x00000006,
		0x00000015, 0x0005001e, 0x00000017, 0x00000014, 0x00000006, 0x00000016,
		0x00040020, 0x00000018, 0x00000003, 0x00000017, 0x0004003b, 0x00000018,
		0x00000019, 0x00000003, 0x00040015, 0x0000001a, 0x00000020, 0x00000001,
		0x0004002b, 0x0000001a, 0x0000001b, 0x00000000, 0x00040020, 0x0000001c,
		0x00000001, 0x0000001a, 0x0004003b, 0x0000001c, 0x0000001d, 0x00000001,
		0x00040020, 0x0000001f, 0x00000007, 0x00000007, 0x0004002b, 0x00000006,
		0x00000022, 0x3f800000, 0x00040020, 0x00000026, 0x00000003, 0x00000014,
		0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8,
		0x00000005, 0x0004003b, 0x0000000b, 0x0000000c, 0x00000007, 0x0003003e,
		0x0000000c, 0x00000013, 0x0004003d, 0x0000001a, 0x0000001e, 0x0000001d,
		0x00050041, 0x0000001f, 0x00000020, 0x0000000c, 0x0000001e, 0x0004003d,
		0x00000007, 0x00000021, 0x00000020, 0x00050051, 0x00000006, 0x00000023,
		0x00000021, 0x00000000, 0x00050051, 0x00000006, 0x00000024, 0x00000021,
		0x00000001, 0x00070050, 0x00000014, 0x00000025, 0x00000023, 0x00000024,
		0x00000011, 0x00000022, 0x00050041, 0x00000026, 0x00000027, 0x00000019,
		0x0000001b, 0x0003003e, 0x00000027, 0x00000025, 0x000100fd, 0x00010038,
	};
	const auto vertexShaderModule = vk::ShaderModule
	{
		device, vertexShaderSpirvCode
	};

	// #version 400
	//
	// layout(location = 0) out vec4 FragColor;
	//
	// void main()
	// {
	//     FragColor = vec4(0.0, 0.4, 1.0, 1.0);
	// }
	const auto fragmentShaderSpirvCode = std::vector<uint32_t>
	{
		0x07230203, 0x00010000, 0x00080001,
		0x0000000e, 0x00000000, 0x00020011,
		0x00000001, 0x0006000b, 0x00000001,
		0x4c534c47, 0x6474732e, 0x3035342e,
		0x00000000, 0x0003000e, 0x00000000,
		0x00000001, 0x0006000f, 0x00000004,
		0x00000004, 0x6e69616d, 0x00000000,
		0x00000009, 0x00030010, 0x00000004,
		0x00000007, 0x00030003, 0x00000002,
		0x00000190, 0x00040005, 0x00000004,
		0x6e69616d, 0x00000000, 0x00060005,
		0x00000009, 0x67617246, 0x746e656d,
		0x6f6c6f43, 0x00000072, 0x00040047,
		0x00000009, 0x0000001e, 0x00000000,
		0x00020013, 0x00000002, 0x00030021,
		0x00000003, 0x00000002, 0x00030016,
		0x00000006, 0x00000020, 0x00040017,
		0x00000007, 0x00000006, 0x00000004,
		0x00040020, 0x00000008, 0x00000003,
		0x00000007, 0x0004003b, 0x00000008,
		0x00000009, 0x00000003, 0x0004002b,
		0x00000006, 0x0000000a, 0x00000000,
		0x0004002b, 0x00000006, 0x0000000b,
		0x3ecccccd, 0x0004002b, 0x00000006,
		0x0000000c, 0x3f800000, 0x0007002c,
		0x00000007, 0x0000000d, 0x0000000a,
		0x0000000b, 0x0000000c, 0x0000000c,
		0x00050036, 0x00000002, 0x00000004,
		0x00000000, 0x00000003, 0x000200f8,
		0x00000005, 0x0003003e, 0x00000009,
		0x0000000d, 0x000100fd, 0x00010038,
	};
	const auto fragmentShaderModule = vk::ShaderModule
	{
		device, fragmentShaderSpirvCode
	};

	const auto shaderStages = std::vector<VkPipelineShaderStageCreateInfo>
	{
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_VERTEX_BIT,
			vertexShaderModule.vkShaderModule,
			"main",
			nullptr,
		},
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fragmentShaderModule.vkShaderModule,
			"main",
			nullptr,
		},
	};

	const auto vertexInputState = VkPipelineVertexInputStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		nullptr,
		0,
		0, nullptr, //    binding descriptions
		0, nullptr, // attributes descriptions
	};

	const auto inputAssembly = VkPipelineInputAssemblyStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		nullptr,
		0,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		VK_FALSE // primitive restart
	};

	const auto viewport = VkViewport
	{
		  0.0f,   0.0f, // (x,y)
		512.0f, 512.0f, // (width, height)
		  0.0f,   1.0f, // (minDepth, maxDepth)
	};
	const auto scissor = VkRect2D
	{
		VkOffset2D {   0,   0 },
		VkExtent2D { 512, 512 },
	};
	const auto viewportState = VkPipelineViewportStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		nullptr,
		0,
		1, &viewport,
		1, &scissor,
	};

	const auto rasterizationState = VkPipelineRasterizationStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		nullptr,
		0,
		VK_FALSE,                        // depth clamp
		VK_FALSE,                        // rasterizer discard
		VK_POLYGON_MODE_FILL,            // polygon mode
		VK_CULL_MODE_NONE,               // culling mode
		VK_FRONT_FACE_COUNTER_CLOCKWISE, // front face
		VK_FALSE,                        // depth bias
		0.0f,                            // depth bias const factor
		0.0f,                            // depth bias clamp
		0.0f,                            // depth bias slope factor
		1.0f,                            // line width
	};

	const auto multisapleState = VkPipelineMultisampleStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		nullptr,
		0,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FALSE, // sample shading
		1.0f, // min sample shading
		nullptr,  // sample mask
		VK_FALSE, // alpha to coverage
		VK_FALSE, // alpha to one
	};

	const auto colorBlendAttachmentState = VkPipelineColorBlendAttachmentState
	{
		VK_FALSE,             // blending enable / disable
		VK_BLEND_FACTOR_ONE , // color source      blend factor
		VK_BLEND_FACTOR_ZERO, // color destination blend factor
		VK_BLEND_OP_ADD,      // color             blend operation
		VK_BLEND_FACTOR_ONE,  // alpha source      blend factor
		VK_BLEND_FACTOR_ZERO, // alpha destination blend factor
		VK_BLEND_OP_ADD,      // alpha             blend operation
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT , // write mask
	};
	const auto colorBlendState = VkPipelineColorBlendStateCreateInfo
	{
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		nullptr,
		0,
		VK_FALSE,                      // logic operation enable / disable
		VK_LOGIC_OP_COPY,              // logic operation
		1, &colorBlendAttachmentState, // color blend state for each color attachments
		{ 0.0f, 0.0f, 0.0f, 0.0f }     // blend constants
	};

	const auto pipelineLayout = vk::PipelineLayout
	{
		device,
		std::vector<VkDescriptorSetLayout> { },
		std::vector<VkPushConstantRange>   { },
	};

	return vk::Pipeline
	{
		device,
		vk::PipelineCache { device, std::vector<const void*> { } },
		VkGraphicsPipelineCreateInfo
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			nullptr,
			0,
			uint32_t(shaderStages.size()), shaderStages.data(),
			&vertexInputState,
			&inputAssembly,
			nullptr, // tessellation state
			&viewportState,
			&rasterizationState,
			&multisapleState,
			nullptr, // depth stencil state
			&colorBlendState,
			nullptr, // dynamic state
			pipelineLayout.vkPipelineLayout,
			renderPass.vkRenderPass,
			0, // subpass index
			VK_NULL_HANDLE, -1, // base pipeline
		}
	};
}

auto MakeImageView (const vk::Image& image)
{
	return vk::ImageView
	{
		image, vk::ImageView::CreateInfo
		{
			VK_IMAGE_VIEW_TYPE_2D,    // type
			VK_FORMAT_B8G8R8A8_UNORM, // format
			VkComponentMapping
			{
				VK_COMPONENT_SWIZZLE_IDENTITY, // R
				VK_COMPONENT_SWIZZLE_IDENTITY, // G
				VK_COMPONENT_SWIZZLE_IDENTITY, // B
				VK_COMPONENT_SWIZZLE_IDENTITY, // A
			},
			VkImageSubresourceRange
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0, 1, // mipmaps
				0, 1, // layers
			},
		}
	};
}

auto MakeFramebuffer (const vk::RenderPass& renderPass, vk::ImageView& attachment)
{
	auto attachments = std::vector<vk::ImageView> { };

	attachments.push_back(std::move(attachment));

	auto framebuffer = vk::Framebuffer
	{
		renderPass,
		attachments,
		VkExtent2D { 512, 512 }, // size
		1, // layers
	};

	attachment = std::move(attachments.front());

	return framebuffer;
}

auto RecordDrawCommandBuffer
(
	const vk::CommandPool& commandPool,
	const vk::RenderPass&  renderPass , const vk::Pipeline& graphicsPipeline,
	const vk::Framebuffer& framebuffer, const vk::Image&    swapchainImage
)
{
	auto commandBuffer = vk::CommandBuffer
	{
		commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY
	};

	const auto imageSubresourceRange = VkImageSubresourceRange
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0, 1, // mipmaps
		0, 1, // layers
	};

	commandBuffer.BeginRecording();
	{
		commandBuffer.RecordCommandPipelineBarrier
		(
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VkImageMemoryBarrier
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				0, //      source family queue
				0, // destination family queue
				swapchainImage.vkImage,
				imageSubresourceRange,
			}
		);
		{
			commandBuffer.RecordCommandBeginRenderPass
			(
				renderPass, framebuffer,
				VkRect2D
				{
					VkOffset2D {   0,   0 }, // offset
					VkExtent2D { 512, 512 }, // size
				},
				VkClearValue
				{
					VkClearColorValue { { 1.0f, 0.8f, 0.4f, 0.0f } } // color
				}
			);
			{
				commandBuffer.RecordCommandBindPipeline(graphicsPipeline);
				commandBuffer.RecordCommandDraw
				(
					3 /* vertex count */, 1 /* instance count */,
					0 /* first vertex */, 0 /* first instance */
				);
			}
			commandBuffer.RecordCommandEndRenderPass();
		}
		commandBuffer.RecordCommandPipelineBarrier
		(
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VkImageMemoryBarrier
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				0, //      source family queue
				0, // destination family queue
				swapchainImage.vkImage,
				imageSubresourceRange,
			}
		);
	}
	commandBuffer.EndRecording();

	return commandBuffer;
}

auto RecordClearCommandBuffer
(
	const vk::CommandPool& commandPool,
	const vk::Image&       swapchainImage
)
{
	auto commandBuffer = vk::CommandBuffer
	{
		commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY
	};

	const auto imageSubresourceRange = VkImageSubresourceRange
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0, 1, // mipmaps
		0, 1, // layers
	};

	commandBuffer.BeginRecording();
	{
		commandBuffer.RecordCommandPipelineBarrier
		(
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageMemoryBarrier
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				0, //      source family queue
				0, // destination family queue
				swapchainImage.vkImage,
				imageSubresourceRange,
			}
		);
		{
			commandBuffer.RecordCommandClearImage
			(
				swapchainImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				imageSubresourceRange,
				VkClearColorValue { { 1.0f, 0.8f, 0.4f, 0.0f } }
			);
		}
		commandBuffer.RecordCommandPipelineBarrier
		(
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageMemoryBarrier
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				0, //      source family queue
				0, // destination family queue
				swapchainImage.vkImage,
				imageSubresourceRange,
			}
		);
	}
	commandBuffer.EndRecording();

	return commandBuffer;
}

int main ()
{
	const auto loader = vk::Loader { };

	const auto instance = vk::Instance
	{
		loader, vk::Instance::CreateInfo
		{
			"Vulkan Demo", vk::Version { 1, 0, 0 }, //      app name & version
			"Demo Engine", vk::Version { 1, 0, 0 }, //   engine name & version
			vk::Version { 1, 0, 0 },                // required Vulkan version
			{ },                                    // enabled layers
			{                                       // enabled extensions
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME,
			},
		}
	};

	const auto physicalDevices = instance.EnumeratePhysicalDevices();
	assert(physicalDevices.size() > 0);
	const auto& physicalDevice = physicalDevices[0];

	const auto device = vk::Device
	{
		physicalDevice, vk::Device::CreateInfo
		{
			std::vector<vk::Queue::CreateInfo>
			{
				{ 0 /* family index */, { 1.0f } /* priorities */ }
			},
			{ },                                 // enabled layers
			{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }, // enabled extensions
			VkPhysicalDeviceFeatures { },        // enabled features
		}
	};
	const auto queue = device.GetQueue(0 /* family index */, 0 /* queue index */);

	const auto display = platform::Display { };
	const auto window  = platform::Window { display, util::uvec2 { 512, 512 } };
	const auto surface = vk::Surface { instance, display, window };

	const auto swapchain = vk::Swapchain
	{
		device, surface, vk::Swapchain::CreateInfo
		{
			3,                                     // min surface count
			VK_FORMAT_B8G8R8A8_UNORM,              // images format
			VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,     // images color space
			1,                                     // images array layers
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  // render target usage
			VK_IMAGE_USAGE_TRANSFER_DST_BIT,       // clear  target usage
			VK_SHARING_MODE_EXCLUSIVE, { },        // images sharing mode
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, // transform before present
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,     // alpha composition mode
			VK_PRESENT_MODE_FIFO_KHR,              // present mode
			VK_TRUE,                               // clipped aka owns pixels
			vk::Swapchain { },                     // old swapchain
		}
	};
	const auto swapchainImages = swapchain.GetImages();

	const auto commandPool = vk::CommandPool { device, 0 /* family index */ };

	auto commandBuffers = std::vector<vk::CommandBuffer> { };

/*
	for (const auto& swapchainImage : swapchainImages)
	{
		auto commandBuffer = RecordClearCommandBuffer
		(
			commandPool, swapchainImage
		);
		commandBuffers.push_back(std::move(commandBuffer));
	}
	const auto waitPipelineStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
*/

	const auto renderPass       = MakeRenderPass(device);
	const auto graphicsPipeline = MakeGraphicsPipeline(device, renderPass);

	auto  attachments = std::vector<vk::ImageView  > { };
	auto framebuffers = std::vector<vk::Framebuffer> { };

	for (const auto& swapchainImage : swapchainImages)
	{
		auto attachment    = MakeImageView(swapchainImage);
		auto framebuffer   = MakeFramebuffer(renderPass, attachment);
		auto commandBuffer = RecordDrawCommandBuffer
		(
			commandPool, renderPass, graphicsPipeline, framebuffer, swapchainImage
		);

		   attachments.push_back(std::move(attachment   ));
		  framebuffers.push_back(std::move(framebuffer  ));
		commandBuffers.push_back(std::move(commandBuffer));
	}
	const auto waitPipelineStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;


	auto imageAvailable = vk::Semaphore { device };
	auto imageReady     = vk::Semaphore { device };

	const auto render = [&] ()
	{
		auto imageIndex = swapchain.Acquire(imageAvailable);

		queue.Submit
		(
			commandBuffers[imageIndex],
			imageAvailable, waitPipelineStage,
			imageReady
		);

		queue.Present(swapchain, imageIndex, imageReady);

		queue.WaitIdle();
	};
	window.ReceiveMessages(render);

	device.WaitIdle();

	return 0;
}
