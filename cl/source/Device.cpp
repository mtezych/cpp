
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

#include <cl/Device.h>

#include <util/util.h>

namespace cl
{
	cl_device_type
	Device::InfoResult<Device::Info::Type>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return util::ReinterpretBytes<cl_device_type>(infoBytes);
	}

	Device::Profile
	Device::InfoResult<Device::Info::Profile>::FromBytes
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

	Device::Version
	Device::InfoResult<Device::Info::Version>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = util::StringFromBytes(infoBytes);

		auto version = Device::Version { };

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
	Device::InfoResult<Device::Info::Vendor>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto vendorString = util::StringFromBytes(infoBytes);

		return vendorString;
	}

	std::string
	Device::InfoResult<Device::Info::Name>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto name = util::StringFromBytes(infoBytes);

		return name;
	}

	std::vector<std::string>
	Device::InfoResult<Device::Info::Extensions>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto extensionsString = util::StringFromBytes(infoBytes);

		const auto extensions = util::Split(extensionsString, ' ');

		return extensions;
	}

	cl_uint
	Device::InfoResult<Device::Info::VendorID>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return util::ReinterpretBytes<cl_uint>(infoBytes);
	}

	Device::Version
	Device::InfoResult<Device::Info::DriverVersion>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = util::StringFromBytes(infoBytes);

		auto version = Device::Version { };

		const auto argsParsedCount = std::sscanf
		(
			versionString.c_str(), "%u.%u", &version.major, &version.minor
		);
		assert(argsParsedCount == 2);

		return version;
	}
}
