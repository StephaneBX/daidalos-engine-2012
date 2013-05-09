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

#include "../../Core/Logger/ILogger.h"
#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Renderers/OGL/COGLException.h"
#include "../../Renderers/OGL/COGLShader.h"
#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Renderers/OGL/COGLShaderProgram.h"

namespace DE
{
	COGLShaderProgram::COGLShaderProgram(const std::string & name) : IShaderProgramBase(name)
	{
		SetShader(name);
	}

	void COGLShaderProgram::SetShader(const std::string & name)
	{
		if(this->m_programId > 0)
			COGLRenderer::glDeleteProgram(this->m_programId);
		
		this->m_programId = COGLRenderer::glCreateProgram();

		IShaderProgramBase::SetShader(name);
		LinkProgram();

		static_cast<COGLShader*>(this->m_vertexShader)->SetProgramId(this->m_programId);
		static_cast<COGLShader*>(this->m_pixelShader)->SetProgramId(this->m_programId);
	}

	const unsigned int COGLShaderProgram::GetProgramId() const
	{
		return this->m_programId;
	}

	int * COGLShaderProgram::GetSamplers()
	{
		return &this->m_samplers[0];
	}

	void COGLShaderProgram::LinkProgram()
	{
		Assert(this->m_vertexShader && this->m_pixelShader);

		std::ostringstream ss;

		// Attach the shaders
		COGLRenderer::glAttachShader(this->m_programId, static_cast<COGLShader*>(this->m_vertexShader)->GetShader());
		COGLRenderer::glAttachShader(this->m_programId, static_cast<COGLShader*>(this->m_pixelShader)->GetShader());

		// Link the program.
		COGLRenderer::glLinkProgram(this->m_programId);
		CheckProgramStatus(GL_LINK_STATUS, "glLinkProgram");

		// Validate the program.
		COGLRenderer::glValidateProgram(this->m_programId);
		CheckProgramStatus(GL_VALIDATE_STATUS, "glUseProgram");

		// Get the texture samplers
		for(int i = 0 ; i < MAXIMUM_TEXTURE_UNIT ; ++i)
		{
			ss.str("");
			ss << "textureSampler" << i;
			this->m_samplers[i] = COGLRenderer::glGetUniformLocation(this->m_programId, ss.str().c_str());
		}
	}

	void COGLShaderProgram::CheckProgramStatus(const int type, const std::string & oglFunction)
	{
		GLint status = GL_TRUE;
		COGLRenderer::glGetProgramiv(this->m_programId, type, &status);

		if(status != GL_TRUE)
		{
			// Link error.
			int logsize;
			COGLRenderer::glGetProgramiv(this->m_programId, GL_INFO_LOG_LENGTH, &logsize);

			// Get the error.
			char * log = new char[logsize + 1];
			memset(log, '\0', logsize + 1);
			COGLRenderer::glGetProgramInfoLog(this->m_programId, logsize, &logsize, log);

			// Log the error.
			ILogger::Log() << "DE:COGLShaderProgram Error: " << log << ".\n";
			ILogger::Kill();

			// Free the the log buffer.
			delete[] log;
				
			// Exception.
			throw COGLException("CheckProgramStatus", oglFunction);
		}
	}
}
