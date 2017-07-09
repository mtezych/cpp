
#ifndef GAME_BOY_MEMORY
#define GAME_BOY_MEMORY

#include <cstdint>
#include <array>

namespace gb
{
	union Memory
	{
		std::array<uint8_t, 0xFFFF> memoryMap;

		struct
		{
			std::array<uint8_t, 0x2000> videoRam;
			std::array<uint8_t, 0x2000>  workRam;
		};
	};
}

#endif
