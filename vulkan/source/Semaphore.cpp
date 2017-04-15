
#include <vulkan/Semaphore.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	Semaphore::Semaphore (const Device& device)
	:
		device      { &device },
		vkSemaphore { VK_NULL_HANDLE }
	{
		const auto semaphoreCreateInfo = VkSemaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0,
		};
		device.vkCreateSemaphore
		(
			device.vkDevice,
			&semaphoreCreateInfo,
			nullptr,
			&vkSemaphore
		);
	}

	Semaphore::~Semaphore ()
	{
		if (vkSemaphore != VK_NULL_HANDLE)
		{
			device->vkDestroySemaphore(device->vkDevice, vkSemaphore, nullptr);
		}
	}

	Semaphore::Semaphore (Semaphore&& semaphore)
	:
		device      { semaphore.device      },
		vkSemaphore { semaphore.vkSemaphore }
	{
		semaphore.device      = nullptr;
		semaphore.vkSemaphore = VK_NULL_HANDLE;
	}

	Semaphore& Semaphore::operator = (Semaphore&& semaphore)
	{
		if (vkSemaphore != VK_NULL_HANDLE)
		{
			device->vkDestroySemaphore(device->vkDevice, vkSemaphore, nullptr);
		}

		device      = semaphore.device;
		vkSemaphore = semaphore.vkSemaphore;

		semaphore.device      = nullptr;
		semaphore.vkSemaphore = VK_NULL_HANDLE;

		return *this;
	}
}
