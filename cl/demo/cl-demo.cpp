
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
#include <cl/Context.h>
#include <cl/CommandQueue.h>
#include <cl/Program.h>

//                                 +------+
//                                 | Host |
//                                 +------+
//                                    |
//                  +-----------------+-------------------+
//                  |                                     |
// +----------------|--------------+     +----------------|--------------+
// |          OpenCL Device        |     |          OpenCL Device        |
// |    +------------------------+ |     |    +------------------------+ |
// |    |      Compute Unit      | |     |    |      Compute Unit      | |
// | +------------------------+  | |     | +------------------------+  | |
// | |      Compute Unit      |  | |     | |      Compute Unit      |  | |
// | | +--------------------+ |  | | ... | | +--------------------+ |  | |
// | | | Processing Element | |  | |     | | | Processing Element | |  | |
// | | +--------------------+ |  | |     | | +--------------------+ |  | |
// | | +--------------------+ |  | |     | | +--------------------+ |  | |
// | | | Processing Element | |  | |     | | | Processing Element | |  | |
// | | +--------------------+ |  | |     | | +--------------------+ |  | |
// | |         ...            |  | |     | |         ...            |  | |
// | | +--------------------+ |  | |     | | +--------------------+ |  | |
// | | | Processing Element | |--+ |     | | | Processing Element | |--+ |
// | | +--------------------+ |    |     | | +--------------------+ |    |
// | +------------------------+    |     | +------------------------+    |
// +-------------------------------+     +-------------------------------+

// +---------------+ +---------------+     +---------------+
// |   Work Group  | |   Work Group  |     |   Work Group  |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// | | Work Item | | | | Work Item | |     | | Work Item | |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// | | Work Item | | | | Work Item | | ... | | Work Item | |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// |      ...      | |      ...      |     |      ...      |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// | | Work Item | | | | Work Item | |     | | Work Item | |
// | +-----------+ | | +-----------+ |     | +-----------+ |
// +---------------+ +---------------+     +---------------+

// Dispatch
// - gl_NumWorkGroups
// - gl_WorkGroupSize
// Work Group
// - gl_WorkGroupID
// Work Item
// - gl_LocalInvocationID
// - gl_GlobalInvocationID = gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID

int main ()
{
	const auto platforms = cl::GetPlatforms();

	for (const auto& platform : platforms)
	{
		const auto platformProfile    = platform.GetInfo<CL_PLATFORM_PROFILE   >();
		const auto platformVersion    = platform.GetInfo<CL_PLATFORM_VERSION   >();
		const auto platformVendor     = platform.GetInfo<CL_PLATFORM_VENDOR    >();
		const auto platformName       = platform.GetInfo<CL_PLATFORM_NAME      >();
		const auto platformExtensions = platform.GetInfo<CL_PLATFORM_EXTENSIONS>();

		const auto devices = platform.GetDevices();

		for (const auto& device : devices)
		{
			const auto deviceType       = device.GetInfo<CL_DEVICE_TYPE      >();
			const auto deviceProfile    = device.GetInfo<CL_DEVICE_PROFILE   >();
			const auto deviceVersion    = device.GetInfo<CL_DEVICE_VERSION   >();
			const auto deviceVendor     = device.GetInfo<CL_DEVICE_VENDOR    >();
			const auto deviceName       = device.GetInfo<CL_DEVICE_NAME      >();
			const auto deviceExtensions = device.GetInfo<CL_DEVICE_EXTENSIONS>();
			const auto deviceVendorID   = device.GetInfo<CL_DEVICE_VENDOR_ID >();
			const auto driverVersion    = device.GetInfo<CL_DRIVER_VERSION   >();
		}

		const auto context = cl::Context { platform, devices };

		const auto contextNumDevices     = context.GetInfo<CL_CONTEXT_NUM_DEVICES    >();
		const auto contextReferenceCount = context.GetInfo<CL_CONTEXT_REFERENCE_COUNT>();

		for (const auto& device : devices)
		{
			const auto commandQueue = cl::CommandQueue { context, device };
		}

		auto program = cl::Program
		{
			context,
			"__kernel void kernel_main                                            \n"
			"(                                                                    \n"
			"    __global const float*  firstInput,                               \n"
			"    __global const float* secondInput,                               \n"
			"    __global       float*      output                                \n"
			")                                                                    \n"
			"{                                                                    \n"
			"    int globalID = get_global_id(0);                                 \n"
			"                                                                     \n"
			"    output[globalID] = firstInput[globalID] + secondInput[globalID]; \n"
			"}                                                                    \n"
		};
		program.Build();
	}

	return 0;
}
