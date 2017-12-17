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
#include "../Medias/CStaticMesh.hpp"

namespace DE {
	CStaticMesh::CStaticMesh() {
		LOG_TRACE();

		this->m_mesh = nullptr;
	}

	CStaticMesh::CStaticMesh(const std::string & filename) {
		LOG_TRACE();

		this->m_mesh = nullptr;
		LoadFromFile(filename);
	}

	CStaticMesh::CStaticMesh(const CStaticMesh & copy) {
		LOG_TRACE();

		*this = copy;
	}

	CStaticMesh::~CStaticMesh() {
		LOG_TRACE();

		if(this->m_mesh != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_mesh->GetName());
	}

	void CStaticMesh::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		// Release previously used resource
		if(this->m_mesh != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_mesh->GetName());

		// Check the existence of the resource and load it if necessary
		IMeshBase * tmp = CResourceManager::GetInstance()->GetResource<IMeshBase>(filename);

		if(tmp == nullptr) {
			this->m_mesh = CMediaManager::GetInstance()->LoadMediaFromFile<IMeshBase>(filename);
			CResourceManager::GetInstance()->AddResource(filename, this->m_mesh);
		} else
			this->m_mesh = tmp;
	}

	void CStaticMesh::FinalizeGeometry(EMeshShadingType shadingType) {
		LOG_TRACE();

		this->m_mesh->FinalizeGeometry(shadingType);
	}

	const CStaticMesh & CStaticMesh::operator=(const CStaticMesh & copy) {
		LOG_TRACE();

		if (this != &copy) {
			if (this->m_mesh != nullptr) {
				CResourceManager::GetInstance()->ReleaseResource(this->m_mesh->GetName());
			}

			this->m_mesh = copy.GetMesh();

			if (this->m_mesh != nullptr) {
				this->m_mesh->AddReference();
			}
		}

		return *this;
	}

	IMeshBase * CStaticMesh::GetMesh() const {
		return this->m_mesh;
	}
}
