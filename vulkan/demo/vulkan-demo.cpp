
#include <vulkan/Loader.h>
#include <vulkan/Instance.h>

#include <array>

int main()
{
#if defined(__ANDROID__)
	const auto path = "libvulkan.so";
#elif defined(__gnu_linux__)
	const auto path = "libvulkan.so.1";
#elif defined(_WIN32)
	const auto path = "vulkan-1.dll";
#else
	#error "Unsupported Platform"
#endif

	const auto loader = vk::Loader { path };

	const auto applicationInfo = VkApplicationInfo
	{
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		"Vulkan Demo",
		VK_MAKE_VERSION(1, 0, 0),
		"Vulkan Engine",
		VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_0,
	};
	std::array<const char*, 1 /* 2 */> instanceExtensions =
	{{
		VK_KHR_SURFACE_EXTENSION_NAME,
//		VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
	}};
	const auto instanceCreateInfo = VkInstanceCreateInfo
	{
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&applicationInfo,
		0, nullptr,
		uint32_t { instanceExtensions.size() }, instanceExtensions.data(),
	};
	const auto instance = vk::Instance { loader, instanceCreateInfo };

	for (auto& physicalDevice : instance.EnumeratePhysicalDevices())
	{
	}

	return 0;
}