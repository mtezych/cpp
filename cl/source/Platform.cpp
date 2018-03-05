
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
	Platform::InfoResult<CL_PLATFORM_PROFILE>::FromBytes
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

	Platform::Version
	Platform::InfoResult<CL_PLATFORM_VERSION>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto versionString = StringFromBytes(infoBytes);

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
	Platform::InfoResult<CL_PLATFORM_NAME>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return StringFromBytes(infoBytes);
	}

	std::string
	Platform::InfoResult<CL_PLATFORM_VENDOR>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return StringFromBytes(infoBytes);
	}

	std::vector<std::string>
	Platform::InfoResult<CL_PLATFORM_EXTENSIONS>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		const auto extensionsString = StringFromBytes(infoBytes);

		const auto extensions = Split(extensionsString, ' ');

		return extensions;
	}
}
