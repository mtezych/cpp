
#ifndef VULKAN_QUERY_POOL
#define VULKAN_QUERY_POOL

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	//
	//            +-----------+
	//            | undefined |
	//            +-----------+
	//                  |
	//            reset |   +------------------+
	//                  |   |      reset       |
	//                  V   V                  |
	//             +-------------+             |
	//       +-----| unavailable |-----+       |
	//       |     +-------------+     |       |
	// begin |       ^    |    ^       | end   |
	//       |       |    |    |       |       |
	//       +-------+    |    +-------+       |
	//                    V                    |
	//              +-----------+              |
	//              | available |--------------+
	//              +-----------+
	//
	struct QueryPool
	{
		struct CreateInfo
		{
			VkQueryPoolCreateInfo createInfo;

			CreateInfo
			(
				const VkQueryType queryType, const uint32_t queryCount
			);

			CreateInfo
			(
				const VkQueryType queryType, const uint32_t queryCount,
				const VkQueryPipelineStatisticFlags pipelineStatistics
			);
		};

		const Device*    device;
		VkQueryPool vkQueryPool;

		QueryPool (const Device& device, const CreateInfo& createInfo);

		~QueryPool ();

		QueryPool (QueryPool&& queryPool);
		QueryPool (const QueryPool& queryPool) = delete;

		QueryPool& operator = (QueryPool&& queryPool);
		QueryPool& operator = (const QueryPool& queryPool) = delete;
	};
}

#endif
