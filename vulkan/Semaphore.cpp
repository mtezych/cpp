
#include "Semaphore.h"

#include "Symbols.h"
#include "Device.h"

namespace vk
{
	Semaphore::Semaphore ()
	:
		device      { nullptr },
		vkSemaphore { VK_NULL_HANDLE },

		vkCreateSemaphore  { nullptr },
		vkDestroySemaphore { nullptr }
	{
	}

	Semaphore::Semaphore (const Device& device)
	:
		device      { &device },
		vkSemaphore { VK_NULL_HANDLE },

		vkCreateSemaphore  { device.LoadDeviceProcedure<symbol::vkCreateSemaphore >() },
		vkDestroySemaphore { device.LoadDeviceProcedure<symbol::vkDestroySemaphore>() }
	{
		const auto semaphoreCreateInfo = VkSemaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0,
		};
		vkCreateSemaphore
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
			vkDestroySemaphore(device->vkDevice, vkSemaphore, nullptr);
		}
	}

	Semaphore::Semaphore (Semaphore&& semaphore)
	:
		device      { semaphore.device      },
		vkSemaphore { semaphore.vkSemaphore },

		vkCreateSemaphore  { semaphore.vkCreateSemaphore  },
		vkDestroySemaphore { semaphore.vkDestroySemaphore }
	{
		semaphore.device      = nullptr;
		semaphore.vkSemaphore = VK_NULL_HANDLE;

		semaphore.vkCreateSemaphore  = nullptr;
		semaphore.vkDestroySemaphore = nullptr;
	}

	Semaphore& Semaphore::operator = (Semaphore&& semaphore)
	{
		if (vkSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device->vkDevice, vkSemaphore, nullptr);
		}

		device      = semaphore.device;
		vkSemaphore = semaphore.vkSemaphore;

		vkCreateSemaphore  = semaphore.vkCreateSemaphore;
		vkDestroySemaphore = semaphore.vkDestroySemaphore;

		semaphore.device      = nullptr;
		semaphore.vkSemaphore = VK_NULL_HANDLE;

		semaphore.vkCreateSemaphore  = nullptr;
		semaphore.vkDestroySemaphore = nullptr;

		return *this;
	}
}
