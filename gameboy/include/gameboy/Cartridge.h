
#ifndef GAME_BOY_CARTRIDGE
#define GAME_BOY_CARTRIDGE

#include <cstdint>
#include <string>
#include <vector>

namespace gb
{
	struct Cartridge
	{
		std::vector<uint8_t> rom;

		Cartridge(const std::string& romPath);
	};
}

#endif
