
#ifdef _WIN32

#include <windows/Window.h>

#include <chrono>
#include <thread>
#include <array>

void window_task ()
{
	const auto window = windows::Window { util::uvec2 { 512, 512 } };

	const auto render = []()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.ReceiveMessages(render);
}

int main()
{
	auto threads = std::array<std::thread, 2>
	{
		std::thread { window_task },
		std::thread { window_task },
	};

	for (auto& thread : threads)
	{
		thread.join();
	}

	return 0;
}

#else

int main()
{
	return 0;
}

#endif
