
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

#include <gles/Shader.h>

#include <cassert>

namespace gles
{
	Shader::Shader (const GLenum type, const std::string& sourceCode)
	:
		glShader { glCreateShader(type) }
	{
		assert
		(
			type == GL_VERTEX_SHADER          ||
			type == GL_TESS_CONTROL_SHADER    ||
			type == GL_TESS_EVALUATION_SHADER ||
			type == GL_GEOMETRY_SHADER        ||
			type == GL_FRAGMENT_SHADER        ||
			type == GL_COMPUTE_SHADER
		);

		assert(glShader != 0);
		assert(glGetError() == GL_NO_ERROR);

		const auto sourceCodeLine = sourceCode.c_str();
		glShaderSource(glShader, 1, &sourceCodeLine, nullptr);
		assert(glGetError() == GL_NO_ERROR);

		glCompileShader(glShader);
		assert(glGetError() == GL_NO_ERROR);

		auto compileStatus = GLint { GL_FALSE };
		glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileStatus);
		assert(compileStatus == GL_TRUE);
		assert(glGetError() == GL_NO_ERROR);
	}

	Shader::~Shader ()
	{
		if (glShader != 0)
		{
			glDeleteShader(glShader);
			assert(glGetError() == GL_NO_ERROR);
		}
	}

	Shader::Shader (Shader&& shader)
	:
		glShader { shader.glShader }
	{
		shader.glShader = 0;
	}

	Shader& Shader::operator = (Shader&& shader)
	{
		if (glShader != 0)
		{
			glDeleteShader(glShader);
			assert(glGetError() == GL_NO_ERROR);
		}

		glShader = shader.glShader;
		shader.glShader = 0;

		return *this;
	}

	GLenum Shader::Type () const
	{
		auto type = GLint { };

		glGetShaderiv(glShader, GL_SHADER_TYPE, &type);
		assert(glGetError() == GL_NO_ERROR);

		return static_cast<GLenum>(type);
	}
}
