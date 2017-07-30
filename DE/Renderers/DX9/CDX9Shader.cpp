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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Renderers/DX9/DX9Global.hpp"
#include "../../Renderers/DX9/CDX9Renderer.hpp"
#include "../../Renderers/DX9/CDX9Shader.hpp"

namespace DE {
	CDX9Shader::CDX9Shader(const std::string & name, ID3DXConstantTable * constantTable) : IShaderBase(name) {
		LOG_TRACE();

		this->m_constantTable = constantTable;
	}

	CDX9Shader::~CDX9Shader() {
		LOG_TRACE();

		SAFE_RELEASE(this->m_constantTable);
	}

	void CDX9Shader::SetParameter(const std::string & name, const bool value) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetBool(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], value);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}

	}

	void CDX9Shader::SetParameter(const std::string & name, const int value) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetInt(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], value);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const float value) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetFloat(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], value);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const bool * value, const int size) {
		LOG_TRACE();
	}

	void CDX9Shader::SetParameter(const std::string & name, const int * value, const int size) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetIntArray(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], value, size);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const float * value, const int size) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetFloatArray(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], value, size);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CVector2F & vec) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			CVector4F vec2(vec.x, vec.y, 0, 0);
			this->m_constantTable->SetVector(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(&vec2));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CVector2I & vec) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			CVector4F vec2((float)vec.x, (float)vec.y, 0, 0);
			this->m_constantTable->SetVector(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(&vec2));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CVector3F & vec) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			CVector4F vec2(vec.x, vec.y, vec.z, 0);
			this->m_constantTable->SetVector(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(&vec2));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CVector4F & vec) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetVector(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(&vec));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CColor & color) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			CVector4F vec = color.ToRGBAVector();
			this->m_constantTable->SetVector(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(&vec));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CMatrix & mat) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetMatrix(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXMATRIX*>(&mat));
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CVector4F * vec, const int size) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetVectorArray(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXVECTOR4*>(vec), size);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::SetParameter(const std::string & name, const CMatrix * mat, const int size) {
		LOG_TRACE();

		CheckHandle(name);
		if(this->m_handles[name] != nullptr) {
			this->m_constantTable->SetMatrixArray(static_cast<CDX9Renderer*>(IRenderer::GetRenderer())->GetDevice(), this->m_handles[name], reinterpret_cast<const D3DXMATRIX*>(mat), size);
		} else {
			LOG_WARNING("Shader attribute '" << name << "' not found in '" << this->m_name.GetFilename() << "'");
		}
	}

	void CDX9Shader::CheckHandle(const std::string & name) {
		LOG_TRACE();

		if(this->m_handles.find(name) == this->m_handles.end())
			this->m_handles[name] = this->m_constantTable->GetConstantByName(nullptr, name.c_str());
	}
}
