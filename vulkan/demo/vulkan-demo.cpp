
#include <vulkan/Loader.h>
#include <vulkan/Instance.h>
#include <vulkan/Surface.h>
#include <vulkan/Device.h>

#include <thread>

int main()
{
	const auto loader = vk::Loader { };

	const auto instance = vk::Instance
	{
		loader, vk::Instance::CreateInfo
		{
			"Vulkan Demo", vk::Version { 1, 0, 0 }, //      app name & version
			"Demo Engine", vk::Version { 1, 0, 0 }, //   engine name & version
			vk::Version { 1, 0 , 0 },               // required Vulkan version
			{ },                                    // enabled layers
			{                                       // enabled extensions
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME
			},
		}
	};

	const auto display = platform::Display { };
	const auto window  = platform::Window { display, util::uvec2 { 512, 512 } };
	const auto surface = vk::Surface { instance, display, window };

	const auto physicalDevices = instance.EnumeratePhysicalDevices();
	assert(physicalDevices.size() > 0);
	const auto& physicalDevice = physicalDevices[0];

	const auto device = vk::Device
	{
		physicalDevice, vk::Device::CreateInfo
		{
			{
				vk::Queue::CreateInfo
				{
					0 /*family index*/, { 1.0f } /* priorities */
				}
			},
			{ }, // layers
			{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }, // extensions
			VkPhysicalDeviceFeatures { },
		}
	};

	const auto queue = device.Queues(0 /*family index*/, 0 /*queue index*/);

	const auto render = []()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.ReceiveMessages(render);

	return 0;
}
