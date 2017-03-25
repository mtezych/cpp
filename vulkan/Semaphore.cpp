
#include "Semaphore.h"

#include "Symbols.h"
#include "Device.h"

namespace vk
{
	Semaphore::Semaphore ()
	:
		semaphore { VK_NULL_HANDLE },
		device    { VK_NULL_HANDLE },
		vkCreateSemaphore  { nullptr },
		vkDestroySemaphore { nullptr }
	{
	}

	Semaphore::Semaphore (const Device& device)
	:
		semaphore { VK_NULL_HANDLE },
		device    { device.device  },
		vkCreateSemaphore  { device.LoadDeviceProcedure<symbol::vkCreateSemaphore >() },
		vkDestroySemaphore { device.LoadDeviceProcedure<symbol::vkDestroySemaphore>() }
	{
		const auto semaphoreCreateInfo = VkSemaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0,
		};
		vkCreateSemaphore(device.device, &semaphoreCreateInfo, nullptr, &semaphore);
	}

	Semaphore::~Semaphore ()
	{
		if (semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, semaphore, nullptr);
		}
	}

	Semaphore::Semaphore (Semaphore&& semaphore)
	:
		semaphore { semaphore.semaphore },
		device    { semaphore.device    },
		vkCreateSemaphore  { semaphore.vkCreateSemaphore  },
		vkDestroySemaphore { semaphore.vkDestroySemaphore }
	{
		semaphore.semaphore = VK_NULL_HANDLE;
		semaphore.device    = VK_NULL_HANDLE;
		semaphore.vkCreateSemaphore  = nullptr;
		semaphore.vkDestroySemaphore = nullptr;
	}

	Semaphore& Semaphore::operator = (Semaphore&& semaphore)
	{
		if (this->semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, this->semaphore, nullptr);
		}

		this->semaphore = semaphore.semaphore;
		this->device    = semaphore.device;
		this->vkCreateSemaphore  = semaphore.vkCreateSemaphore;
		this->vkDestroySemaphore = semaphore.vkDestroySemaphore;

		semaphore.semaphore = VK_NULL_HANDLE;
		semaphore.device    = VK_NULL_HANDLE;
		semaphore.vkCreateSemaphore  = nullptr;
		semaphore.vkDestroySemaphore = nullptr;

		return *this;
	}
}
