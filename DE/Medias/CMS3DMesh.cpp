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
#include "../Core/Logger/ILogger.hpp"
#include "../Medias/CMS3DMesh.hpp"

namespace daidalosengine {
	CMS3DMesh::CMS3DMesh(const std::string & name, SMS3DInternalData * data) : IAnimatedMeshBase(name) {
		LOG_TRACE();

		// Data initialization
		this->m_internalData = data;
		InitializeBuffers(this->m_internalData->numberOfMeshes);

		for(int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
			InitializeBufferGroup(this->m_internalData->mesh[i].numberOfFaces * 3);

			// Copy texture coordinates
			memcpy(GetTexcoords(i), this->m_internalData->mesh[i].textureCoordinates, sizeof(CVector2F) * this->m_internalData->mesh[i].numberOfFaces * 3);
		}

		// Visibility shape calculation
		CVector3F min;
		CVector3F max;
		for(int frame = 0; frame < this->m_internalData->numberOfFrames; ++frame) {
			SetFrame((float) frame);
			FinalizeGeometry(MESH_SHADING_SMOOTH);

			for(int j = 0; j < this->m_internalData->numberOfMeshes; ++j) {
				for(int k = 0; k < this->m_internalData->mesh[j].numberOfFaces * 3; ++k) {
					max.x = std::max(max.x, GetVertices(j)[k].x);
					max.y = std::max(max.y, GetVertices(j)[k].y);
					max.z = std::max(max.z, GetVertices(j)[k].z);

					min.x = std::min(min.x, GetVertices(j)[k].x);
					min.y = std::min(min.y, GetVertices(j)[k].y);
					min.z = std::min(min.z, GetVertices(j)[k].z);
				}
			}
		}

		// Find the position
		GetVisibilityShape()->SetPosition((max + min) / 2.0F);

		// Find the radius
		GetVisibilityShape()->SetRadius((max - ((max + min) / 2.0F)).Length());

		TangentAndBinormalCalculation();
	}

	CMS3DMesh::~CMS3DMesh() {
		LOG_TRACE();

		for(int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
			for(int j = 0; j < this->m_internalData->numberOfFrames; ++j) {
				SAFE_DELETE_ARRAY(this->m_internalData->mesh[i].vertex[j]);
				SAFE_DELETE_ARRAY(this->m_internalData->mesh[i].normal[j]);
			}

			SAFE_DELETE_ARRAY(this->m_internalData->mesh[i].vertex);
			SAFE_DELETE_ARRAY(this->m_internalData->mesh[i].normal);
			SAFE_DELETE_ARRAY(this->m_internalData->mesh[i].textureCoordinates);
		}

		SAFE_DELETE_ARRAY(this->m_internalData->mesh);
		SAFE_DELETE(this->m_internalData);
	}

	void CMS3DMesh::FinalizeGeometry(EMeshShadingType shadingType) {
		LOG_TRACE();

		int frameA = (int)floor(GetFrame());
		int frameB = (int)floor(GetFrame() + 1);

		// Linear
		float interpolation = GetFrame() - frameA;

		// Cosine
		float interpolation2 = (1 - cos(interpolation * PI)) / 2.0F;

		// Use the first frame if the choosen frame is less than 0
		if(frameA < 0) {
			frameA = 0;
			frameB = 0;
		}

		// Use the last frame if the choosen frame is bigger than this->m_header.m_numFrames - 1
		if(frameB > this->m_internalData->numberOfFrames - 1) {
			frameA = this->m_internalData->numberOfFrames - 1;
			frameB = this->m_internalData->numberOfFrames - 1;
		}

		/**
		* Data interpolation
		*/
		switch(shadingType) {
		case MESH_SHADING_SMOOTH:
			for(int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
				for(int j = 0; j < this->m_internalData->mesh[i].numberOfFaces * 3; ++j) {
					GetVertices(i)[j] = this->m_internalData->mesh[i].vertex[frameA][j].LinearInterpolation(this->m_internalData->mesh[i].vertex[frameB][j], interpolation2);
					GetNormals(i)[j] = this->m_internalData->mesh[i].normal[frameA][j].LinearInterpolation(this->m_internalData->mesh[i].normal[frameB][j], interpolation2);
				}
			}
			break;

		default:
			// Flat shading
			for (int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
				for (int j = 0; j < this->m_internalData->mesh[i].numberOfFaces * 3; ++j) {
					GetVertices(i)[j] = this->m_internalData->mesh[i].vertex[frameA][j].LinearInterpolation(this->m_internalData->mesh[i].vertex[frameB][j], interpolation2);
				}
			}
			FlatShadingNormals();
			break;
		}
	}

	int CMS3DMesh::GetNumberOfFrames() const {
		LOG_TRACE();

		return this->m_internalData->numberOfFrames;
	}
}
