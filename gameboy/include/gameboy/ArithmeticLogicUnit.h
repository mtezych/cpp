
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

		void Execute (Nop& nop)
		{
			static_cast<void>(nop);
		}

		void Execute (LoadReg8Reg8& load)
		{
			load.dst = load.src;
		}

		void Execute (LoadMemReg8& load)
		{
		}
	};
}

#endif
