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

#ifndef __CMD2MESH_HPP
#define __CMD2MESH_HPP

#include "../Medias/IAnimatedMeshBase.hpp"

#define MD2_ID (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')
#define MD2_VERSION 8
#define MD2_SKIN_NAME_SIZE 68
#define MD2_FRAME_NAME_SIZE 16
#define MD2_NUMVERTEX_NORMALS 162

namespace daidalosengine {
	/**
	* MD2 header information structure
	*/
	struct SMD2Header {
		int id;
		int version;
		int skinWidth;
		int skinHeight;
		int frameSize;
		int numSkins;
		int numVertices;
		int numTextureCoordinates;
		int numTriangle;
		int numGLCommands;
		int numFrames;

		int offsetSkins;
		int offsetTextureCoordinates;
		int offsetTriangles;
		int offsetFrames;
		int offsetGLCommands;
		int offsetEnd;
	};

	/**
	* MD2 veretx information structure
	*/
	struct SMD2Vertex {
		unsigned char position[3];
		unsigned char normalIndice;
		CVector3F fullPosition;
		CVector3F averageNormal;
	};

	/**
	* MD2 triangle information structure
	*/
	struct SMD2Triangle {
		short vertex[3];
		short textureCoordinatesIndice[3];
		CVector3F faceNormal;
	};

	/**
	* MD2 texture coordinates information structure
	*/
	struct SMD2TextureCoordinates {
		short u;
		short v;
	};

	/**
	* MD2 frame information structure
	*/
	struct SMD2Frame {
		CVector3F scale;
		CVector3F translation;
		char name[MD2_FRAME_NAME_SIZE];
		SMD2Vertex * vertex;
	};

	/**
	* MD2 skin information structure
	*/
	struct SMD2Skin {
		char name[MD2_SKIN_NAME_SIZE];
	};

	/**
	* MD2 face information structure
	*/
	struct SMD2Face {
		CVector3F position[3];
		CVector3F normal[3];
		CVector2F textureCoordinates[3];
	};

	/**
	* MD2 internal data structure
	*/
	struct SMD2InternalData {
		SMD2Header header;
		SMD2Skin * skin;
		SMD2TextureCoordinates * textureCoordinates;
		SMD2Triangle * triangle;
		SMD2Frame * frame;
		CVector3F * transformedVertex;
		CVector3F * transformedNormal;
	};

	/**
	* MD2 mesh
	*/
	class CMD2Mesh : public IAnimatedMeshBase {
	private:
		SMD2InternalData * m_internalData;

	public:
		/**
		* Constructor
		* @param name The resource name.
		* @param data The internal data
		*/
		CMD2Mesh(const std::string & name, SMD2InternalData * data);

		/**
		* Destructor
		*/
		~CMD2Mesh();

		/**
		* Finalize the geometry.
		* Copy the geometry to the output buffers.
		* @param shadingType The shading type
		*/
		void FinalizeGeometry(EMeshShadingType shadingType);

		/**
		* Get the number of frames
		* @return The number of frames
		*/
		const int GetNumberOfFrames() const;

	private:
		CMD2Mesh(const CMD2Mesh & copy);
		const CMD2Mesh & operator=(const CMD2Mesh & copy);
	};
}

#endif
