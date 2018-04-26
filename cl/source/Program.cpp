
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, mtezych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cl/Program.h>

#include <cl/Context.h>

#include <util/util.h>

#include <cassert>
#include <cstddef>

namespace cl
{
	Program::Program (const Context& context, const std::string& sourceCode)
	:
		clProgram { nullptr }
	{
		auto sourceCodeLine = sourceCode.c_str();

		auto result = cl_int { CL_INVALID_PROGRAM };
		clProgram = clCreateProgramWithSource
		(
			context.clContext,
			1, &sourceCodeLine, nullptr,
			&result
		);
		assert(result == CL_SUCCESS);
	}

	Program::~Program ()
	{
		if (clProgram != nullptr)
		{
			clReleaseProgram(clProgram);
		}
	}

	Program::Program (Program&& program)
	:
		clProgram { program.clProgram }
	{
		program.clProgram = nullptr;
	}

	Program& Program::operator = (Program&& program)
	{
		if (clProgram != nullptr)
		{
			clReleaseProgram(clProgram);
		}

		clProgram = program.clProgram;

		program.clProgram = nullptr;

		return *this;
	}

	void Program::Build ()
	{
		const auto result = clBuildProgram
		(
			clProgram,
			0, nullptr,
			nullptr,
			nullptr, nullptr
		);
		assert(result == CL_SUCCESS);
	}

	cl_build_status
	Program::InfoResult<CL_PROGRAM_BUILD_STATUS>::FromBytes
	(
		const std::vector<std::byte>& bytes
	)
	{
		return util::ReinterpretBytes<cl_build_status>(bytes);
	}

	std::string
	Program::InfoResult<CL_PROGRAM_BUILD_LOG>::FromBytes
	(
		const std::vector<std::byte>& bytes
	)
	{
		return util::StringFromBytes(bytes);
	}
}
