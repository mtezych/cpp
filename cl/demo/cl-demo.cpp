
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
#include <cl/Kernel.h>
#include <cl/Memory.h>

#include <iostream>

//
//                        [ Platform & Memory Models ]
//
// ┌─────────────────────────────────────────────────────────────────────┐
// │                              Host Memory                            │
// └──────────────────────────────────┰──────────────────────────────────┘
// ┌──────────────────────────────────┸──────────────────────────────────┐
// │                                 Host                                │
// └──────────────────────────────────┰──────────────────────────────────┘
//                  ┏━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━┓
// ┌────────────────┸──────────────┐     ┌────────────────┸──────────────┐
// │          OpenCL Device        │     │          OpenCL Device        │
// │    ┌────────────────────────┐ │     │    ┌────────────────────────┐ │
// │    │     Compute Unit       │ │     │    │     Compute Unit       │ │
// │ ┌──┴─────────────────────┐  │ │     │ ┌──┴─────────────────────┐  │ │
// │ │      Compute Unit      │  │ │     │ │      Compute Unit      │  │ │
// │ │ ┌────────────────────┐ │  │ │     │ │ ┌────────────────────┐ │  │ │
// │ │ │ Processing Element │ │  │ │     │ │ │ Processing Element │ │  │ │
// │ │ └──────────┰─────────┘ │  │ │     │ │ └──────────┰─────────┘ │  │ │
// │ │ ┌──────────┸─────────┐ │  │ │     │ │ ┌──────────┸─────────┐ │  │ │
// │ │ │   Private Memory   │ │  │ │     │ │ │   Private Memory   │ │  │ │
// │ │ └────────────────────┘ │  │ │ ... │ │ └────────────────────┘ │  │ │
// │ │           ...          │  │ │     │ │           ...          │  │ │
// │ │ ┌────────────────────┐ │  │ │     │ │ ┌────────────────────┐ │  │ │
// │ │ │ Processing Element │ │  │ │     │ │ │ Processing Element │ │  │ │
// │ │ └──────────┰─────────┘ │  │ │     │ │ └──────────┰─────────┘ │  │ │
// │ │ ┌──────────┸─────────┐ │  │ │     │ │ ┌──────────┸─────────┐ │  │ │
// │ │ │   Private Memory   │ ├──┘ │     │ │ │   Private Memory   │ ├──┘ │
// │ │ └────────────────────┘ ├──┐ │     │ │ └────────────────────┘ ├──┐ │
// │ └────────────┰───────────┘  │ │     │ └────────────┰───────────┘  │ │
// │ ┌────────────┸───────────┐──┘ │     │ ┌────────────┸───────────┐──┘ │
// │ │       Local Memory     │    │     │ │       Local Memory     │    │
// │ └────────────────────────┘    │     │ └────────────────────────┘    │
// └──────────────┰────────────────┘     └──────────────┰────────────────┘
// ┌──────────────┸────────────────┐     ┌──────────────┸────────────────┐
// │   Global / Constant  Memory   │     │   Global / Constant  Memory   │
// └───────────────────────────────┘     └───────────────────────────────┘
//
//                  [ Execution Model ]
//
// ┌───────────────┐ ┌───────────────┐     ┌───────────────┐
// │   Work Group  │ │   Work Group  │     │   Work Group  │
// │ ┌───────────┐ │ │ ┌───────────┐ │     │ ┌───────────┐ │
// │ │ Work Item │ │ │ │ Work Item │ │     │ │ Work Item │ │
// │ └───────────┘ │ │ └───────────┘ │     │ └───────────┘ │
// │ ┌───────────┐ │ │ ┌───────────┐ │     │ ┌───────────┐ │
// │ │ Work Item │ │ │ │ Work Item │ │ ... │ │ Work Item │ │
// │ └───────────┘ │ │ └───────────┘ │     │ └───────────┘ │
// │      ...      │ │      ...      │     │      ...      │
// │ ┌───────────┐ │ │ ┌───────────┐ │     │ ┌───────────┐ │
// │ │ Work Item │ │ │ │ Work Item │ │     │ │ Work Item │ │
// │ └───────────┘ │ │ └───────────┘ │     │ └───────────┘ │
// └───────────────┘ └───────────────┘     └───────────────┘
//              ┌───────────────────┬───────────────────────┐
//              │     OpenCL C      │         GLSL          │
// ┌────────────┼───────────────────┼───────────────────────┤
// │            │ get_work_dim()    │                       │
// │  NDRange   │ get_num_groups()  │ gl_NumWorkGroups      │
// │            │ get_local_size()  │ gl_WorkGroupSize      │
// │            │ get_global_size() │                       │
// ├────────────┼───────────────────┼───────────────────────┤
// │ Work Group │ get_group_id()    │ gl_WorkGroupID        │
// ├────────────┼───────────────────┼───────────────────────┤
// │            │ get_local_id()    │ gl_LocalInvocationID  │
// │ Work  Item │ get_global_id()   │ gl_GlobalInvocationID │
// └────────────┴───────────────────┴───────────────────────┘
//
//     get_global_size() = get_num_groups() * get_local_size()
//
//       get_global_id() = get_group_id() * get_local_size() + get_local_id()
// gl_GlobalInvocationID = gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID
//

