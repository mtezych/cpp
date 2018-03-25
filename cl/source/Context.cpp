
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

#include <array>
#include <algorithm>

namespace cl
{
	Context::Context
	(
		const Platform&            platform,
		const std::vector<Device>& devices
	):
		clContext { nullptr }
	{
		const auto properties = std::array<cl_context_properties, 3>
		{
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(platform.clPlatformID),
			0
		};

		auto clDevices = std::vector<cl_device_id>
		{
			devices.size(), nullptr
		};
		std::transform
		(
			devices.begin(), devices.end(), clDevices.begin(),

			[](const Device& device) -> cl_device_id
			{
				return device.clDeviceID;
			}
		);

		auto result = cl_int { CL_INVALID_CONTEXT };
		clContext = clCreateContext
		(
			properties.data(),
			static_cast<cl_uint>(clDevices.size()), clDevices.data(),
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

	namespace
	{
		template <typename Type>
		const Type& ReinterpretBytes (const std::vector<std::byte>& bytes)
		{
			assert(bytes.size() == sizeof(Type));

			return *static_cast<const Type*>(static_cast<const void*>(bytes.data()));
		}
	}

	cl_uint
	Context::InfoResult<CL_CONTEXT_NUM_DEVICES>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return ReinterpretBytes<cl_uint>(infoBytes);
	}

	cl_uint
	Context::InfoResult<CL_CONTEXT_REFERENCE_COUNT>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return ReinterpretBytes<cl_uint>(infoBytes);
	}
}
