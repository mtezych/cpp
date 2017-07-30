
#ifndef GAME_BOY_REGISTERS
#define GAME_BOY_REGISTERS

#include <cstdint>

namespace gb
{
	using Reg8  = uint8_t;
	using Reg16 = uint16_t;

	//
	// Flags register
	//
	//   7   6   5   4   3   2   1   0
	// +---+---+---+---+---+---+---+---+
	// | Z | N | H | C | 0 | 0 | 0 | 0 |
	// +---+---+---+---+---+---+---+---+
	//
	// X -> Zero Flag
	//   This bit is set when:
	//    - the result of a math operation is zero
	//    - two values match when using the CP instruction
	//
	// N -> Subtract Flag
	//   This bit is set if:
	//    - subtraction was perfomed in the last math instruction
	//
	// H -> Half Carry Flag
	//   This bit is set if:
	//    - carry occured from the lower nibble in the last math operation
	//
	// C -> Carry Flag
	//   This bit is set if:
	//    - carry occurred from the last math operation
	//    - register A is the smaller value when executing the CP instruction
	//
	union Flags
	{
		Reg8   : 4;
		Reg8 C : 1;
		Reg8 H : 1;
		Reg8 N : 1;
		Reg8 Z : 1;
	};
	static_assert(sizeof(Flags) == sizeof(Reg8), "Flags register is 8-bit.");

	//
	// GameBoy has eight 8-bit registers A, B, C, D, E, F, H, L
	// and two 16-bit registers SP and PC.
	//
	// +---------+---------+
	// |15 ... 8 | 7 ... 0 |
	// +---------+---------+
	// |    A    |    F    |
	// |    B    |    C    |
	// |    D    |    E    |
	// |    H    |    L    |
	// |-------------------|
	// |        S P        |
	// |        P C        |
	// +-------------------+
	//
	// Some instructions allow using registers 8-bit registers as
	// 16-bit registers by paring them up in the following manner:
	// AF, BC, DE and HL.
	//
	// The A (Accumulator) register is used to store data and the results of
	// arithmetic and logical operations.
	//
	// The SP (Stack Pointer) register points to the current stack position.
	//
	// The PC (Program Counter) register points to the next instruction
	// to be executed in the GameBoy memory.
	//
	struct Registers
	{
		union
		{
			struct
			{
				Flags F;
				Reg8  A; // Accumulator
			};
			Reg16 AF;
		};

		union
		{
			struct
			{
				Reg8 C;
				Reg8 B;
			};
			Reg16 BC;
		};

		union
		{
			struct
			{
				Reg8 E;
				Reg8 D;
			};
			Reg16 DE;
		};

		union
		{
			struct
			{
				Reg8 L;
				Reg8 H;
			};
			Reg16 HL;
		};

		Reg16 SP; // Stack Pointer
		Reg16 PC; // Program Counter
	};
}

#endif
