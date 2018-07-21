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

#include <algorithm>
#include "../Core/Core.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Medias/COBJMesh.hpp"

namespace daidalosengine {
	COBJMesh::COBJMesh(const std::string & name, SOBJInternalData * data) : IMeshBase(name) {
		LOG_TRACE();

		// Data initialization
		this->m_internalData = data;
		InitializeBuffers(this->m_internalData->groupSize.size());

		for(unsigned int i = 0; i < this->m_internalData->groupSize.size(); ++i) {
			InitializeBufferGroup(this->m_internalData->groupSize[i]);

			memcpy(GetVertices(i), this->m_internalData->vertex[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
			memcpy(GetTexcoords(i), this->m_internalData->texcoord[i], sizeof(CVector2F) * this->m_internalData->groupSize[i]);

			if(this->m_internalData->normal[i] == nullptr) {
				// If no normals are present, calculate them (flat)
				FlatShadingNormals();
			} else {
				memcpy(GetNormals(i), this->m_internalData->normal[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
			}
		}

		// Visibilty shape calculation
		CVector3F min;
		CVector3F max;
		for(auto i = 0U; i < this->m_internalData->groupSize.size(); ++i) {
			for(auto j = 0; j < this->m_internalData->groupSize[i]; ++j) {
				max.x = std::max(max.x, GetVertices(i)[j].x);
				max.y = std::max(max.y, GetVertices(i)[j].y);
				max.z = std::max(max.z, GetVertices(i)[j].z);

				min.x = std::min(min.x, GetVertices(i)[j].x);
				min.y = std::min(min.y, GetVertices(i)[j].y);
				min.z = std::min(min.z, GetVertices(i)[j].z);
			}
		}

		// Find the position
		GetVisibilityShape()->SetPosition((max + min) / 2.0F);

		// Find the radius
		GetVisibilityShape()->SetRadius((max - ((max + min) / 2.0F)).Length());

		TangentAndBinormalCalculation();
	}

	COBJMesh::~COBJMesh() {
		LOG_TRACE();

		for(unsigned int i = 0; i < this->m_internalData->groupSize.size(); ++i) {
			if (this->m_internalData->vertex != nullptr) {
				SAFE_DELETE_ARRAY(this->m_internalData->vertex[i]);
			}

			if (this->m_internalData->normal != nullptr) {
				SAFE_DELETE_ARRAY(this->m_internalData->normal[i]);
			}

			if (this->m_internalData->texcoord != nullptr) {
				SAFE_DELETE_ARRAY(this->m_internalData->texcoord[i]);
			}
		}

		SAFE_DELETE_ARRAY(this->m_internalData->vertex);
		SAFE_DELETE_ARRAY(this->m_internalData->normal);
		SAFE_DELETE_ARRAY(this->m_internalData->texcoord);
		SAFE_DELETE(this->m_internalData);
	}

	void COBJMesh::FinalizeGeometry(EMeshShadingType shadingType) {
		LOG_TRACE();

		switch(shadingType) {
		case MESH_SHADING_FLAT:
			FlatShadingNormals();
			break;

		case MESH_SHADING_SMOOTH:
			// Use the declared normal if possible
			if(this->m_internalData->normal != nullptr) {
				for(auto i = 0U; i < this->m_internalData->groupSize.size(); ++i) {
					memcpy(GetNormals(i), this->m_internalData->normal[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
				}
			} else {
				// If no normal, use flat shading
				FlatShadingNormals();
			}
			
			break;

		default:
			FlatShadingNormals();
			break;
		}
	}
}
