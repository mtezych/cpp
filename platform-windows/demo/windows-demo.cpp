
#ifdef _WIN32

#include <windows/Window.h>

#include <chrono>
#include <thread>

int main()
{
	const auto window = windows::Window { util::uvec2 { 512, 512 } };

	const auto render = []()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 160 });
	};
	window.ReceiveMessages(render);

	return 0;
}

#else

int main()
{
	return 0;
}

#endif
