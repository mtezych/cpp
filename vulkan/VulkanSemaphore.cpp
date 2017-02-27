
#include "VulkanSemaphore.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"

namespace vk
{
	VulkanSemaphore::VulkanSemaphore ()
	:
		semaphore { VK_NULL_HANDLE },
		device    { VK_NULL_HANDLE },
		vkCreateSemaphore  { nullptr },
		vkDestroySemaphore { nullptr }
	{
	}

	VulkanSemaphore::VulkanSemaphore (const VulkanDevice& device)
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

	VulkanSemaphore::~VulkanSemaphore ()
	{
		if (semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, semaphore, nullptr);
		}
	}

	VulkanSemaphore::VulkanSemaphore (VulkanSemaphore&& semaphore)
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

	VulkanSemaphore& VulkanSemaphore::operator = (VulkanSemaphore&& semaphore)
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
