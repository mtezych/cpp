
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

#include <cl/CommandQueue.h>

#include <cl/Context.h>
#include <cl/Device.h>

#include <util/util.h>

namespace cl
{
	namespace
	{
		constexpr bool
		ValidateQueueProperties (const cl_command_queue_properties queueProperties)
		{
			const auto matchesAnyQueueProperty = util::IsAnyBitSet
			(
				queueProperties,
				CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
				CL_QUEUE_PROFILING_ENABLE
			);

			return matchesAnyQueueProperty || (queueProperties == 0);
		}
	}

	CommandQueue::CommandQueue
	(
		const Context& context, const Device& device,
		const ExecMode execMode, const Profiling profiling
	):
		clCommandQueue { nullptr }
	{
		const auto queueProperties = cl_command_queue_properties
		{
			util::enum_cast(execMode) | util::enum_cast(profiling)
		};
		assert(ValidateQueueProperties(queueProperties));

		auto result = cl_int { CL_INVALID_COMMAND_QUEUE };
		clCommandQueue = clCreateCommandQueue
		(
			context.clContext, device.clDeviceID,
			queueProperties,
			&result
		);
		assert(result == CL_SUCCESS);
	}

	CommandQueue::~CommandQueue ()
	{
		if (clCommandQueue != nullptr)
		{
			clReleaseCommandQueue(clCommandQueue);
		}
	}

	CommandQueue::CommandQueue (CommandQueue&& commandQueue)
	:
		clCommandQueue { commandQueue.clCommandQueue }
	{
		commandQueue.clCommandQueue = nullptr;
	}

	CommandQueue& CommandQueue::operator = (CommandQueue&& commandQueue)
	{
		if (clCommandQueue != nullptr)
		{
			clReleaseCommandQueue(clCommandQueue);
		}

		clCommandQueue = commandQueue.clCommandQueue;

		commandQueue.clCommandQueue = nullptr;

		return *this;
	}

	cl_uint
	CommandQueue::InfoResult<CommandQueue::Info::ReferenceCount>::FromBytes
	(
		const std::vector<std::byte>& infoBytes
	)
	{
		return util::ReinterpretBytes<cl_uint>(infoBytes);
	}
}
