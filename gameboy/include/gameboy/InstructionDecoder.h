
#ifndef GAME_BOY_INSTRUCTION_DECODER
#define GAME_BOY_INSTRUCTION_DECODER

#include <gameboy/Instruction.h>

#include <cassert>

namespace gb
{
	struct InstructionDecoder
	{
		AnyInstruction decode(const uint8_t byte, Registers& registers)
		{
			switch (byte)
			{
				case 0x00 :
				{
					return Nop { };
				}

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

				default :
				{
					assert(false);

					return Nop { };
				}
			}
		}
	};
}

#endif