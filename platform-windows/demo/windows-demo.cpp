
#ifdef _WIN32

#include <windows/Window.h>
#include <windows/Bitmap.h>

#include <chrono>
#include <thread>
#include <array>

void WindowTask ()
{
	const auto window = windows::Window { util::uvec2 { 512, 512 } };

	const auto render = []()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.ReceiveMessages(render);
}

void BitmapTask ()
{
	const auto bitmap = windows::Bitmap { util::uvec2 { 256, 256 } };

	auto window = windows::Window { util::uvec2 { 512, 512 } };

	const auto render = [&]()
	{
		window.Draw(bitmap);

		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.ReceiveMessages(render);
}

int main()
{
	auto threads = std::array<std::thread, 3>
	{
		std::thread { WindowTask },
		std::thread { WindowTask },
		std::thread { BitmapTask },
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
