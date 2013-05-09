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

#include "../Core/IRenderer.h"
#include "../Core/Memory/CExceptionAssert.h"
#include "../Medias/CShaderProgram.h"

namespace DE
{
	CShaderProgram::CShaderProgram()
	{
		this->m_shaderProgram = nullptr;
	}

	CShaderProgram::CShaderProgram(const std::string & name)
	{
		this->m_shaderProgram = nullptr;
		LoadFromFile(name);
	}

	CShaderProgram::~CShaderProgram()
	{
		if(this->m_shaderProgram != nullptr)
			delete this->m_shaderProgram;
	}
	
	void CShaderProgram::LoadFromFile(const std::string & name)
	{
		if(this->m_shaderProgram != nullptr)
			delete this->m_shaderProgram;

		this->m_shaderProgram = IRenderer::GetRenderer()->CreateShaderProgram(name);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const bool value)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const int value)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const float value)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const bool * value, const int size)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value, size);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const int * value, const int size)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value, size);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const float * value, const int size)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, value, size);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CVector2F & vec)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, vec);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CVector3F & vec)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, vec);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CVector4F & vec)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, vec);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CColor & color)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, color);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CMatrix & mat)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, mat);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CVector4F * vec, const int size)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, vec, size);
	}

	void CShaderProgram::SetParameter(EShaderProgramFlag flags, const std::string & name, const CMatrix * mat, const int size)
	{
		Assert(this->m_shaderProgram);
		this->m_shaderProgram->SetParameter(flags, name, mat, size);
	}

	IShaderProgramBase * CShaderProgram::GetShaderProgram() const
	{
		return this->m_shaderProgram;
	}
}
