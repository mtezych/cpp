
#include <gameboy/GameBoy.h>

int main()
{
	const auto gameBoy = gb::GameBoy
	{
		gb::Cartridge { "/home/mzych/Downloads/PokemonGameBoy/PokemonRed.gb" }
	};

	return 0;
}
