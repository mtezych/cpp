
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

#include <gles/Program.h>

#include <gles/Shader.h>

#include <cassert>

namespace gles
{
	Program::Program(const Shader& vertexShader, const Shader& fragmentShader)
	:
		glProgram { glCreateProgram() }
	{
		assert(glProgram != 0);
		assert(glGetError() == GL_NO_ERROR);

		assert(  vertexShader.Type() == GL_VERTEX_SHADER);
		assert(fragmentShader.Type() == GL_FRAGMENT_SHADER);

		glAttachShader(glProgram, vertexShader.glShader);
		assert(glGetError() == GL_NO_ERROR);
		glAttachShader(glProgram, fragmentShader.glShader);
		assert(glGetError() == GL_NO_ERROR);

		glLinkProgram(glProgram);
		assert(glGetError() == GL_NO_ERROR);

		auto linkStatus = GLint { GL_FALSE };
		glGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus);
		assert(linkStatus == GL_TRUE);
		assert(glGetError() == GL_NO_ERROR);
	}

	Program::~Program()
	{
		auto currentProgram = GLint { };
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		assert(glGetError() == GL_NO_ERROR);

		if (glProgram == static_cast<GLuint>(currentProgram))
		{
			glUseProgram(0);
		}

		if (glProgram != 0)
		{
			glDeleteProgram(glProgram);
			assert(glGetError() == GL_NO_ERROR);
		}

		// Note that deleting a program also implicitly detaches all shaders.
	}

	Program::Program (Program&& program)
	:
		glProgram { program.glProgram }
	{
		program.glProgram = 0;
	}

	Program& Program::operator = (Program&& program)
	{
		auto currentProgram = GLint { };
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		assert(glGetError() == GL_NO_ERROR);

		if (glProgram == static_cast<GLuint>(currentProgram))
		{
			glUseProgram(0);
		}

		if (glProgram != 0)
		{
			glDeleteProgram(glProgram);
			assert(glGetError() == GL_NO_ERROR);
		}

		// Note that deleting a program also implicitly detaches all shaders.

		glProgram = program.glProgram;
		program.glProgram = 0;

		return *this;
	}

	void Program::Use()
	{
		glUseProgram(glProgram);
		assert(glGetError() == GL_NO_ERROR);
	}
}
