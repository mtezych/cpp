
// GameBoy Color CPU Manual:
//        http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf

// GameBoy Programming Manual:
//        http://www.chrisantonellis.com/files/gameboy/gb-programming-manual.pdf

// GameBoy Opcode Summary:
//        http://gameboy.mongenel.com/dmg/opcodes.html
//        http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html

// GameBoy Opcode Map:
//        http://imrannazar.com/Gameboy-Z80-Opcode-Map

// GameBoy Memory Map:
//        http://gameboy.mongenel.com/dmg/asmmemmap.html

// Writing a Game Boy emulator, Cinoop:
//        https://cturt.github.io/cinoop.html

// GameBoy emulator by brNX
//        https://github.com/brNX/gameboy-emu


#ifndef GAME_BOY_GAME_BOY
#define GAME_BOY_GAME_BOY

#include <gameboy/Processor.h>
#include <gameboy/Memory.h>
#include <gameboy/Cartridge.h>

namespace gb
{
	struct GameBoy
	{
		Processor processor;
		Memory    memory;
		Cartridge cartridge;

		GameBoy (Cartridge cartridge);
	};
}

#endif
