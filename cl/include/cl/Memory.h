
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

#ifndef CL_MEMORY
#define CL_MEMORY

#include <cl/Context.h>

#include <util/util.h>

#include <vector>
#include <cassert>
#include <cstddef>

// OpenCL Basics: Flags for the creating memory objects
//     https://streamhpc.com/blog/2013-02-03/opencl-basics-flags-for-the-creating-memory-objects

namespace cl
{
	constexpr
	bool ValidateMemoryFlags (const cl_mem_flags memoryFlags)
	{
		const auto matchesAnyQueueProperty = util::IsAnyBitSet
		(
			memoryFlags,

			CL_MEM_READ_ONLY       |
			CL_MEM_WRITE_ONLY      |
			CL_MEM_READ_WRITE      |

			CL_MEM_HOST_WRITE_ONLY |
			CL_MEM_HOST_READ_ONLY  |
			CL_MEM_HOST_NO_ACCESS  |

			CL_MEM_USE_HOST_PTR    |
			CL_MEM_ALLOC_HOST_PTR  |
			CL_MEM_COPY_HOST_PTR
		);

		return matchesAnyQueueProperty || (memoryFlags == 0);
	}

	enum class DeviceAccess : cl_mem_flags
	{
		ReadWrite = CL_MEM_READ_WRITE,
		ReadOnly  = CL_MEM_READ_ONLY,
		WriteOnly = CL_MEM_WRITE_ONLY,
	};

	enum class HostAccess : cl_mem_flags
	{
		ReadWrite = 0,
		ReadOnly  = CL_MEM_HOST_WRITE_ONLY,
		WriteOnly = CL_MEM_HOST_READ_ONLY,
		NoAccess  = CL_MEM_HOST_NO_ACCESS,
	};

	enum class Alloc : cl_mem_flags
	{
		Device = 0,
		Host   = CL_MEM_ALLOC_HOST_PTR,
	};

	// @todo: Add support for CL_MEM_USE_HOST_PTR flag.

	template <typename ValueType>
	struct Memory
	{
		cl_mem clMemory;

		Memory
		(
			const Context&     context,
			const DeviceAccess deviceAccess,
			const HostAccess   hostAccess,
			const Alloc        alloc,
			const std::size_t  size
		):
			clMemory { nullptr }
		{
			const auto flags = cl_mem_flags
			{
				util::enum_cast(deviceAccess) |
				util::enum_cast(  hostAccess) |
				util::enum_cast(alloc)
			};
			assert(ValidateMemoryFlags(flags));

			auto result = cl_int { CL_INVALID_MEM_OBJECT };
			clMemory = clCreateBuffer
			(
				context.clContext,
				flags,
				sizeof(ValueType) * size, nullptr,
				&result
			);
			assert(result == CL_SUCCESS);
		}

		Memory
		(
			const Context&                context,
			const DeviceAccess            deviceAccess,
			const HostAccess              hostAccess,
			const Alloc                   alloc,
			const std::vector<ValueType>& buffer
		):
			clMemory { nullptr }
		{
			const auto flags = cl_mem_flags
			{
				util::enum_cast(deviceAccess) |
				util::enum_cast(  hostAccess) |
				util::enum_cast(alloc)        |
				CL_MEM_COPY_HOST_PTR
			};
			assert(ValidateMemoryFlags(flags));

			// @note: The OpenCL spec guarantees that the host_ptr
			//        will be used only for reading data,
			//
			//        using which memory content of
			//        OpenCL buffer will be initialized with,
			//
			//        when the CL_MEM_COPY_HOST_PTR flag is specified.
			//
			const auto host_ptr = const_cast<ValueType*>(buffer.data());

			auto result = cl_int { CL_INVALID_MEM_OBJECT };
			clMemory = clCreateBuffer
			(
				context.clContext,
				flags,
				sizeof(ValueType) * buffer.size(), host_ptr,
				&result
			);
			assert(result == CL_SUCCESS);
		}

		~Memory ()
		{
			if (clMemory != nullptr)
			{
				clReleaseMemObject(clMemory);
			}
		}

		Memory (Memory&& memory)
		:
			clMemory { memory.clMemory }
		{
			memory.clMemory = nullptr;
		}

		Memory (const Memory& memory) = delete;

		Memory& operator = (Memory&& memory)
		{
			if (clMemory != nullptr)
			{
				clReleaseMemObject(clMemory);
			}

			clMemory = memory.clMemory;

			memory.clMemory = nullptr;

			return *this;
		}

		Memory& operator = (const Memory& memory) = delete;
	};
}

#endif
