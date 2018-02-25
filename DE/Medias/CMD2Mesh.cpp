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
#include "../Core/Utility/CMatrix.hpp"
#include "../Core/Utility/CVector3.hpp"
#include "../Medias/CMD2Mesh.hpp"

namespace daidalosengine {
	CMD2Mesh::CMD2Mesh(const std::string & name, SMD2InternalData * data) : IAnimatedMeshBase(name) {
		LOG_TRACE();

		this->m_internalData = data;

		this->m_vertex = new CVector3F*[1];
		this->m_normal = new CVector3F*[1];
		this->m_tangent = new CVector3F*[1];
		this->m_binormal = new CVector3F*[1];
		this->m_texcoord = new CVector2F*[1];

		// Data initialization
		this->m_groupSize.push_back(this->m_internalData->header.numTriangle * 3);
		this->m_vertex[0] = new CVector3F[this->m_internalData->header.numTriangle * 3];
		this->m_normal[0] = new CVector3F[this->m_internalData->header.numTriangle * 3];
		this->m_tangent[0] = new CVector3F[this->m_internalData->header.numTriangle * 3];
		this->m_binormal[0] = new CVector3F[this->m_internalData->header.numTriangle * 3];
		this->m_texcoord[0] = new CVector2F[this->m_internalData->header.numTriangle * 3];

		// Copy texture coordinates
		for(int i = 0; i < this->m_internalData->header.numTriangle; i++) {
			for(int j = 0; j < 3; j++) {
				this->m_texcoord[0][i * 3 + j].x = (float)(this->m_internalData->textureCoordinates[this->m_internalData->triangle[i].textureCoordinatesIndice[j]].u) / ((float)this->m_internalData->header.skinWidth);
				this->m_texcoord[0][i * 3 + j].y = (float)(this->m_internalData->textureCoordinates[this->m_internalData->triangle[i].textureCoordinatesIndice[j]].v) / ((float)this->m_internalData->header.skinHeight);
			}
		}

		// Visibility shape calculation
		CVector3F min;
		CVector3F max;
		for(int frame = 0; frame < this->m_internalData->header.frameSize; ++frame) {
			this->m_frame = (float) frame;
			FinalizeGeometry(MESH_SHADING_SMOOTH);

			for(int j = 0; j < this->m_internalData->header.numTriangle * 3; ++j) {

				max.x = std::max(max.x, this->m_vertex[0][j].x);
				max.y = std::max(max.y, this->m_vertex[0][j].y);
				max.z = std::max(max.z, this->m_vertex[0][j].z);

				min.x = std::min(min.x, this->m_vertex[0][j].x);
				min.y = std::min(min.y, this->m_vertex[0][j].y);
				min.z = std::min(min.z, this->m_vertex[0][j].z);
			}
		}

		// Find the position
		this->m_visibilityShape->SetPosition((max + min) / 2.0F);

		// Find the radius
		this->m_visibilityShape->SetRadius((max - ((max + min) / 2.0F)).Length());

		TangentAndBinormalCalculation();
	}

	CMD2Mesh::~CMD2Mesh() {
		LOG_TRACE();

		SAFE_DELETE_ARRAY(this->m_internalData->skin);
		SAFE_DELETE_ARRAY(this->m_internalData->textureCoordinates);
		SAFE_DELETE_ARRAY(this->m_internalData->triangle);
		SAFE_DELETE_ARRAY(this->m_internalData->transformedVertex);
		SAFE_DELETE_ARRAY(this->m_internalData->transformedNormal);

		for(int i = 0; i < this->m_internalData->header.numFrames; ++i)
			SAFE_DELETE_ARRAY(this->m_internalData->frame[i].vertex);
		SAFE_DELETE_ARRAY(this->m_internalData->frame);
		SAFE_DELETE(this->m_internalData);
	}

