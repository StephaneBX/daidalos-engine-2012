/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#include <Windows.h>
#include <gl/GL.h>
#include "../../Core/Logger/ILogger.h"
#include "../../Renderers/OGL/COGLException.h"
#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Renderers/OGL/COGLShader.h"

namespace DE
{
	COGLShader::COGLShader(const std::string & name, const unsigned int shader) : IShaderBase(name)
	{
		this->m_shader = shader;
	}

	COGLShader::~COGLShader()
	{
		COGLRenderer::glDeleteShader(this->m_shader);
	}

	void COGLShader::SetProgramId(const unsigned int program)
	{
		this->m_associatedProgram = program;
	}

	void COGLShader::SetParameter(const std::string & name, const bool value)
	{
		CheckVariable(name);
		COGLRenderer::glUniform1i(this->m_variables[name], value);
	}

	void COGLShader::SetParameter(const std::string & name, const int value)
	{
		CheckVariable(name);
		COGLRenderer::glUniform1i(this->m_variables[name], value);
	}

	void COGLShader::SetParameter(const std::string & name, const float value)
	{
		CheckVariable(name);
		COGLRenderer::glUniform1f(this->m_variables[name], value);
	}

	void COGLShader::SetParameter(const std::string & name, const bool * value, const int size) {}

	void COGLShader::SetParameter(const std::string & name, const int * value, const int size)
	{
		CheckVariable(name);
		COGLRenderer::glUniform1iv(this->m_variables[name], size, value);
	}

	void COGLShader::SetParameter(const std::string & name, const float * value, const int size)
	{
		CheckVariable(name);
		COGLRenderer::glUniform1fv(this->m_variables[name], size, value);
	}

	void COGLShader::SetParameter(const std::string & name, const CVector2F & vec) 
	{
		CheckVariable(name);
		COGLRenderer::glUniform2fv(this->m_variables[name], 1, vec);
	}

	void COGLShader::SetParameter(const std::string & name, const CVector3F & vec) 
	{
		CheckVariable(name);
		COGLRenderer::glUniform3fv(this->m_variables[name], 1, vec);
	}

	void COGLShader::SetParameter(const std::string & name, const CVector4F & vec)
	{
		CheckVariable(name);
		COGLRenderer::glUniform4fv(this->m_variables[name], 1, vec);
	}

	void COGLShader::SetParameter(const std::string & name, const CColor & vec)
	{
		CheckVariable(name);
		COGLRenderer::glUniform4fv(this->m_variables[name], 1, vec.ToRGBAVector());
	}

	void COGLShader::SetParameter(const std::string & name, const CMatrix & mat)
	{
		CheckVariable(name);
		COGLRenderer::glUniformMatrix4fv(this->m_variables[name], 1, false, mat);
	}

	void COGLShader::SetParameter(const std::string & name, const CVector4F * vec, const int size)
	{
		CheckVariable(name);
		COGLRenderer::glUniform4fv(this->m_variables[name], size, reinterpret_cast<const float*>(&vec));
	}

	void COGLShader::SetParameter(const std::string & name, const CMatrix * mat, const int size)
	{
		CheckVariable(name);
		COGLRenderer::glUniformMatrix4fv(this->m_variables[name], 1, false, reinterpret_cast<const float*>(&mat));
	}

	void COGLShader::ClearVariables()
	{
		this->m_variables.clear();
	}

	const unsigned int COGLShader::GetShader() const
	{
		return this->m_shader;
	}

	void COGLShader::CheckVariable(const std::string & name)
	{
		if(this->m_variables.find(name) == this->m_variables.end())
		{
			const COGLRenderer * renderer = static_cast<COGLRenderer*>(IRenderer::GetRenderer());
			this->m_variables[name] = COGLRenderer::glGetUniformLocation(this->m_associatedProgram, name.c_str());

			// Check for error.
			if(this->m_variables[name] == -1)
			{
				// Log the error.
				ILogger::Log() << "CE:COGLShader Warning: Unable to get the uniform location '" << name << "' for the shader " << this->m_name.GetFilename() << ".\n";
			}
		}
	}
}
