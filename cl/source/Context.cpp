
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

#include <cl/Context.h>

#include <cl/Platform.h>
#include <cl/Device.h>

#include <array>

namespace cl
{
	Context::Context
	(
		const Platform&               platform,
		const gsl::span<const Device> devices
	):
		clContext { nullptr }
	{
		const auto properties = std::array<cl_context_properties, 3>
		{
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(platform.clPlatformID),
			0
		};

		static_assert(sizeof(Device) == sizeof(cl_device_id));

		auto result = cl_int { CL_INVALID_CONTEXT };
		clContext = clCreateContext
		(
			properties.data(),
			static_cast<cl_uint>(devices.size()),
			reinterpret_cast<const cl_device_id*>(util::data_or_null(devices)),
			nullptr, nullptr,
			&result
		);
		assert(result == CL_SUCCESS);
	}

	Context::Context
	(
		const Platform&      platform,
		const cl_device_type deviceType
	):
		clContext { nullptr }
	{
		const auto properties = std::array<cl_context_properties, 3>
		{
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(platform.clPlatformID),
			0
		};

		auto result = cl_int { CL_INVALID_CONTEXT };
		clContext = clCreateContextFromType
		(
			properties.data(),
			deviceType,
			nullptr, nullptr,
			&result
		);
		assert(result == CL_SUCCESS);
	}

	Context::~Context ()
	{
		if (clContext != nullptr)
		{
			clReleaseContext(clContext);
		}
	}

	Context::Context (Context&& context)
	:
		clContext { context.clContext }
	{
		context.clContext = nullptr;
	}

	Context& Context::operator = (Context&& context)
	{
		if (clContext != nullptr)
		{
			clReleaseContext(clContext);
		}

		clContext = context.clContext;

		context.clContext = nullptr;

		return *this;
	}

	cl_uint
	Context::InfoResult<Context::Info::NumDevices>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return util::ReinterpretBytes<cl_uint>(infoBytes);
	}

	cl_uint
	Context::InfoResult<Context::Info::ReferenceCount>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return util::ReinterpretBytes<cl_uint>(infoBytes);
	}
}
