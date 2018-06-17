
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

#ifndef CL_PLATFORM
#define CL_PLATFORM

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

#include <cl/Device.h>

#include <util/util.h>

#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <cstddef>

namespace cl
{
	struct Platform
	{
		cl_platform_id clPlatformID;

		enum class Profile
		{
			Full,
			Embedded,
		};

		struct Version
		{
			std::uint32_t major;
			std::uint32_t minor;
			std::string   info;
		};

		enum class Info : cl_platform_info
		{
			Profile    = CL_PLATFORM_PROFILE,
			Version    = CL_PLATFORM_VERSION,
			Name       = CL_PLATFORM_NAME,
			Vendor     = CL_PLATFORM_VENDOR,
			Extensions = CL_PLATFORM_EXTENSIONS,
		};

		template <Info info>
		auto GetInfo () const
		{
			auto infoSize = std::size_t { 0 };
			auto result = clGetPlatformInfo
			(
				clPlatformID, util::enum_cast(info),
				0, nullptr, &infoSize
			);
			assert(result == CL_SUCCESS);

			auto infoBytes = std::vector<std::byte>
			{
				infoSize, std::byte { 0x00 }
			};
			result = clGetPlatformInfo
			(
				clPlatformID, util::enum_cast(info),
				infoBytes.size(), infoBytes.data(), nullptr
			);
			assert(result == CL_SUCCESS);

			return InfoResult<info>::FromBytes(infoBytes);
		}

		std::vector<Device>
		GetDevices(const cl_device_type deviceType = CL_DEVICE_TYPE_ALL) const;

	private:

		template <Info info>
		struct InfoResult;
	};

	std::vector<Platform> GetPlatforms ();

	template <>
	struct Platform::InfoResult<Platform::Info::Profile>
	{
		static Profile
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Platform::InfoResult<Platform::Info::Version>
	{
		static Version
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Platform::InfoResult<Platform::Info::Vendor>
	{
		static std::string
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Platform::InfoResult<Platform::Info::Name>
	{
		static std::string
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Platform::InfoResult<Platform::Info::Extensions>
	{
		static std::vector<std::string>
		FromBytes (const std::vector<std::byte>& infoBytes);
	};
}

#endif
