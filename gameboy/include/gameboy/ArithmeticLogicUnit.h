
#ifndef GAME_BOY_ARITHMETIC_LOGIC_UNIT
#define GAME_BOY_ARITHMETIC_LOGIC_UNIT

#include <gameboy/Instruction.h>

namespace gb
{
	struct ArithmeticLogicUnit
	{
		void Execute (AnyInstruction& instruction)
		{
			const auto execute = [this](auto& instrucation)
			{
				Execute(instrucation);
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
			// load.dstReg = load.srcReg;
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

		void Execute(SubtractWithCarryReg8Reg8& subtractWithCarry)
		{
			// subtractWithCarry.dstReg -= (subtractWithCarry.srcReg + flags.C);
		}
	};
}

#endif
