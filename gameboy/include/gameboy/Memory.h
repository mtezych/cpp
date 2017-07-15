
#ifndef GAME_BOY_MEMORY
#define GAME_BOY_MEMORY

#include <cstdint>
#include <array>

/*
                 +-------------------------------------------------------------+
 0x0000 - 0x3FFF |  16 KB |           |          ROM Bank #0        [Fixed]    |
                 |--------| Cartridge |----------------------------------------|
 0x4000 - 0x7FFF |  16 KB |           |          ROM Bank #1 - #N [Switchable] |
                 +-------------------------------------------------------------+
 0x8000 - 0x9FFF |   8 KB |  GameBoy  |    Video RAM Bank #0 - #1 [Switchable] |
                 +-------------------------------------------------------------+
 0xA000 - 0xBFFF |   8 KB | Cartridge | External RAM              [Switchable] |
                 +-------------------------------------------------------------+
 0xC000 - 0xCFFF |   4 KB |           |     Work RAM Bank #0        [Fixed]    |
                 +--------| GameBoy   |----------------------------------------+
 0xD000 - 0xDFFF |   4 KB |           |     Work RAM Bank #1 - #7 [Switchable] |
                 +-------------------------------------------------------------+
 0xE000 - 0xFDFF | 7.5 KB |           |     Echo RAM                           |
                 +-------------------------------------------------------------+
 0xFE00 - 0xFE9F | 160  B |           |          OAM (Sprite Attribute Table)  |
                 +-------------------------------------------------------------+
 0xFEA0 - 0xFEFF |  96  B |           |       Unused Region                    |
                 +-------------------------------------------------------------+
 0xFF00 - 0xFF7F | 128  B |           |    I/O Ports                           |
                 +-------------------------------------------------------------+
 0xFF80 - 0xFFFE | 127  B |  GameBoy  |     High RAM                           |
                 +-------------------------------------------------------------+
          0xFFFF |   1  B |           |    Interrupt Switch                    |
                 +-------------------------------------------------------------+
*/
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
