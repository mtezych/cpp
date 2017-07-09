
#include <gameboy/GameBoy.h>

#include <utility>

namespace gb
{
	GameBoy::GameBoy (Cartridge cartridge)
	:
		processor { },
		memory    { },
		cartridge { std::move(cartridge) }
	{
		processor.Run(this->cartridge.rom);
	}
}
