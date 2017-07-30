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

#ifndef __CMS3DMESH_HPP
#define __CMS3DMESH_HPP



#include "../Core/Utility/CMatrix.hpp"
#include "../Medias/IAnimatedMeshBase.hpp"

namespace DE {
	#define MS3D_ROOT_JOINT -1
	#define MS3D_VERTEX_NO_BONES -1

	/**
	* MS3D internal data structure for meshes
	*/
	struct SMS3DInternalDataMesh {
		int numberOfFaces;
		CVector3F ** vertex;
		CVector3F ** normal;
		CVector2F * textureCoordinates;
	};

	/**
	* MS3D internal data structure
	*/
	struct SMS3DInternalData {
		int numberOfFrames;
		int numberOfMeshes;
		SMS3DInternalDataMesh * mesh;
	};

	/**
	* MS3D (Milkshape 3D) mesh
	*/
	class CMS3DMesh : public IAnimatedMeshBase {
	private:
		SMS3DInternalData * m_internalData;

	public:
		/**
		* Constructor
		* @param name The resource name.
		* @param data The internal data
		*/
		CMS3DMesh(const std::string & name, SMS3DInternalData * data);

		/**
		* Destructor
		*/
		~CMS3DMesh();

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
		CMS3DMesh(const CMS3DMesh & copy);
		const CMS3DMesh & operator=(const CMS3DMesh & copy);
	};
}

#endif
