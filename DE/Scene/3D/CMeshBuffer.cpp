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

#include "../../Core/IRenderer.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Scene/3D/I3DScene.h"
#include "../../Scene/3D/CMeshBuffer.h"

namespace DE {
	CMeshBuffer::CMeshBuffer() {
		LOG_TRACE();

		this->m_numberOfElements = 0;
		SetMeshFormat(MESHBUFFER_ELEMENT_POSITION);
	}

	void CMeshBuffer::Transform(CVector3F * vertex, CVector2F * texcoord, CVector3F * normal, CVector3F * tangent, CVector3F * binormal, const int numberOfElements) {
		LOG_TRACE();

		ICameraSceneNode * camera = this->m_scene->GetActiveCamera();

		// Check the vertex buffer size
		if((int)this->m_vertexBuffer.GetSize() < this->m_stride * numberOfElements) {
			// Create a new one if the current one can't hold the data
			this->m_vertexBuffer = IRenderer::GetRenderer()->CreateVertexBuffer(this->m_stride, numberOfElements, BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);
		}

		int pitch = 0;
		int index = 0;
		unsigned char * lock = (unsigned char*)this->m_vertexBuffer.Lock(this->m_stride, 0, 0, LOCK_WRITEONLY);

		for(int i = 0; i < numberOfElements; i += 3) {
			// Vertex transformation
			for(int j = 0; j < 3; ++j) {
				memcpy(lock + (this->m_stride * (index + j)) + pitch, vertex[i + j], sizeof(CVector3F));
			}
			pitch += sizeof(CVector3F);

			// Texcoord transformation
			if((this->m_format & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, texcoord[i + j], sizeof(CVector2F));
				}
				pitch += sizeof(CVector2F);
			}

			// Normal transformation
			if((this->m_format & MESHBUFFER_ELEMENT_NORMAL) != 0) {
				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, normal[i + j], sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);
			}

			// Normal Tangent and Binormal transformation
			if((this->m_format & MESHBUFFER_ELEMENT_NTB) != 0) {
				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, normal[i + j], sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);

				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, tangent[i + j], sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);

				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, binormal[i + j], sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);
			}

			index += 3;
			pitch = 0;
		}

		this->m_vertexBuffer.Unlock();

		this->m_numberOfElements = index;
	}

	void CMeshBuffer::Transform(const std::vector<CTriangle3D*> & triangles, EMeshShadingType shadingType) {
		LOG_TRACE();

		ICameraSceneNode * camera = this->m_scene->GetActiveCamera();

		// Check the vertex buffer size
		if((int)this->m_vertexBuffer.GetSize() < this->m_stride * triangles.size() * 3) {
			// Create a new one if the current one can't hold the data
			this->m_vertexBuffer = IRenderer::GetRenderer()->CreateVertexBuffer(this->m_stride, triangles.size() * 3, BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);
		}

		int pitch = 0;
		int index = 0;
		CVector3F ab, ac, normalFace, tangent, binormal;
		CVector3F tangentBinormal[2];
		CVector2F abuv, acuv;
		unsigned char * lock = (unsigned char*)this->m_vertexBuffer.Lock(this->m_stride, 0, 0, LOCK_WRITEONLY);

		for(int i = 0; i < (int)triangles.size(); ++i) {
			// Vertex transformation
			for(int j = 0; j < 3; ++j) {
				memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetVertex(j), sizeof(CVector3F));
			}
			pitch += sizeof(CVector3F);

			// Texcoord transformation
			if((this->m_format & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetTexcoord(j), sizeof(CVector2F));
				}
				pitch += sizeof(CVector2F);
			}

			// Normal transformation
			if((this->m_format & MESHBUFFER_ELEMENT_NORMAL) != 0) {
				switch(shadingType) {
				case MESH_SHADING_FLAT:
					for(int j = 0; j < 3; ++j) {
						memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetFlatNormal(), sizeof(CVector3F));
					}
					break;

				default:
					for(int j = 0; j < 3; ++j) {
						memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetNormal(j), sizeof(CVector3F));
					}
					break;
				}

				pitch += sizeof(CVector3F);
			}

			// Normal Tangent and Binormal transformation
			if((this->m_format & MESHBUFFER_ELEMENT_NTB) != 0) {
				CVector3F tmpNormal[3];
				switch(shadingType) {
				case MESH_SHADING_FLAT:
					for(int j = 0; j < 3; ++j) {
						tmpNormal[j] = triangles[i]->GetFlatNormal();
					}
					break;

				default:
					for(int j = 0; j < 3; ++j) {
						triangles[i]->GetNormal(j);
					}
					break;
				}

				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, tmpNormal[j], sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);

				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetTangent(j), sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);

				for(int j = 0; j < 3; ++j) {
					memcpy(lock + (this->m_stride * (index + j)) + pitch, triangles[i]->GetBinormal(j), sizeof(CVector3F));
				}
				pitch += sizeof(CVector3F);
			}

			index += 3;
			pitch = 0;
		}

		this->m_vertexBuffer.Unlock();

		this->m_numberOfElements = index;
	}

	void CMeshBuffer::SetScene(I3DScene * scene) {
		LOG_TRACE();

		this->m_scene = scene;
	}

	void CMeshBuffer::SetMeshFormat(const int format) {
		LOG_TRACE();

		// Format verification
		// The position must be selected
		Assert((format & MESHBUFFER_ELEMENT_POSITION) != 0);

		// MESHBUFFER_ELEMENT_NORMAL and MESHBUFFER_ELEMENT_NTB can't be both chosen
		// In addition, MESHBUFFER_ELEMENT_NTB need MESHBUFFER_ELEMENT_TEXCOORD for calculation reason
		if((format & MESHBUFFER_ELEMENT_NTB) != 0) {
			Assert((format & MESHBUFFER_ELEMENT_TEXCOORD) != 0);
			Assert((format & MESHBUFFER_ELEMENT_NORMAL) == 0);
		}

		this->m_format = format;

		// Stride calculation
		this->m_stride = sizeof(CVector3F);

		if((format & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
			this->m_stride += sizeof(CVector2F);
		}

		if((format & MESHBUFFER_ELEMENT_NORMAL) != 0) {
			this->m_stride += sizeof(CVector3F);
		}

		if((format & MESHBUFFER_ELEMENT_NTB) != 0) {
			this->m_stride += sizeof(CVector3F) * 3;
		}
	}

	const int CMeshBuffer::GetNumberOfElements() const {
		LOG_TRACE();

		return this->m_numberOfElements;
	}

	const int CMeshBuffer::GetMeshFormat() const {
		LOG_TRACE();

		return this->m_format;
	}

	const unsigned long CMeshBuffer::GetStride() const {
		LOG_TRACE();

		return this->m_stride;
	}

	const CBuffer & CMeshBuffer::GetVertexBuffer() const {
		LOG_TRACE();

		return this->m_vertexBuffer;
	}
}
