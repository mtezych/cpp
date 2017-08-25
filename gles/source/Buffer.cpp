
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
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

#include <gles/Buffer.h>

#include <cassert>

namespace gles
{
	Buffer::Buffer()
	{
		glGenBuffers(1, &glBuffer);
		assert(glBuffer != 0);
		assert(glGetError() == GL_NO_ERROR);
	}

	Buffer::~Buffer()
	{
		if (glBuffer != 0)
		{
			glDeleteBuffers(1, &glBuffer);
			assert(glGetError() == GL_NO_ERROR);
		}
	}

	Buffer::Buffer(Buffer&& buffer)
	:
		glBuffer { buffer.glBuffer }
	{
		buffer.glBuffer = 0;
	}

	Buffer& Buffer::operator = (Buffer&& buffer)
	{
		if (glBuffer != 0)
		{
			glDeleteBuffers(1, &glBuffer);
			assert(glGetError() == GL_NO_ERROR);
		}

		glBuffer = buffer.glBuffer;
		buffer.glBuffer = 0;

		return *this;
	}

	void Buffer::Bind(const GLenum target)
	{
		assert
		(
			target == GL_ARRAY_BUFFER              ||
			target == GL_ELEMENT_ARRAY_BUFFER      ||
			target == GL_PIXEL_PACK_BUFFER         ||
			target == GL_PIXEL_UNPACK_BUFFER       ||
			target == GL_UNIFORM_BUFFER            ||
			target == GL_SHADER_STORAGE_BUFFER     ||
			target == GL_TRANSFORM_FEEDBACK_BUFFER
		);
		glBindBuffer(target, glBuffer);
		assert(glGetError() == GL_NO_ERROR);
	}
}
