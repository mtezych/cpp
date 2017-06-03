
#ifndef VULKAN_FENCE
#define VULKAN_FENCE

#include <vulkan/vulkan.h>

#include <limits>
#include <vector>

namespace vk
{
	struct Device;

	struct Fence
	{
		const Device* device;
		VkFence       vkFence;

		explicit
		Fence (const Device& device);
		Fence (const Device& device, const VkFenceCreateFlags flags);

		~Fence();

		Fence (Fence&& fence);
		Fence (const Fence& fence) = delete;

		Fence& operator = (Fence&& fence);
		Fence& operator = (const Fence& fence) = delete;

		//
		// @note: returns true if fence is signaled; false otherwise
		//
		bool GetStatus () const;

		void Reset ();

		//
		// @note: returns false if time out occurred; false otherwise
		//
		bool Wait (const uint64_t timeOut = std::numeric_limits<uint64_t>::max());

		static bool WaitAny
		(
			const std::vector<Fence>& fences,
			const uint64_t timeOut = std::numeric_limits<uint64_t>::max()
		);
	};
}

#endif
