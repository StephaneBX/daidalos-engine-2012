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

#include "../Core/Logger/ILogger.hpp"
#include "../Loaders/CMD2Loader.hpp"

namespace DE {
	CMD2Loader::CMD2Loader() {
		LOG_TRACE();
	}

	CMD2Loader::~CMD2Loader() {
		LOG_TRACE();
	}

	IAnimatedMeshBase * CMD2Loader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		this->m_internalData = new SMD2InternalData;

		FILE * file;
		fopen_s(&file, filename.c_str(), "rb");

		fread(&this->m_internalData->header, sizeof(SMD2Header), 1, file);
		if(this->m_internalData->header.id == MD2_ID && this->m_internalData->header.version == MD2_VERSION) {
			// Geometry allocation
			this->m_internalData->skin = new SMD2Skin[this->m_internalData->header.numSkins];
			this->m_internalData->textureCoordinates = new SMD2TextureCoordinates[this->m_internalData->header.numTextureCoordinates];
			this->m_internalData->triangle = new SMD2Triangle[this->m_internalData->header.numTriangle];
			this->m_internalData->frame = new SMD2Frame[this->m_internalData->header.numFrames];

			// Read skins information
			fseek(file, this->m_internalData->header.offsetSkins, SEEK_SET);
			fread(this->m_internalData->skin, sizeof(SMD2Skin), this->m_internalData->header.numSkins, file);

			// Read texture coordinates
			fseek(file, this->m_internalData->header.offsetTextureCoordinates, SEEK_SET);
			fread(this->m_internalData->textureCoordinates, sizeof(SMD2TextureCoordinates), this->m_internalData->header.numTextureCoordinates, file);

			// Read triangles information
			fseek(file, this->m_internalData->header.offsetTriangles, SEEK_SET);
			for(int i = 0; i < this->m_internalData->header.numTriangle; i++) {
				fread(this->m_internalData->triangle[i].vertex, sizeof(short), 3, file);
				fread(this->m_internalData->triangle[i].textureCoordinatesIndice, sizeof(short), 3, file);
				this->m_internalData->triangle[i].faceNormal = CVector3F(0, 0, 0);
			}

			fseek(file, this->m_internalData->header.offsetFrames, SEEK_SET);

			for(int i = 0; i < this->m_internalData->header.numFrames; i++) {
				// Vertex allocation
				this->m_internalData->frame[i].vertex = new SMD2Vertex[this->m_internalData->header.numVertices];

				// Read frame information
				fread(&this->m_internalData->frame[i].scale, sizeof(CVector3F), 1, file);
				fread(&this->m_internalData->frame[i].translation, sizeof(CVector3F), 1, file);
				fread(&this->m_internalData->frame[i].name, sizeof(char), MD2_FRAME_NAME_SIZE, file);

				for(int j = 0; j < this->m_internalData->header.numVertices; j++) {
					// Read vertex information
					fread(&this->m_internalData->frame[i].vertex[j].position, sizeof(unsigned char), 3, file);
					fread(&this->m_internalData->frame[i].vertex[j].normalIndice, sizeof(unsigned char), 1, file);
				}
			}

			// Transformed data
			this->m_internalData->transformedVertex = new CVector3F[this->m_internalData->header.numTriangle];
			this->m_internalData->transformedNormal = new CVector3F[this->m_internalData->header.numTriangle];

			fclose(file);

			InitializeGeometry();
		}

		return new CMD2Mesh(filename, this->m_internalData);
	}

	void CMD2Loader::InitializeGeometry() {
		LOG_TRACE();

		// Get the real vertex positions
		for(int i = 0; i < this->m_internalData->header.numFrames; i++) {
			for(int j = 0; j < this->m_internalData->header.numVertices; j++) {
				this->m_internalData->frame[i].vertex[j].fullPosition.x = this->m_internalData->frame[i].vertex[j].position[0] * this->m_internalData->frame[i].scale.x + this->m_internalData->frame[i].translation.x;
				this->m_internalData->frame[i].vertex[j].fullPosition.y = this->m_internalData->frame[i].vertex[j].position[1] * this->m_internalData->frame[i].scale.y + this->m_internalData->frame[i].translation.y;
				this->m_internalData->frame[i].vertex[j].fullPosition.z = this->m_internalData->frame[i].vertex[j].position[2] * this->m_internalData->frame[i].scale.z + this->m_internalData->frame[i].translation.z;
			}
		}

		// Average Normals calculation
		for(int i = 0; i < this->m_internalData->header.numFrames; i++)
			ComputeAverageNormals(i);
	}

	void CMD2Loader::ComputeAverageNormals(const int frameId) {
		LOG_TRACE();

		CVector3F tmpNormal;
		float facesCount;

		// Stock faces normals in the faces structure
		ComputeFaceNormals(frameId);

		// Perform normal calculation for each vertex
		for(int i = 0; i < this->m_internalData->header.numVertices; ++i) {
			tmpNormal = CVector3F(0, 0, 0);
			facesCount = 0;

			for(int j = 0; j < this->m_internalData->header.numTriangle; ++j) {
				// This triangle uses this vertex, so we add the face normal to the list
				if((int)this->m_internalData->triangle[j].vertex[0] == i || (int)this->m_internalData->triangle[j].vertex[1] == i || (int)this->m_internalData->triangle[j].vertex[2] == i) {
					tmpNormal += this->m_internalData->triangle[j].faceNormal;
					facesCount++;
				}
			}

			// Divide the normal by the number of found triangles and normalize the result
			if(facesCount > 0)
				tmpNormal = tmpNormal / facesCount;
			tmpNormal = tmpNormal.Normalize();

			// We use this normal
			this->m_internalData->frame[frameId].vertex[i].averageNormal = tmpNormal;
		}
	}

	void CMD2Loader::ComputeFaceNormals(const int frameId) {
		LOG_TRACE();

		// Perform face normal calculation for each triangle of the frame
		for(int i = 0; i < this->m_internalData->header.numTriangle; ++i) {
			// Normal calculation
			CVector3F ab = this->m_internalData->frame[frameId].vertex[this->m_internalData->triangle[i].vertex[0]].fullPosition - this->m_internalData->frame[frameId].vertex[this->m_internalData->triangle[i].vertex[2]].fullPosition;
			CVector3F ac = this->m_internalData->frame[frameId].vertex[this->m_internalData->triangle[i].vertex[2]].fullPosition - this->m_internalData->frame[frameId].vertex[this->m_internalData->triangle[i].vertex[1]].fullPosition;
			this->m_internalData->triangle[i].faceNormal = ab.CrossProduct(ac);
		}
	}
}
