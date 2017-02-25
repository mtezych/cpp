
#include "Vulkan.h"

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

	const auto vulkan = vk::Vulkan { path };

	return 0;
}
