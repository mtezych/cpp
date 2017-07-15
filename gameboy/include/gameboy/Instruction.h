
#ifndef GAME_BOY_INSTRUCTION
#define GAME_BOY_INSTRUCTION

#include <gameboy/Registers.h>

#include <variant>

namespace gb
{
	enum Mnemonic
	{
		NOP,
		LD,
	};

	struct Instruction
	{
		Mnemonic mnemonic;

	protected:
		Instruction (const Mnemonic mnemonic)
		:
			mnemonic { mnemonic }
		{
		}

		~Instruction() = default;
	};

	//
	//        x0
	//    +--------+
	// 0x |  NOP   |
	//    +--------+
	//
	struct Nop : Instruction
	{
		Nop ()
		:
			Instruction { Mnemonic::NOP }
		{
		}
	};

	//
	//        x0       x1       x2       x3       x4       x5             x7
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	// 4x | LD B,B | LD B,C | LD B,D | LD B,E | LD B,H | LD B,L |     | LD B,A |
	// 5x | LD D,B | LD D,C | LD D,D | LD D,E | LD D,H | LD D,L | ... | LD D,A |
	// 6x | LD H,B | LD H,C | LD H,D | LD H,E | LD H,H | LD H,L |     | LD H,A |
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	//
	//        x8       x9       xA       xB       xC       xD             xF
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	// 4x | LD C,B | LD C,C | LD C,D | LD C,E | LD C,H | LD C,L |     | LD C,A |
	// 5x | LD E,B | LD E,C | LD E,D | LD E,E | LD E,H | LD E,L | ... | LD E,A |
	// 6x | LD L,B | LD L,C | LD L,D | LD L,E | LD L,H | LD L,L |     | LD L,A |
	// 7x | LD A,B | LD A,C | LD A,D | LD A,E | LD A,H | LD A,L |     | LD A,A |
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	//
	struct LoadReg8Reg8 : Instruction
	{
		Reg8&       dstReg;
		const Reg8& srcReg;

		LoadReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Instruction { Mnemonic::LD },
			dstReg      { dstReg       },
			srcReg      { srcReg       }
		{
		}
	};

	//
	//         x6              xE
	//    +---------+     +---------+
	// 0x | LD B,d8 |     | LD C,d8 |
	// 1x | LD D,d8 | ... | LD E,d8 |
	// 2x | LD H,d8 |     | LD L,d8 |
	// 3x |         |     | LD A,d8 |
	//    +---------+     +---------+
	//
	struct LoadReg8Immediate : Instruction
	{
		Reg8&         reg;
		const uint8_t immediate;

		LoadReg8Immediate (Reg8& reg, const uint8_t immediate)
		:
			Instruction { Mnemonic::LD },
			reg         { reg          },
			immediate   { immediate    }
		{
		}
	};

	struct LoadMemoryReg8 : Instruction
	{
		const Reg8& reg;

		LoadMemoryReg8 (const Reg8& reg)
		:
			Instruction { Mnemonic::LD },
			reg         { reg          }
		{
		}
	};

	using AnyInstruction = std::variant<Nop, LoadReg8Reg8, LoadMemoryReg8>;
}

#endif
