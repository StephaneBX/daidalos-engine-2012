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

#ifndef __CMESHBUFFER_H
#define __CMESHBUFFER_H

#include <algorithm>
#include <vector>
#include "../Core/Enums.h"
#include "../Core/Utility/CVector2.h"
#include "../Core/Utility/CVector3.h"
#include "../Scene/CTriangle3D.h"
#include "../Medias/CBuffer.h"

namespace DE
{
	class IScene;

	/**
	* The mesh buffer elements
	*/
	enum EMeshBufferElement
	{
		MESHBUFFER_ELEMENT_POSITION = 1 << 0,
		MESHBUFFER_ELEMENT_TEXCOORD = 1 << 1,
		MESHBUFFER_ELEMENT_NORMAL   = 1 << 2,
		MESHBUFFER_ELEMENT_NTB      = 1 << 3
	};

	/**
	* Mesh buffer class. Tranform the mesh data into the chosen format into a vertex buffer.
	*/
	class CMeshBuffer
	{
	private:
		int m_format;
		int m_numberOfElements;
		unsigned long m_stride;
		CBuffer m_vertexBuffer;
		IScene * m_scene;

	public:
		/**
		* Constructor
		*/
		CMeshBuffer();

		/**
		* Tranform the data into the mesh buffer format
		* @param vertex The vertices
		* @param texcoord The texture coordinates
		* @param normal The normals
		* @param tangent The tangents
		* @param binormal The binormals
		* @param numberOfElements The number of elements
		*/
		void Transform(CVector3F * vertex, CVector2F * texcoord, CVector3F * normal, CVector3F * tangent, CVector3F * binormal, const int numberOfElements);

		/**
		* Tranform the data into the mesh buffer format
		* @param triangles A vector containing triangles
		* @param shadingType The shading type
		*/
		void Transform(const std::vector<CTriangle3D*> & triangles, EMeshShadingType shadingType = MESH_SHADING_SMOOTH);

		/**
		* Set the mesh format
		* @param format The mesh format
		*/
		void SetMeshFormat(const int format);

		/**
		* Set the associated scene
		* @param scene The associated scene
		*/
		void SetScene(IScene * scene);

		/**
		* Get the number of elements in the buffer
		* @return The number of elements
		*/
		const int GetNumberOfElements() const;

		/**
		* Get the mesh format
		* @return The mesh format
		*/
		const int GetMeshFormat() const;

		/**
		* Get the stride
		* @return The stride
		*/
		const unsigned long GetStride() const;

		/**
		* Get the vertex buffer
		* @return The vertex buffer
		*/
		const CBuffer & GetVertexBuffer() const;
	};
}

#endif
