
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

#include <cl/Platform.h>

#include <util/util.h>

namespace cl
{
	namespace
	{
		constexpr
		bool ValidateDeviceType (const cl_device_type deviceType)
		{
			const auto matchesAnyDeviceType = util::IsAnyBitSet
			(
				deviceType,
				CL_DEVICE_TYPE_DEFAULT     |
				CL_DEVICE_TYPE_CPU         |
				CL_DEVICE_TYPE_GPU         |
				CL_DEVICE_TYPE_ACCELERATOR |
				CL_DEVICE_TYPE_CUSTOM
			);

			return matchesAnyDeviceType || (deviceType == CL_DEVICE_TYPE_ALL);
		}
	}

	std::vector<Platform> GetPlatforms ()
	{
		auto platformIDsCount = cl_uint { 0 };
		auto result = clGetPlatformIDs(0, nullptr, &platformIDsCount);
		assert(result == CL_SUCCESS);

		auto platformIDs = std::vector<cl_platform_id>
		{
			platformIDsCount, nullptr
		};
		result = clGetPlatformIDs(platformIDsCount, platformIDs.data(), nullptr);
		assert(result == CL_SUCCESS);

		auto platforms = std::vector<Platform> { };

		for (const auto platformID : platformIDs)
		{
			platforms.push_back(Platform { platformID });
		}

		return platforms;
	}

	Platform::Profile
	Platform::InfoResult<Platform::Info::Profile>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto profile = util::StringFromBytes(infoBytes);

		if (profile == "FULL_PROFILE")
		{
			return Profile::Full;
		}
		else if (profile == "EMBEDDED_PROFILE")
		{
			return Profile::Embedded;
		}
		else
		{
			assert(false);
		}
	}

	Platform::Version
	Platform::InfoResult<Platform::Info::Version>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = util::StringFromBytes(infoBytes);

		auto version = Platform::Version { };

		auto charsParsedCount = unsigned { 0 };
		auto  argsParsedCount = std::sscanf
		(
			versionString.c_str(), "OpenCL %u.%u %n",
			&version.major, &version.minor, &charsParsedCount
		);
		assert( argsParsedCount == 2);
		assert(charsParsedCount  > 0);

		version.info = versionString.substr(charsParsedCount);

		return version;
	}

	std::string
	Platform::InfoResult<Platform::Info::Vendor>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto vendorString = util::StringFromBytes(infoBytes);

		return vendorString;
	}

	std::string
	Platform::InfoResult<Platform::Info::Name>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto name = util::StringFromBytes(infoBytes);

		return name;
	}

	std::vector<std::string>
	Platform::InfoResult<Platform::Info::Extensions>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto extensionsString = util::StringFromBytes(infoBytes);

		const auto extensions = util::Split(extensionsString, ' ');

		return extensions;
	}

	std::vector<Device>
	Platform::GetDevices (const cl_device_type deviceType) const
	{
		assert(ValidateDeviceType(deviceType));

		auto deviceIDsCount = cl_uint { 0 };
		auto result = clGetDeviceIDs
		(
			clPlatformID, deviceType,
			0, nullptr,
			&deviceIDsCount
		);
		assert(result == CL_SUCCESS);

		auto deviceIDs = std::vector<cl_device_id> { deviceIDsCount, nullptr };
		result = clGetDeviceIDs
		(
			clPlatformID, deviceType,
			deviceIDsCount, deviceIDs.data(),
			nullptr
		);
		assert(result == CL_SUCCESS);

		auto devices = std::vector<Device> { };

		for (const auto deviceID : deviceIDs)
		{
			devices.push_back(Device { deviceID });
		}

		return devices;
	}
}
