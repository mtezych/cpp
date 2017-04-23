
#include <vulkan/Loader.h>
#include <vulkan/Instance.h>
#include <vulkan/Surface.h>

int main()
{
	const auto loader = vk::Loader { };

	const auto applicationInfo = VkApplicationInfo
	{
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		"Vulkan Demo", VK_MAKE_VERSION(1, 0, 0),
		"Demo Engine", VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_0,
	};
	const auto instanceExtensions = std::vector<const char*>
	{
		VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME,
	};
	const auto instanceCreateInfo = VkInstanceCreateInfo
	{
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&applicationInfo,
		0, nullptr, // instance layers
		uint32_t ( instanceExtensions.size() ), instanceExtensions.data(),
	};
	const auto instance = vk::Instance { loader, instanceCreateInfo };

	const auto display = platform::Display { };
	const auto window  = platform::Window { display, util::uvec2 { 512, 512 } };
	const auto surface = vk::Surface { instance, display, window };

	const auto physicalDevices = instance.EnumeratePhysicalDevices();
	assert(physicalDevices.size() > 0);
	const auto& physicalDevice = physicalDevices[0];

	const auto render = []()
	{
	};
	window.ReceiveMessages(render);

	return 0;
}
