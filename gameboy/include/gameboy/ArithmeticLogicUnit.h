
#ifndef GAME_BOY_ARITHMETIC_LOGIC_UNIT
#define GAME_BOY_ARITHMETIC_LOGIC_UNIT

#include <gameboy/Instruction.h>

namespace gb
{
	struct ArithmeticLogicUnit
	{
		void Execute (AnyInstruction& instruction)
		{
			const auto execute = [this](auto& instruction)
			{
				Execute(instruction);
			};
			std::visit(execute, instruction);
		}

		void Execute (NoOperation& noOperation)
		{
			static_cast<void>(noOperation);
		}

		void Execute (LoadReg8Reg8& load)
		{
			load.dstReg = load.srcReg;
		}

		void Execute (LoadReg8Immediate& load)
		{
			// assert(load.immediate == memory.Read(registers.PC));
			// load.reg = load.immediate;
		}

		void Execute (LoadMemoryReg8& load)
		{
		}

		void Execute (AddReg8Reg8& add)
		{
			// add.dstReg += add.srcReg;
		}

		void Execute (SubtractReg8Reg8& subtract)
		{
			// subtract.dstReg -= subtract.srcReg;
		}

		void Execute (AddWithCarryReg8Reg8& addWithCarry)
		{
			// addWithCarry.dstReg += (addWithCarry.srcReg + flags.C);
		}

		void Execute (SubtractWithCarryReg8Reg8& subtractWithCarry)
		{
			// subtractWithCarry.dstReg -= (subtractWithCarry.srcReg + flags.C);
		}

		void Execute (AndReg8Reg8& and)
		{
			// and.dstReg &= and.srcReg;
		}

		void Execute (OrReg8Reg8& or)
		{
			// or.dstReg |= or.srcReg;
		}

		void Execute (XorReg8Reg8& xor)
		{
			// xor.dstReg ^= xor.srcReg;
		}

		void Execute (CompareReg8Reg8& compare)
		{
			// if (compare.srcReg == compare.dstReg) { }
		}

		void Execute (IncrementReg8& increment)
		{
			// ++increment.reg;
		}

		void Execute (DecrementReg8& decrement)
		{
			// --decrement.reg;
		}

		void Execute (IncrementReg16& increment)
		{
			// ++increment.reg;
		}

		void Execute (DecrementReg16& decrement)
		{
			// --decrement.reg;
		}
	};
}

#endif
