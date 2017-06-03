
#ifndef VULKAN_BUFFER_VIEW
#define VULKAN_BUFFER_VIEW

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;
	struct Buffer;

	struct BufferView
	{
		struct CreateInfo
		{
			VkBufferViewCreateInfo createInfo;

			CreateInfo
			(
				const VkFormat     format,
				const VkDeviceSize offset,
				const VkDeviceSize range
			);
		};

		const Device* device;
		VkBufferView  vkBufferView;

		BufferView
		(
			const Device&     device,
			const Buffer&     buffer,
			const CreateInfo& createInfo
		);

		~BufferView ();

		BufferView (BufferView&& bufferView);
		BufferView (const BufferView& bufferView) = delete;

		BufferView& operator = (BufferView&& bufferView);
		BufferView& operator = (const BufferView& bufferView) = delete;
	};
}

#endif
