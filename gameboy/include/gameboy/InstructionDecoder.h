
#ifndef GAME_BOY_INSTRUCTION_DECODER
#define GAME_BOY_INSTRUCTION_DECODER

#include <gameboy/Instruction.h>

#include <cassert>

namespace gb
{
	struct InstructionDecoder
	{
		//
		//     x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
		//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		// 0x |##|##|  |##|##|##|##|  |  |##|  |##|##|##|##|  |
		// 1x |##|##|  |##|##|##|##|  |  |##|  |##|##|##|##|  |
		// 2x |  |##|  |##|##|##|##|  |  |##|  |##|##|##|##|  |
		// 3x |  |##|  |##|  |  |  |  |  |##|  |##|##|##|##|  |
		// 4x |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// 5x |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// 6x |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// 7x |  |  |  |  |  |  |##|  |##|##|##|##|##|##|  |##|
		// 8x |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// 9x |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// Ax |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// Bx |##|##|##|##|##|##|  |##|##|##|##|##|##|##|  |##|
		// Cx |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
		// Dx |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
		// Ex |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
		// Fx |  |  |  |##|  |  |  |  |  |##|  |##|  |  |  |  |
		//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		//
		AnyInstruction decode (const uint8_t byte, Registers& registers)
		{
			switch (byte)
			{
				// NoOperation
				{
					case 0x00 :
					{
						return NoOperation { };
					}
				}

				// Stop
				{
					case 0x10 :
					{
						// @todo: The STOP instruction is 2-byte wide
						//        with second byte expected to be 0x00.
						//        Ignore next byte by advancuing Program Counter.
						return Stop { };
					}
				}

				// Halt
				{
					case 0x76 :
					{
						return Halt { };
					}
				}

				// DisableInterrupt
				{
					case 0xF3 :
					{
						return DisableInterrupt { };
					}
					case 0xFB :
					{
						return EnableInterrupt { };
					}
				}

				// Load Reg8, Imm8
				{
					// @todo: The LOAD Reg8, Imm8 instruction is 2-byte wide
					//        with second byte defining value of the immediate.
					case 0x06 :
					{
						return LoadReg8Imm8 { registers.B, 0xff };
					}
					case 0x0E :
					{
						return LoadReg8Imm8 { registers.C, 0xff };
					}
					case 0x16 :
					{
						return LoadReg8Imm8 { registers.D, 0xff };
					}
					case 0x1E :
					{
						return LoadReg8Imm8 { registers.E, 0xff };
					}
					case 0x26 :
					{
						return LoadReg8Imm8 { registers.H, 0xff };
					}
					case 0x2E :
					{
						return LoadReg8Imm8 { registers.L, 0xff };
					}
					case 0x3E :
					{
						return LoadReg8Imm8 { registers.A, 0xff };
					}
				}

				// Load Reg16, Imm16
				{
					// @todo: The LOAD Reg16, Imm16 instruction is 3-byte wide
					//        with second and third bytes defining value of the immediate.
					case 0x01 :
					{
						return LoadReg16Imm16 { registers.BC, 0xFFFF };
					}
					case 0x11 :
					{
						return LoadReg16Imm16 { registers.DE, 0xFFFF };
					}
					case 0x21 :
					{
						return LoadReg16Imm16 { registers.HL, 0xFFFF };
					}
					case 0x31 :
					{
						return LoadReg16Imm16 { registers.SP, 0xFFFF };
					}
				}

				// Load Reg8, Reg8
				{
					case 0x40 :
					{
						return LoadReg8Reg8 { registers.B, registers.B };
					}
					case 0x41 :
					{
						return LoadReg8Reg8 { registers.B, registers.C };
					}
					case 0x42 :
					{
						return LoadReg8Reg8 { registers.B, registers.D };
					}
					case 0x43 :
					{
						return LoadReg8Reg8 { registers.B, registers.E };
					}
					case 0x44 :
					{
						return LoadReg8Reg8 { registers.B, registers.H };
					}
					case 0x45 :
					{
						return LoadReg8Reg8 { registers.B, registers.L };
					}
					case 0x47 :
					{
						return LoadReg8Reg8 { registers.B, registers.A };
					}

					case 0x48 :
					{
						return LoadReg8Reg8 { registers.C, registers.B };
					}
					case 0x49 :
					{
						return LoadReg8Reg8 { registers.C, registers.C };
					}
					case 0x4A :
					{
						return LoadReg8Reg8 { registers.C, registers.D };
					}
					case 0x4B :
					{
						return LoadReg8Reg8 { registers.C, registers.E };
					}
					case 0x4C :
					{
						return LoadReg8Reg8 { registers.C, registers.H };
					}
					case 0x4D :
					{
						return LoadReg8Reg8 { registers.C, registers.L };
					}
					case 0x4F :
					{
						return LoadReg8Reg8 { registers.C, registers.A };
					}

					case 0x50 :
					{
						return LoadReg8Reg8 { registers.D, registers.B };
					}
					case 0x51 :
					{
						return LoadReg8Reg8 { registers.D, registers.C };
					}
					case 0x52 :
					{
						return LoadReg8Reg8 { registers.D, registers.D };
					}
					case 0x53 :
					{
						return LoadReg8Reg8 { registers.D, registers.E };
					}
					case 0x54 :
					{
						return LoadReg8Reg8 { registers.D, registers.H };
					}
					case 0x55 :
					{
						return LoadReg8Reg8 { registers.D, registers.L };
					}
					case 0x57 :
					{
						return LoadReg8Reg8 { registers.D, registers.A };
					}

					case 0x58 :
					{
						return LoadReg8Reg8 { registers.E, registers.B };
					}
					case 0x59 :
					{
						return LoadReg8Reg8 { registers.E, registers.C };
					}
					case 0x5A :
					{
						return LoadReg8Reg8 { registers.E, registers.D };
					}
					case 0x5B :
					{
						return LoadReg8Reg8 { registers.E, registers.E };
					}
					case 0x5C :
					{
						return LoadReg8Reg8 { registers.E, registers.H };
					}
					case 0x5D :
					{
						return LoadReg8Reg8 { registers.E, registers.L };
					}
					case 0x5F :
					{
						return LoadReg8Reg8 { registers.E, registers.A };
					}

					case 0x60 :
					{
						return LoadReg8Reg8 { registers.H, registers.B };
					}
					case 0x61 :
					{
						return LoadReg8Reg8 { registers.H, registers.C };
					}
					case 0x62 :
					{
						return LoadReg8Reg8 { registers.H, registers.D };
					}
					case 0x63 :
					{
						return LoadReg8Reg8 { registers.H, registers.E };
					}
					case 0x64 :
					{
						return LoadReg8Reg8 { registers.H, registers.H };
					}
					case 0x65 :
					{
						return LoadReg8Reg8 { registers.H, registers.L };
					}
					case 0x67 :
					{
						return LoadReg8Reg8 { registers.H, registers.A };
					}

					case 0x68 :
					{
						return LoadReg8Reg8 { registers.L, registers.B };
					}
					case 0x69 :
					{
						return LoadReg8Reg8 { registers.L, registers.C };
					}
					case 0x6A :
					{
						return LoadReg8Reg8 { registers.L, registers.D };
					}
					case 0x6B :
					{
						return LoadReg8Reg8 { registers.L, registers.E };
					}
					case 0x6C :
					{
						return LoadReg8Reg8 { registers.L, registers.H };
					}
					case 0x6D :
					{
						return LoadReg8Reg8 { registers.L, registers.L };
					}
					case 0x6F :
					{
						return LoadReg8Reg8 { registers.L, registers.A };
					}

					case 0x78 :
					{
						return LoadReg8Reg8 { registers.A, registers.B };
					}
					case 0x79 :
					{
						return LoadReg8Reg8 { registers.A, registers.C };
					}
					case 0x7A :
					{
						return LoadReg8Reg8 { registers.A, registers.D };
					}
					case 0x7B :
					{
						return LoadReg8Reg8 { registers.A, registers.E };
					}
					case 0x7C :
					{
						return LoadReg8Reg8 { registers.A, registers.H };
					}
					case 0x7D :
					{
						return LoadReg8Reg8 { registers.A, registers.L };
					}
					case 0x7F :
					{
						return LoadReg8Reg8 { registers.A, registers.A };
					}
				}

				// Load Reg16, Reg16
				{
					case 0xF9 :
					{
						return LoadReg16Reg16 { registers.SP, registers.HL };
					}
				}

				// Add Reg8, Reg8
				{
					case 0x80 :
					{
						return AddReg8Reg8 { registers.A, registers.B };
					}
					case 0x81 :
					{
						return AddReg8Reg8 { registers.A, registers.C };
					}
					case 0x82 :
					{
						return AddReg8Reg8 { registers.A, registers.D };
					}
					case 0x83 :
					{
						return AddReg8Reg8 { registers.A, registers.E };
					}
					case 0x84 :
					{
						return AddReg8Reg8 { registers.A, registers.H };
					}
					case 0x85 :
					{
						return AddReg8Reg8 { registers.A, registers.L };
					}
					case 0x87 :
					{
						return AddReg8Reg8 { registers.A, registers.A };
					}
				}

				// AddWithCarry Reg8, Reg8
				{
					case 0x88 :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.B };
					}
					case 0x89 :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.C };
					}
					case 0x8A :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.D };
					}
					case 0x8B :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.E };
					}
					case 0x8C :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.H };
					}
					case 0x8D :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.L };
					}
					case 0x8F :
					{
						return AddWithCarryReg8Reg8 { registers.A, registers.A };
					}
				}

				// Subtract Reg8, Reg8
				{
					case 0x90 :
					{
						return SubtractReg8Reg8 { registers.A, registers.B };
					}
					case 0x91 :
					{
						return SubtractReg8Reg8 { registers.A, registers.C };
					}
					case 0x92 :
					{
						return SubtractReg8Reg8 { registers.A, registers.D };
					}
					case 0x93 :
					{
						return SubtractReg8Reg8 { registers.A, registers.E };
					}
					case 0x94 :
					{
						return SubtractReg8Reg8 { registers.A, registers.H };
					}
					case 0x95 :
					{
						return SubtractReg8Reg8 { registers.A, registers.L };
					}
					case 0x97 :
					{
						return SubtractReg8Reg8 { registers.A, registers.A };
					}
				}

				// SubtractWithCarry Reg8, Reg8
				{
					case 0x98 :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.B };
					}
					case 0x99 :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.C };
					}
					case 0x9A :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.D };
					}
					case 0x9B :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.E };
					}
					case 0x9C :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.H };
					}
					case 0x9D :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.L };
					}
					case 0x9F :
					{
						return SubtractWithCarryReg8Reg8 { registers.A, registers.A };
					}
				}

				// And Reg8, Reg8
				{
					case 0xA0 :
					{
						return AndReg8Reg8 { registers.A, registers.B };
					}
					case 0xA1 :
					{
						return AndReg8Reg8 { registers.A, registers.C };
					}
					case 0xA2 :
					{
						return AndReg8Reg8 { registers.A, registers.D };
					}
					case 0xA3 :
					{
						return AndReg8Reg8 { registers.A, registers.E };
					}
					case 0xA4 :
					{
						return AndReg8Reg8 { registers.A, registers.H };
					}
					case 0xA5 :
					{
						return AndReg8Reg8 { registers.A, registers.L };
					}
					case 0xA7 :
					{
						return AndReg8Reg8 { registers.A, registers.A };
					}
				}

				// Xor Reg8, Reg8
				{
					case 0xA8 :
					{
						return XorReg8Reg8 { registers.A, registers.B };
					}
					case 0xA9 :
					{
						return XorReg8Reg8 { registers.A, registers.C };
					}
					case 0xAA :
					{
						return XorReg8Reg8 { registers.A, registers.D };
					}
					case 0xAB :
					{
						return XorReg8Reg8 { registers.A, registers.E };
					}
					case 0xAC :
					{
						return XorReg8Reg8 { registers.A, registers.H };
					}
					case 0xAD :
					{
						return XorReg8Reg8 { registers.A, registers.L };
					}
					case 0xAF :
					{
						return XorReg8Reg8 { registers.A, registers.A };
					}
				}

				// Or Reg8, Reg8
				{
					case 0xB0 :
					{
						return OrReg8Reg8 { registers.A, registers.B };
					}
					case 0xB1 :
					{
						return OrReg8Reg8 { registers.A, registers.C };
					}
					case 0xB2 :
					{
						return OrReg8Reg8 { registers.A, registers.D };
					}
					case 0xB3 :
					{
						return OrReg8Reg8 { registers.A, registers.E };
					}
					case 0xB4 :
					{
						return OrReg8Reg8 { registers.A, registers.H };
					}
					case 0xB5 :
					{
						return OrReg8Reg8 { registers.A, registers.L };
					}
					case 0xB7 :
					{
						return OrReg8Reg8 { registers.A, registers.A };
					}
				}

				// Compare Reg8, Reg8
				{
					case 0xB8 :
					{
						return CompareReg8Reg8 { registers.A, registers.B };
					}
					case 0xB9 :
					{
						return CompareReg8Reg8 { registers.A, registers.C };
					}
					case 0xBA :
					{
						return CompareReg8Reg8 { registers.A, registers.D };
					}
					case 0xBB :
					{
						return CompareReg8Reg8 { registers.A, registers.E };
					}
					case 0xBC :
					{
						return CompareReg8Reg8 { registers.A, registers.H };
					}
					case 0xBD :
					{
						return CompareReg8Reg8 { registers.A, registers.L };
					}
					case 0xBF :
					{
						return CompareReg8Reg8 { registers.A, registers.A };
					}
				}

				// Increment Reg8
				{
					case 0x04 :
					{
						return IncrementReg8 { registers.B };
					}
					case 0x0C :
					{
						return IncrementReg8 { registers.C };
					}
					case 0x14 :
					{
						return IncrementReg8 { registers.D };
					}
					case 0x1C :
					{
						return IncrementReg8 { registers.E };
					}
					case 0x24 :
					{
						return IncrementReg8 { registers.H };
					}
					case 0x2C :
					{
						return IncrementReg8 { registers.L };
					}
					case 0x3C :
					{
						return IncrementReg8 { registers.A };
					}
				}

				// Decrement Reg8
				{
					case 0x05 :
					{
						return DecrementReg8 { registers.B };
					}
					case 0x0D :
					{
						return DecrementReg8 { registers.C };
					}
					case 0x15 :
					{
						return DecrementReg8 { registers.D };
					}
					case 0x1D :
					{
						return DecrementReg8 { registers.E };
					}
					case 0x25 :
					{
						return DecrementReg8 { registers.H };
					}
					case 0x2D :
					{
						return DecrementReg8 { registers.L };
					}
					case 0x3D :
					{
						return DecrementReg8 { registers.A };
					}
				}

				// Increment Reg16
				{
					case 0x03 :
					{
						return IncrementReg16 { registers.BC };
					}
					case 0x13 :
					{
						return IncrementReg16 { registers.DE };
					}
					case 0x23 :
					{
						return IncrementReg16 { registers.HL };
					}
					case 0x33 :
					{
						return IncrementReg16 { registers.SP };
					}
				}

				// Decrement Reg16
				{
					case 0x0B :
					{
						return DecrementReg16 { registers.BC };
					}
					case 0x1B :
					{
						return DecrementReg16 { registers.DE };
					}
					case 0x2B :
					{
						return DecrementReg16 { registers.HL };
					}
					case 0x3B :
					{
						return DecrementReg16 { registers.SP };
					}
				}

				// Add Reg16, Reg16
				{
					case 0x09 :
					{
						return AddReg16Reg16 { registers.HL, registers.BC };
					}
					case 0x19 :
					{
						return AddReg16Reg16 { registers.HL, registers.DE };
					}
					case 0x29 :
					{
						return AddReg16Reg16 { registers.HL, registers.HL };
					}
					case 0x39 :
					{
						return AddReg16Reg16 { registers.HL, registers.SP };
					}
				}

				default :
				{
					assert(false);

					return NoOperation { };
				}
			}
		}
	};
}

#endif