
#include "Window.h"

#include <chrono>
#include <thread>

int main()
{
	const auto window = windows::Window { 512, 512 };

	const auto render = []()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds { 160 });
	};
	window.RecieveMessages(render);

	return 0;
}
