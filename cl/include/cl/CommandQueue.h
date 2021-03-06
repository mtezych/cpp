
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

#ifndef CL_COMMAND_QUEUE
#define CL_COMMAND_QUEUE

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

#include <cl/Kernel.h>
#include <cl/Event.h>
#include <cl/Memory.h>

#include <util/util.h>

#include <gsl/span>

#include <array>
#include <vector>
#include <cstddef>

namespace cl
{
	struct Context;
	struct Device;

	enum NDRange : cl_uint
	{
		Range1D = 1,
		Range2D = 2,
		Range3D = 3,
	};

	struct Range
	{
		std::size_t offset;
		std::size_t size;
	};

	enum class ExecMode : cl_command_queue_properties
	{
		   InOrder = 0,
		OutOfOrder = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
	};

	enum class Profiling : cl_command_queue_properties
	{
		 Enable = CL_QUEUE_PROFILING_ENABLE,
		Disable = 0,
	};

	struct CommandQueue
	{
		cl_command_queue clCommandQueue;

		CommandQueue
		(
			const Context& context, const Device& device,
			ExecMode  execMode  = ExecMode::InOrder,
			Profiling profiling = Profiling::Disable
		);

		~CommandQueue ();

		CommandQueue (CommandQueue&& commandQueue);
		CommandQueue (const CommandQueue& commandQueue) = delete;

		CommandQueue& operator = (CommandQueue&& commandQueue);
		CommandQueue& operator = (const CommandQueue& commandQueue) = delete;

		struct Properties
		{
			ExecMode  execMode;
			Profiling profiling;
		};

		enum class Info : cl_command_queue_info
		{
			ReferenceCount = CL_QUEUE_REFERENCE_COUNT,
			Properties     = CL_QUEUE_PROPERTIES,
		};

		template <Info info>
		auto GetInfo() const
		{
			auto infoSize = std::size_t { 0 };
			auto result = clGetCommandQueueInfo
			(
				clCommandQueue, util::enum_cast(info),
				0, nullptr, &infoSize
			);
			assert(result == CL_SUCCESS);

			auto infoBytes = std::vector<std::byte>
			{
				infoSize, std::byte { 0x00 }
			};
			result = clGetCommandQueueInfo
			(
				clCommandQueue, util::enum_cast(info),
				infoBytes.size(), infoBytes.data(), nullptr
			);
			assert(result == CL_SUCCESS);

			return InfoResult<info>::FromBytes(infoBytes);
		}

		template <NDRange WorkDimension>
		Event EnqueueKernel
		(
			const Kernel& kernel,
			const std::array<size_t, WorkDimension>& globalWorkSize,
			const std::array<size_t, WorkDimension>&  localWorkSize,
			const std::vector<Event>& waitEvents
		)
		{
			static_assert(sizeof(Event) == sizeof(cl_event));

			const auto globalWorkOffset = std::array<size_t, WorkDimension> { };

			auto signalEvent = cl_event { };

			auto result = clEnqueueNDRangeKernel
			(
				clCommandQueue, kernel.clKernel,
				WorkDimension,
				globalWorkOffset.data(),
				globalWorkSize.data(), localWorkSize.data(),
				static_cast<cl_uint>(waitEvents.size()),
				reinterpret_cast<const cl_event*>(util::data_or_null(waitEvents)),
				&signalEvent
			);
			assert(result == CL_SUCCESS);

			return Event { signalEvent };
		}

		template <typename ValueType>
		Event EnqueueReadBuffer
		(
			const Memory<ValueType>& memory,
			const gsl::span<ValueType> buffer,
			const Range& range,
			const std::vector<Event>& waitEvents
		)
		{
			assert(buffer.size() >= static_cast<std::ptrdiff_t>(range.size));

			auto signalEvent = cl_event { };

			auto result = clEnqueueReadBuffer
			(
				clCommandQueue, memory.clMemory,
				cl_bool { CL_BLOCKING },
				range.offset, sizeof(ValueType) * range.size,
				buffer.data(),
				static_cast<cl_uint>(waitEvents.size()),
				reinterpret_cast<const cl_event*>(util::data_or_null(waitEvents)),
				&signalEvent
			);
			assert(result == CL_SUCCESS);

			return Event { signalEvent };
		}

	private:

		template <Info info>
		struct InfoResult;
	};

	template <>
	struct CommandQueue::InfoResult<CommandQueue::Info::ReferenceCount>
	{
		static cl_uint
		FromBytes (const std::vector<std::byte>& infoBytes);
	};

	template <>
	struct CommandQueue::InfoResult<CommandQueue::Info::Properties>
	{
		static CommandQueue::Properties
		FromBytes (const std::vector<std::byte>& infoBytes);
	};
}

#endif
