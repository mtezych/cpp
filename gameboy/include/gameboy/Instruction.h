
#ifndef GAME_BOY_INSTRUCTION
#define GAME_BOY_INSTRUCTION

#include <gameboy/Registers.h>

#include <type_traits>
#include <variant>

namespace gb
{
	enum Mnemonic
	{
		NOP,
		STOP, HALT,
		EI, DI,
		INC, DEC,
		LD,
		ADD, ADC,
		SUB, SBC,
		AND, OR, XOR,
		CP,
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

	template <typename RegType>
	struct RegInstruction : Instruction
	{
		RegType& reg;

	protected:
		RegInstruction (const Mnemonic mnemonic, RegType& reg)
		:
			Instruction { mnemonic },
			reg         { reg      }
		{
		}

		~RegInstruction() = default;

		static_assert(std::is_same_v<RegType, Reg8> || std::is_same_v<RegType, Reg16>);
	};
	using  Reg8Instruction = RegInstruction<Reg8>;
	using Reg16Instruction = RegInstruction<Reg16>;

	template <typename RegType>
	struct RegRegInstruction : Instruction
	{
		      RegType& dstReg;
		const RegType& srcReg;

	protected:
		RegRegInstruction (const Mnemonic mnemonic, RegType& dstReg, const RegType& srcReg)
		:
			Instruction { mnemonic },
			dstReg      { dstReg   },
			srcReg      { srcReg   }
		{
		}

		~RegRegInstruction() = default;

		static_assert(std::is_same_v<RegType, Reg8> || std::is_same_v<RegType, Reg16>);
	};
	using   Reg8Reg8Instruction = RegRegInstruction<Reg8>;
	using Reg16Reg16Instruction = RegRegInstruction<Reg16>;

	//
	//      x0
	//    +-----+
	// 0x | NOP |
	//    +-----+
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
	//       x0
	//    +------+
	// 1x | STOP |
	//    +------+
	//
	struct Stop : Instruction
	{
		Stop ()
		:
			Instruction { Mnemonic::STOP }
		{
		}
	};

	//
	//       x6
	//    +------+
	// 7x | HALT |
	//    +------+
	//
	struct Halt : Instruction
	{
		Halt ()
		:
			Instruction { Mnemonic::HALT }
		{
		}
	};

	//
	//      x3         xB
	//    +----+     +----+
	// Fx | DI | ... | EI |
	//    +----+     +----+
	//
	struct EnableInterrupt : Instruction
	{
		EnableInterrupt ()
		:
			Instruction { Mnemonic::EI }
		{
		}
	};
	struct DisableInterrupt : Instruction
	{
		DisableInterrupt ()
		:
			Instruction { Mnemonic::DI }
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

	template <typename RegType>
	struct AddRegReg : RegRegInstruction<RegType>
	{
		AddRegReg (RegType& dstReg, const RegType& srcReg)
		:
			RegRegInstruction { Mnemonic::ADD, dstReg, srcReg }
		{
		}
	};

	//
	//        x0        x1        x2        x3        x4        x5              x7
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// 8x | ADD A,B | ADD A,C | ADD A,D | ADD A,E | ADD A,H | ADD A,L |     | ADD A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	using AddReg8Reg8 = AddRegReg<Reg8>;

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

	//
	//        x0        x1        x2        x3        x4        x5              x7
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// Ax | AND A,B | AND A,C | AND A,D | AND A,E | AND A,H | AND A,L |     | AND A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct AndReg8Reg8 : Reg8Reg8Instruction
	{
		AndReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::AND, dstReg, srcReg }
		{
		}
	};

	//
	//        x0       x1       x2       x3       x4       x5             x7
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	// Bx | OR A,B | OR A,C | OR A,D | OR A,E | OR A,H | OR A,L |     | OR A,A |
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	//
	struct OrReg8Reg8 : Reg8Reg8Instruction
	{
		OrReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::OR, dstReg, srcReg }
		{
		}
	};

	//
	//        x8        x9        xA        xB        xC        xD              xF
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	// Ax | XOR A,B | XOR A,C | XOR A,D | XOR A,E | XOR A,H | XOR A,L |     | XOR A,A |
	//    +---------+---------+---------+---------+---------+---------+     +---------+
	//
	struct XorReg8Reg8 : Reg8Reg8Instruction
	{
		XorReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::XOR, dstReg, srcReg }
		{
		}
	};

	//
	//        x8       x9       xA       xB       xC       xD             xF
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	// Bx | CP A,B | CP A,C | CP A,D | CP A,E | CP A,H | CP A,L |     | CP A,A |
	//    +--------+--------+--------+--------+--------+--------+     +--------+
	//
	struct CompareReg8Reg8 : Reg8Reg8Instruction
	{
		CompareReg8Reg8 (Reg8& dstReg, const Reg8& srcReg)
		:
			Reg8Reg8Instruction { Mnemonic::CP, dstReg, srcReg }
		{
		}
	};

	template <typename Reg>
	struct IncrementReg : RegInstruction<Reg>
	{
		IncrementReg (Reg& reg)
		:
			RegInstruction { Mnemonic::INC , reg }
		{
		}
	};

	template <typename Reg>
	struct DecrementReg : RegInstruction<Reg>
	{
		DecrementReg (Reg& reg)
		:
			RegInstruction { Mnemonic::DEC , reg }
		{
		}
	};

	//
	//        x4            xC
	//    +-------+     +-------+
	// 0x | INC B |     | INC C |
	// 1x | INC D | ... | INC E |
	// 2x | INC H |     | INC L |
	// 3x |       |     | INC A |
	//    +-------+     +-------+
	//
	using IncrementReg8 = IncrementReg<Reg8>;

	//
	//        x5            xD
	//    +-------+     +-------+
	// 0x | DEC B |     | DEC C |
	// 1x | DEC D | ... | DEC E |
	// 2x | DEC H |     | DEC L |
	// 3x |       |     | DEC A |
	//    +-------+     +-------+
	//
	using DecrementReg8 = DecrementReg<Reg8>;

	//
	//        x3
	//    +--------+
	// 0x | INC BC |
	// 1x | INC DE |
	// 2x | INC HL |
	// 3x | INC SP |
	//    +--------+
	//
	using IncrementReg16 = IncrementReg<Reg16>;

	//
	//        xB
	//    +--------+
	// 0x | DEC BC |
	// 1x | DEC DE |
	// 2x | DEC HL |
	// 3x | DEC SP |
	//    +--------+
	//
	using DecrementReg16 = DecrementReg<Reg16>;

	//
	//          x9
	//    +-----------+
	// 0x | ADD HL,BC |
	// 1x | ADD HL,DE |
	// 2x | ADD HL,HL |
	// 3x | ADD HL,SP |
	//    +-----------+
	//
	using AddReg16Reg16 = AddRegReg<Reg16>;

	using AnyInstruction = std::variant
	<
		NoOperation,
		Stop,
		Halt,
		EnableInterrupt,
		DisableInterrupt,
		LoadReg8Reg8,
		LoadReg8Immediate,
		LoadMemoryReg8,
		AddReg8Reg8,
		AddWithCarryReg8Reg8,
		SubtractReg8Reg8,
		SubtractWithCarryReg8Reg8,
		AndReg8Reg8,
		OrReg8Reg8,
		XorReg8Reg8,
		CompareReg8Reg8,
		IncrementReg8,
		DecrementReg8,
		IncrementReg16,
		DecrementReg16,
		AddReg16Reg16
	>;
}

#endif
