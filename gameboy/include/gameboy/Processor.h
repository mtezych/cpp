
#ifndef GAME_BOY_PROCESSOR
#define GAME_BOY_PROCESSOR

#include <gameboy/Registers.h>
#include <gameboy/ArithmeticLogicUnit.h>
#include <gameboy/InstructionDecoder.h>

#include <vector>

namespace gb
{
	struct Processor
	{
		Registers           registers;
		ArithmeticLogicUnit alu;
		InstructionDecoder  decoder;

		void Run (const std::vector<uint8_t>& code)
		{
			for (const auto byte : code)
			{
				auto instruction = decoder.decode(byte, registers);

				alu.Execute(instruction);
			}
		}
	};
}

#endif
