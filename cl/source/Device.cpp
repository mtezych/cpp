
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

#include <sstream>

namespace cl
{
	namespace
	{
		std::string
		StringFromBytes (const std::vector<std::byte>& bytes)
		{
			return std::string
			{
				static_cast<const char*>(static_cast<const void*>(bytes.data()))
			};
		}

		std::vector<std::string>
		Split (const std::string& string, const char delimiter)
		{
			auto tokens = std::vector<std::string> { };

			auto stream = std::stringstream { string };
			auto token  = std::string { };

			while (std::getline(stream, token, delimiter))
			{
				tokens.push_back(token);
			}

			return tokens;
		}

		template <typename Type>
		const Type& ReinterpretBytes (const std::vector<std::byte>& bytes)
		{
			return *static_cast<const Type*>(static_cast<const void*>(bytes.data()));
		}
	}

	cl_device_type
	Device::InfoResult<CL_DEVICE_TYPE>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		assert(infoBytes.size() == sizeof(cl_device_type));

		return ReinterpretBytes<cl_device_type>(infoBytes);
	}

	Device::Profile
	Device::InfoResult<CL_DEVICE_PROFILE>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto profile = StringFromBytes(infoBytes);

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
	Device::InfoResult<CL_DEVICE_VERSION>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = StringFromBytes(infoBytes);

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
	Device::InfoResult<CL_DEVICE_VENDOR>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto vendorString = StringFromBytes(infoBytes);

		return vendorString;
	}

	std::string
	Device::InfoResult<CL_DEVICE_NAME>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto name = StringFromBytes(infoBytes);

		return name;
	}

	std::vector<std::string>
	Device::InfoResult<CL_DEVICE_EXTENSIONS>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto extensionsString = StringFromBytes(infoBytes);

		const auto extensions = Split(extensionsString, ' ');

		return extensions;
	}

	cl_uint
	Device::InfoResult<CL_DEVICE_VENDOR_ID>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		assert(infoBytes.size() == sizeof(cl_uint));

		return ReinterpretBytes<cl_uint>(infoBytes);
	}

	Device::Version
	Device::InfoResult<CL_DRIVER_VERSION>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = StringFromBytes(infoBytes);

		auto version = Device::Version { };

		const auto argsParsedCount = std::sscanf
		(
			versionString.c_str(), "%u.%u", &version.major, &version.minor
		);
		assert(argsParsedCount == 2);

		return version;
	}
}
