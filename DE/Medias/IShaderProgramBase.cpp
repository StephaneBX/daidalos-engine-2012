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

#include "../Core/CResourceManager.hpp"
#include "../Core/CMediaManager.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Core/Memory/CExceptionAssert.hpp"
#include "../Medias/IShaderProgramBase.hpp"

namespace daidalosengine {
	IShaderProgramBase::IShaderProgramBase(const std::string & name) : IResource(name) {
		LOG_TRACE();

		this->m_vertexShader = nullptr;
		this->m_pixelShader = nullptr;

		SetShader(name);
	}

	IShaderProgramBase::~IShaderProgramBase() {
		LOG_TRACE();

		if (this->m_vertexShader != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(this->m_vertexShader->GetName());
		}

		if (this->m_pixelShader != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(this->m_pixelShader->GetName());
		}
	}

	void IShaderProgramBase::SetShader(const std::string & name) {
		LOG_TRACE();

		this->m_vertexShader = nullptr;
		this->m_pixelShader = nullptr;

		Load(this->m_vertexShader, name + ".vsh");
		Load(this->m_pixelShader, name + ".psh");
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const bool value) {

		LOG_TRACE();
		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const int value) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const float value) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const bool * value, const int size) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value, size);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value, size);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const int * value, const int size) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value, size);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value, size);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const float * value, const int size) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, value, size);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, value, size);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector2F & vec) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, vec);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, vec);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector2I & vec) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, vec);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, vec);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector3F & vec) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, vec);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, vec);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector4F & vec) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, vec);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, vec);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CColor & color) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, color);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, color);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CMatrix & mat) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, mat);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, mat);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector4F * vec, const int size) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, vec, size);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, vec, size);
		}
	}

	void IShaderProgramBase::SetParameter(const EShaderProgramFlag flags, const std::string & name, const CMatrix * mat, const int size) {
		LOG_TRACE();

		if((flags & SHADERPROGRAM_FLAG_VERTEX) != 0) {
			Assert(this->m_vertexShader);
			this->m_vertexShader->SetParameter(name, mat, size);
		}

		if((flags & SHADERPROGRAM_FLAG_PIXEL) != 0) {
			Assert(this->m_pixelShader);
			this->m_pixelShader->SetParameter(name, mat, size);
		}
	}

	IShaderBase * IShaderProgramBase::GetVertexShader() const {
		LOG_TRACE();

		return this->m_vertexShader;
	}

	IShaderBase * IShaderProgramBase::GetPixelShader() const {
		LOG_TRACE();

		return this->m_pixelShader;
	}

	void IShaderProgramBase::Load(IShaderBase *& shader, const std::string & name) {
		LOG_TRACE();

		// Release previously used resource
		if (shader != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(shader->GetName());
		}

		// Check the existence of the resource and load it if necessary
		const auto tmp = CResourceManager::GetInstance()->GetResource<IShaderBase>(name);

		if(tmp == nullptr) {
			shader = CMediaManager::GetInstance()->LoadMediaFromFile<IShaderBase>(name);
			CResourceManager::GetInstance()->AddResource(name, shader);
		} else
			shader = tmp;
	}
}
