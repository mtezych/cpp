
#include <vulkan/Loader.h>
#include <vulkan/Instance.h>
#include <vulkan/Device.h>
#include <vulkan/Surface.h>
#include <vulkan/Swapchain.h>

#include <thread>

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
				VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME
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

	const auto render = [] ()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.ReceiveMessages(render);

	return 0;
}