	void CMD2Mesh::FinalizeGeometry(EMeshShadingType shadingType) {
		LOG_TRACE();

		int frameA = (int)floor(this->m_frame);
		int frameB = (int)floor(this->m_frame + 1);

		// Linear
		float interpolation = this->m_frame - frameA;

		// Cosine
		float interpolation2 = (1 - cos(interpolation * PI)) / 2.0F;

		// Use the first frame if the choosen frame is less than 0
		if(frameA < 0) {
			frameA = 0;
			frameB = 0;
		}

		// Use the last frame if the choosen frame is bigger than this->m_header.m_numFrames - 1
		if(frameB > this->m_internalData->header.numFrames - 1) {
			frameA = this->m_internalData->header.numFrames - 1;
			frameB = this->m_internalData->header.numFrames - 1;
		}

		SMD2Frame * currentFrameA = &this->m_internalData->frame[frameA];
		SMD2Frame * currentFrameB = &this->m_internalData->frame[frameB];
		SMD2Vertex * currentVertexA;
		SMD2Vertex * currentVertexB;
		CVector3F currentNormalA;
		CVector3F currentNormalB;
		CVector3F currentPositionA;
		CVector3F currentPositionB;

		for(int i = 0; i < this->m_internalData->header.numVertices; i++) {
			currentVertexA = &currentFrameA->vertex[i];
			currentVertexB = &currentFrameB->vertex[i];

			currentNormalA.x = currentVertexA->averageNormal.x;
			currentNormalA.z = currentVertexA->averageNormal.y;
			currentNormalA.y = currentVertexA->averageNormal.z;

			currentNormalB.x = currentVertexB->averageNormal.x;
			currentNormalB.z = currentVertexB->averageNormal.y;
			currentNormalB.y = currentVertexB->averageNormal.z;

			this->m_internalData->transformedNormal[i] = currentNormalA.CosineInterpolation(currentNormalB, interpolation2);

			// Swap the y and z axis and inverse the x and z axis values, the md2 file uses a different space coordinates
			currentPositionA.x = currentVertexA->fullPosition.x;
			currentPositionA.z = currentVertexA->fullPosition.y;
			currentPositionA.y = currentVertexA->fullPosition.z;

			currentPositionB.x = currentVertexB->fullPosition.x;
			currentPositionB.z = currentVertexB->fullPosition.y;
			currentPositionB.y = currentVertexB->fullPosition.z;

			this->m_internalData->transformedVertex[i] = currentPositionA.CosineInterpolation(currentPositionB, interpolation2);
		}

		// Do a 90 degree rotation to match with the engine space coordinates
		CMatrix rotation(CVector3F(1, 1, 1), CVector3F(0, DegreeToRadian(90), 0), CVector3F(0, 1, 0));
		CMatrix::TransformArray(this->m_internalData->transformedVertex, this->m_internalData->transformedVertex, rotation, this->m_internalData->header.numVertices, false);
		CMatrix::TransformArray(this->m_internalData->transformedNormal, this->m_internalData->transformedNormal, rotation, this->m_internalData->header.numVertices, true);

		/**
		* Data interpolation
		*/
		switch(shadingType) {
		case MESH_SHADING_FLAT:
			for(int i = 0; i < this->m_internalData->header.numTriangle; ++i) {
				for(int j = 0; j < 3; ++j) {
					this->m_vertex[0][i * 3 + j] = this->m_internalData->transformedVertex[this->m_internalData->triangle[i].vertex[j]];
				}
			}
			FlatShadingNormals();
			break;

		case MESH_SHADING_SMOOTH:
			for(int i = 0; i < this->m_internalData->header.numTriangle; ++i) {
				for(int j = 0; j < 3; ++j) {
					this->m_vertex[0][i * 3 + j] = this->m_internalData->transformedVertex[this->m_internalData->triangle[i].vertex[j]];
					this->m_normal[0][i * 3 + j] = this->m_internalData->transformedNormal[this->m_internalData->triangle[i].vertex[j]];
				}
			}
			break;
		}
	}

	const int CMD2Mesh::GetNumberOfFrames() const {
		LOG_TRACE();

		return this->m_internalData->header.numFrames;
	}
}
