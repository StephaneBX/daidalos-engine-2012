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

#include "../Core/Core.h"
#include "../Medias/IMeshBase.h"

namespace DE {
	IMeshBase::~IMeshBase() {
		for(unsigned int i = 0; i < this->m_groupSize.size(); ++i) {
			if(this->m_vertex != nullptr)
				SAFE_DELETE_ARRAY(this->m_vertex[i]);

			if(this->m_normal != nullptr)
				SAFE_DELETE_ARRAY(this->m_normal[i]);

			if(this->m_tangent != nullptr)
				SAFE_DELETE_ARRAY(this->m_tangent[i]);

			if(this->m_binormal != nullptr)
				SAFE_DELETE_ARRAY(this->m_binormal[i]);

			if(this->m_texcoord != nullptr)
				SAFE_DELETE_ARRAY(this->m_texcoord[i]);
		}

		SAFE_DELETE_ARRAY(this->m_vertex);
		SAFE_DELETE_ARRAY(this->m_normal);
		SAFE_DELETE_ARRAY(this->m_tangent);
		SAFE_DELETE_ARRAY(this->m_binormal);
		SAFE_DELETE_ARRAY(this->m_texcoord);
		SAFE_DELETE(this->m_visibilityShape);
	}

	const int IMeshBase::GetNumberOfGroups() const {
		return this->m_groupSize.size();
	}

	const int IMeshBase::GetGroupSize(const int group) const {
		return this->m_groupSize[group];
	}

	CVector3F * IMeshBase::GetVertices(const int group) const {
		return this->m_vertex[group];
	}

	CVector3F * IMeshBase::GetNormals(const int group) const {
		return this->m_normal[group];
	}

	CVector3F * IMeshBase::GetTangents(const int group) const {
		return this->m_tangent[group];
	}

	CVector3F * IMeshBase::GetBinormals(const int group) const {
		return this->m_binormal[group];
	}

	CVector2F * IMeshBase::GetTexcoords(const int group) const {
		return this->m_texcoord[group];
	}

	CSphereF * IMeshBase::GetVisibilityShape() const {
		return this->m_visibilityShape;
	}

	IMeshBase::IMeshBase(const std::string & name) : IResource(name) {
		this->m_vertex = nullptr;
		this->m_normal = nullptr;
		this->m_texcoord = nullptr;
		this->m_visibilityShape = new CSphereF();
	}

	void IMeshBase::FlatShadingNormals() {
		// Loop through the groups
		for(unsigned int i = 0; i < this->m_groupSize.size(); ++i) {
			// Loop through the vertices
			for(int j = 0; j < this->m_groupSize[i]; j += 3) {
				// Normal calculation
				CVector3F ab = this->m_vertex[i][j + 1] - this->m_vertex[i][j];
				CVector3F ac = this->m_vertex[i][j + 2] - this->m_vertex[i][j];
				CVector3F normal = ab.CrossProduct(ac).Normalize();

				// Replace th previous normals
				std::fill(&this->m_normal[i][j], &this->m_normal[i][j] + 3, normal);
			}
		}
	}

	void IMeshBase::TangentAndBinormalCalculation() {
		CVector3F ab, ac;
		CVector2F abuv, acuv;

		for(unsigned int i = 0; i < this->m_groupSize.size(); ++i) {
			for(int j = 0; j < this->m_groupSize[i]; j += 3) {
				ab = this->m_vertex[i][j + 1] - this->m_vertex[i][j];
				ac = this->m_vertex[i][j + 2] - this->m_vertex[i][j];
				abuv = this->m_texcoord[i][j + 1] - this->m_texcoord[i][j];
				acuv = this->m_texcoord[i][j + 2] - this->m_texcoord[i][j];
				float fDenominator = acuv.y * abuv.x - abuv.y * acuv.x;

				for(int k = 0; k < 3; ++k) {
					if(fDenominator >= -EPSILON && fDenominator <= EPSILON) {
						this->m_tangent[i][j + k] = CVector3F(1, 0, 0);
						this->m_binormal[i][j + k] = this->m_tangent[i][j + k].CrossProduct(this->m_normal[i][j + k]).Normalize();
					} else {
						float fScale1 = 1.0f / fDenominator;
						this->m_tangent[i][j + k] = ((acuv.y * ab - abuv.y * ac) * fScale1).Normalize();

						// Gram-Schmidt orthogonalization
						this->m_tangent[i][j + k] -= (this->m_normal[i][j + k] * this->m_tangent[i][j + k].DotProduct(this->m_normal[i][j + k])).Normalize();
						this->m_binormal[i][j + k] = this->m_tangent[i][j + k].CrossProduct(this->m_normal[i][j + k]).Normalize();
					}
				}
			}
		}
	}
}
