
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

#ifndef CL_CONTEXT
#define CL_CONTEXT

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

#include <cl/Device.h>

#include <util/util.h>

#include <gsl/span>

#include <vector>
#include <cstddef>

namespace cl
{
	struct Platform;

	struct Context
	{
		cl_context clContext;

		//
		// @note: The NotifyCallback must be thread-safe.
		//
		using NotifyCallback = void (CL_CALLBACK *)
		(
			const char* errorInfo,
			const void* privateInfo, size_t cb,
			void* userData
		);

		Context
		(
			const Platform&         platform,
			gsl::span<const Device> devices
		);

		Context
		(
			const Platform& platform,
			cl_device_type  deviceType
		);

		~Context ();

		Context (Context&& context);
		Context (const Context& context) = delete;

		Context& operator = (Context&& context);
		Context& operator = (const Context& context) = delete;

		enum class Info : cl_context_info
		{
			NumDevices     = CL_CONTEXT_NUM_DEVICES,
			ReferenceCount = CL_CONTEXT_REFERENCE_COUNT,
		};

		template <Info info>
		auto GetInfo() const
		{
			auto infoSize = std::size_t { 0 };
			auto result = clGetContextInfo
			(
				clContext, util::enum_cast(info),
				0, nullptr, &infoSize
			);
			assert(result == CL_SUCCESS);

			auto infoBytes = std::vector<std::byte>
			{
				infoSize, std::byte { 0x00 }
			};
			result = clGetContextInfo
			(
				clContext, util::enum_cast(info),
				infoBytes.size(), infoBytes.data(), nullptr
			);
			assert(result == CL_SUCCESS);

			return InfoResult<info>::FromBytes(infoBytes);
		}

	private:

		template <Info info>
		struct InfoResult;
	};

	template <>
	struct Context::InfoResult<Context::Info::NumDevices>
	{
		static cl_uint
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Context::InfoResult<Context::Info::ReferenceCount>
	{
		static cl_uint
		FromBytes (const std::vector<std::byte>& infoBytes);
	};
}

#endif
