
#include <vulkan/Fence.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

#include <algorithm>

namespace vk
{
	Fence::Fence (const Device& device)
	:
		Fence { device, VkFenceCreateFlags { 0 } }
	{
	}

	Fence::Fence (const Device& device, const VkFenceCreateFlags flags)
	:
		device  { &device        },
		vkFence { VK_NULL_HANDLE }
	{
		const auto fenceCreateInfo = VkFenceCreateInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			flags,
		};
		assert((flags == 0) || (flags == VK_FENCE_CREATE_SIGNALED_BIT));

		const auto result = device.vkCreateFence
		(
			device.vkDevice,
			&fenceCreateInfo,
			nullptr,
			&vkFence
		);
		assert(result == VK_SUCCESS);
	}

	Fence::~Fence ()
	{
		if (vkFence != VK_NULL_HANDLE)
		{
			device->vkDestroyFence(device->vkDevice, vkFence, nullptr);
		}
	}

	Fence::Fence (Fence&& fence)
	:
		device  { fence.device  },
		vkFence { fence.vkFence }
	{
		fence.device  = nullptr;
		fence.vkFence = VK_NULL_HANDLE;
	}

	Fence& Fence::operator = (Fence&& fence)
	{
		if (vkFence != VK_NULL_HANDLE)
		{
			device->vkDestroyFence(device->vkDevice, vkFence, nullptr);
		}

		device  = fence.device;
		vkFence = fence.vkFence;

		fence.device  = nullptr;
		fence.vkFence = VK_NULL_HANDLE;

		return *this;
	}

	bool Fence::GetStatus () const
	{
		const auto result = device->vkGetFenceStatus(device->vkDevice, vkFence);
		assert((result == VK_SUCCESS) || (result == VK_NOT_READY));

		return result == VK_SUCCESS;
	}

	void Fence::Reset ()
	{
		const auto result = device->vkResetFences(device->vkDevice, 1, &vkFence);
		assert(result == VK_SUCCESS);
	}

	bool Fence::Wait (const uint64_t timeOut)
	{
		const auto result = device->vkWaitForFences
		(
			device->vkDevice,
			1, &vkFence,
			VK_TRUE, // waitAll
			timeOut
		);
		assert((result == VK_SUCCESS) || (result == VK_TIMEOUT));

		return result == VK_SUCCESS;
	}

	bool
	Fence::WaitAny (const std::vector<Fence> &fences, const uint64_t timeOut)
	{
		if (fences.size() > 0)
		{
			const auto device = fences[0].device;

			auto vkFences = std::vector<VkFence>
			(
				fences.size(), VK_NULL_HANDLE
			);
			std::transform
			(
				fences.begin(), fences.end(), vkFences.begin(),

				[](const Fence& fence) -> VkFence
				{
					return fence.vkFence;
				}
			);

			const auto result = device->vkWaitForFences
			(
				device->vkDevice,
				static_cast<uint32_t>(vkFences.size()), vkFences.data(),
				VK_FALSE, // waitAny
				timeOut
			);
			assert((result == VK_SUCCESS) || (result == VK_TIMEOUT));

			return result == VK_SUCCESS;
		}
		else
		{
			return false;
		}
	}
}
