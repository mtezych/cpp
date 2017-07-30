
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
		ADD, ADC,
		SUB, SBC,
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

	struct Reg8Reg8Instruction : Instruction
	{
		      Reg8& dstReg;
		const Reg8& srcReg;

		Reg8Reg8Instruction (const Mnemonic mnemonic, Reg8& dstReg, const Reg8& srcReg)
		:
			Instruction { mnemonic },
			dstReg      { dstReg   },
			srcReg      { srcReg   }
		{
		}
	};

	//
	//        x0
	//    +--------+
	// 0x |  NOP   |
	//    +--------+
	//
	struct NoOperation : Instruction
	{
		NoOperation ()
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
	struct LoadReg8Reg8 : Reg8Reg8Instruction
	{
		LoadReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::LD, dstReg, srcReg }
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

	//
	//        x0        x1        x2        x3        x4        x5              x7
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// 8x | ADD A,B | ADD A,C | ADD A,D | ADD A,E | ADD A,H | ADD A,L |     | ADD A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct AddReg8Reg8 : Reg8Reg8Instruction
	{
		AddReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::ADD, dstReg, srcReg }
		{
		}
	};

	//
	//        x8        x9        xA        xB        xC        xD              xF
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// 8x | ADC A,B | ADC A,C | ADC A,D | ADC A,E | ADC A,H | ADC A,L |     | ADC A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct AddWithCarryReg8Reg8 : Reg8Reg8Instruction
	{
		AddWithCarryReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::ADC, dstReg, srcReg }
		{
		}
	};

	//
	//        x0        x1        x2        x3        x4        x5              x7
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// 9x | SUB A,B | SUB A,C | SUB A,D | SUB A,E | SUB A,H | SUB A,L |     | SUB A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct SubtractReg8Reg8 : Reg8Reg8Instruction
	{
		SubtractReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::SUB, dstReg, srcReg }
		{
		}
	};

	//
	//        x8        x9        xA        xB        xC        xD              xF
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// 9x | SBC A,B | SBC A,C | SBC A,D | SBC A,E | SBC A,H | SBC A,L |     | SBC A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct SubtractWithCarryReg8Reg8 : Reg8Reg8Instruction
	{
		SubtractWithCarryReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::SBC, dstReg, srcReg }
		{
		}
	};

	using AnyInstruction = std::variant
	<
		NoOperation,
		LoadReg8Reg8,
		LoadReg8Immediate,
		LoadMemoryReg8,
		AddReg8Reg8,
		AddWithCarryReg8Reg8,
		SubtractReg8Reg8,
		SubtractWithCarryReg8Reg8
	>;
}

#endif
