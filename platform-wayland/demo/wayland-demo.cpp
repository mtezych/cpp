
#ifdef __gnu_linux__

#include <wayland/Display.h>

int main()
{
	const auto display = wayland::Display { };

	return 0;
}

#else

int main()
{
	return 0;
}

#endif
