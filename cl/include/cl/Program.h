
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

#ifndef CL_PROGRAM
#define CL_PROGRAM

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

#include <cl/Device.h>

#include <string>
#include <vector>
#include <cstddef>

namespace cl
{
	struct Context;

	struct Program
	{
		cl_program clProgram;

		Program (const Context& context, const std::string& sourceCode);

		~Program ();

		Program (Program&& program);
		Program (const Program& program) = delete;

		Program& operator = (Program&& program);
		Program& operator = (const Program& program) = delete;

		void Build ();

		template <cl_program_build_info Info>
		auto BuildInfo (const Device& device)
		{
			auto infoSize = size_t { 0 };
			auto result = clGetProgramBuildInfo
			(
				clProgram, device.clDeviceID,
				Info,
				0, nullptr,
				&infoSize
			);
			assert(result == CL_SUCCESS);

			auto infoBytes = std::vector<std::byte>
			{
				infoSize, std::byte { 0x00 }
			};
			result = clGetProgramBuildInfo
			(
				clProgram, device.clDeviceID,
				Info,
				infoBytes.size(), infoBytes.data(),
				nullptr
			);
			assert(result == CL_SUCCESS);

			return InfoResult<Info>::FromBytes(infoBytes);
		}

	private:
		template <cl_program_build_info Info>
		struct InfoResult;
	};

	template <>
	struct Program::InfoResult<CL_PROGRAM_BUILD_STATUS>
	{
		static cl_build_status
		FromBytes (const std::vector<std::byte>& bytes);
	};

	template <>
	struct Program::InfoResult<CL_PROGRAM_BUILD_LOG>
	{
		static std::string
		FromBytes (const std::vector<std::byte> &bytes);
	};
}

#endif
