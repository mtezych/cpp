
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

#ifndef CL_DEVICE
#define CL_DEVICE

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

#include <util/util.h>

#include <vector>
#include <string>
#include <cassert>
#include <cstddef>

namespace cl
{
	struct Device
	{
		cl_device_id clDeviceID;

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

		enum class Info : cl_device_info
		{
			Type          = CL_DEVICE_TYPE,
			Profile       = CL_DEVICE_PROFILE,
			Version       = CL_DEVICE_VERSION,
			Vendor        = CL_DEVICE_VENDOR,
			Name          = CL_DEVICE_NAME,
			Extensions    = CL_DEVICE_EXTENSIONS,
			VendorID      = CL_DEVICE_VENDOR_ID,
			DriverVersion = CL_DRIVER_VERSION,
		};

		template <Info info>
		auto GetInfo() const
		{
			auto infoSize = std::size_t { 0 };
			auto result = clGetDeviceInfo
			(
				clDeviceID, util::enum_cast(info),
				0, nullptr, &infoSize
			);
			assert(result == CL_SUCCESS);

			auto infoBytes = std::vector<std::byte>
			{
				infoSize, std::byte { 0x00 }
			};
			result = clGetDeviceInfo
			(
				clDeviceID, util::enum_cast(info),
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
	struct Device::InfoResult<Device::Info::Type>
	{
		static cl_device_type
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::Profile>
	{
		static Profile
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::Version>
	{
		static Version
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::Vendor>
	{
		static std::string
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::Name>
	{
		static std::string
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::Extensions>
	{
		static std::vector<std::string>
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::VendorID>
	{
		static cl_uint
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct Device::InfoResult<Device::Info::DriverVersion>
	{
		static Version
		FromBytes (const std::vector<std::byte>& infoBytes);
	};
}

#endif