template <typename value_type, std::size_t size>
std::ostream&
operator << (std::ostream& ostream, const std::array<value_type, size>& array)
{
	ostream << "[ ";

	for (const auto element : array)
	{
		ostream << element << ' ';
	}

	ostream << ']';

	return ostream;
}

void GetInfo (const cl::Platform& platform)
{
	const auto profile    = platform.GetInfo<cl::Platform::Info::Profile   >();
	const auto version    = platform.GetInfo<cl::Platform::Info::Version   >();
	const auto vendor     = platform.GetInfo<cl::Platform::Info::Vendor    >();
	const auto name       = platform.GetInfo<cl::Platform::Info::Name      >();
	const auto extensions = platform.GetInfo<cl::Platform::Info::Extensions>();
}

void GetInfo (const cl::Device& device)
{
	const auto type            = device.GetInfo<cl::Device::Info::Type           >();
	const auto profile         = device.GetInfo<cl::Device::Info::Profile        >();
	const auto version         = device.GetInfo<cl::Device::Info::Version        >();
	const auto vendor          = device.GetInfo<cl::Device::Info::Vendor         >();
	const auto name            = device.GetInfo<cl::Device::Info::Name           >();
	const auto extensions      = device.GetInfo<cl::Device::Info::Extensions     >();
	const auto vendorID        = device.GetInfo<cl::Device::Info::VendorID       >();
	const auto driverVersion   = device.GetInfo<cl::Device::Info::DriverVersion  >();
	const auto maxComputeUnits = device.GetInfo<cl::Device::Info::MaxComputeUnits>();
}

void GetInfo (const cl::Context& context)
{
	const auto numDevices = context.GetInfo<cl::Context::Info::NumDevices    >();
	const auto refCount   = context.GetInfo<cl::Context::Info::ReferenceCount>();
}

void GetInfo (const cl::CommandQueue& commandQueue)
{
	const auto properties = commandQueue.GetInfo<cl::CommandQueue::Info::Properties    >();
	const auto refCount   = commandQueue.GetInfo<cl::CommandQueue::Info::ReferenceCount>();
}

void Compute (const cl::Context& context, cl::CommandQueue& commandQueue)
{
	auto program = cl::Program
	{
		context,
		"kernel void cl_main                                                  \n"
		"(                                                                    \n"
		"    global const float* const  firstInput,                           \n"
		"    global const float* const secondInput,                           \n"
		"    global       float* const      output                            \n"
		")                                                                    \n"
		"{                                                                    \n"
		"    int globalID = get_global_id(0);                                 \n"
		"                                                                     \n"
		"    output[globalID] = firstInput[globalID] + secondInput[globalID]; \n"
		"}                                                                    \n"
	};
	program.Build();

	auto kernel = cl::Kernel { program, "cl_main" };

	const auto firstSrcMemory = cl::Memory<float>
	{
		context,
		cl::DeviceAccess::ReadOnly,
		cl::HostAccess::ReadWrite,
		cl::Alloc::Device,
		{
			1.5f, 7.6f, 8.1f, 1.5f, 1.7f, 2.4f, 5.0f, 3.5f,
		},
	};

	const auto secondSrcMemory = cl::Memory<float>
	{
		context,
		cl::DeviceAccess::ReadOnly,
		cl::HostAccess::ReadWrite,
		cl::Alloc::Device,
		{
			3.6f, 1.0f, 0.8f, 4.3f, 2.7f, 6.5f, 2.8f, 1.4f,
		},
	};

	const auto dstMemory = cl::Memory<float>
	{
		context,
		cl::DeviceAccess::WriteOnly,
		cl::HostAccess::ReadWrite,
		cl::Alloc::Device,
		8,
	};

	kernel.SetArg(0,  firstSrcMemory);
	kernel.SetArg(1, secondSrcMemory);
	kernel.SetArg(2,       dstMemory);

	const auto signalKernelEvent = commandQueue.EnqueueKernel<cl::Range1D>
	(
		kernel,
		{ 8 }, { 1 },
		std::vector<cl::Event> { }
	);

	auto buffer = std::array<float, 8> { };

	const auto signalReadBufferEvent = commandQueue.EnqueueReadBuffer
	(
		dstMemory,
		gsl::make_span(buffer),
		cl::Range { 0, 8 },
		std::vector<cl::Event> { }
	);

	std::cout << buffer << std::endl;
}

int main ()
{
	const auto platforms = cl::GetPlatforms();

	for (const auto& platform : platforms)
	{
		GetInfo(platform);

		const auto devices = platform.GetDevices();

		for (const auto& device : devices)
		{
			GetInfo(device);

			const auto context = cl::Context { platform, { &device, 1 } };

			GetInfo(context);

			auto commandQueue = cl::CommandQueue { context, device };

			GetInfo(commandQueue);

			Compute(context, commandQueue);
		}
	}

	return 0;
}
