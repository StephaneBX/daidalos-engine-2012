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

#ifndef __CMS3DLOADER_HPP
#define __CMS3DLOADER_HPP

#include <string>
#include "../Core/ILoader.hpp"
#include "../Medias/CMS3DMesh.hpp"

namespace daidalosengine {
	/**
	* MS3D header structure
	*/
	struct SMS3DHeader {
		char id[10];
		int version;
	};

	/**
	* MS3D veretx information structure
	*/
	struct SMS3DVertex {
		unsigned char flag;
		CVector3F position;
		char boneId;
		unsigned char unused;

		// Currently unused
		char boneIds[3];
		unsigned char weights[3];
		unsigned int extra;
	};

	/**
	* MS3D face information structure
	*/
	struct SMS3DFace {
		unsigned short flags;
		unsigned short vertexIndices[3];
		CVector3F normals[3];
		float texCoords[2][3];
		unsigned char smoothing;
		unsigned char groupId;
	};

	/**
	* MS3D mesh information structure
	*/
	struct SMS3DMesh {
		unsigned char flags;
		char name[32];
		unsigned short numFaces;
		unsigned short * facesIndices;
		char materialId;
		std::string comment;
	};

	/**
	* MS3D material information structure
	*/
	struct SMS3DMaterial {
		char name[32];
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float emission[4];
		float shininess;
		float transparency;
		unsigned char mode;
		char texture[128];
		char alpha[128];
		std::string comment;
	};

	/**
	* MS3D key frame information structure
	*/
	struct SMS3DKeyFrame {
		float time;
		CVector3F param;
	};

	/**
	* MS3D joint information structure
	*/
	struct SMS3DJoint {
		// Data from file
		unsigned char flags;
		char name[32];
		char parentName[32];
		CVector3F rotation;
		CVector3F position;
		unsigned short numRotationFrames;
		unsigned short numTranslationFrames;
		SMS3DKeyFrame * rotationKeyFrames;
		SMS3DKeyFrame * translationKeyFrames;

		// Extended data
		int parent;
		CMatrix matrixLocalSkeleton;
		CMatrix matrixGlobalSkeleton;
		CMatrix matrixLocal;
		CMatrix matrixGlobal;
		unsigned short currentRotationFrame;
		unsigned short currentTranslationFrame;
		std::string comment;
	};

	/**
	* MS3D data information structure
	*/
	struct SMS3DData {
		int numberOfFrames;
		unsigned short numberOfVertices;
		unsigned short numberOfFaces;
		unsigned short numberOfMeshes;
		unsigned short numberOfMaterials;
		unsigned short numberOfJoints;

		SMS3DHeader header;
		SMS3DVertex * vertex;
		SMS3DFace * face;
		SMS3DMesh * mesh;
		SMS3DMaterial * material;
		SMS3DJoint * joint;
		std::string comment;

		CVector3F * transformedVertex;
		CVector3F * transformedNormal;
	};

	/**
	* Loader for MS3D files
	*/
	class CMS3DLoader : public ILoader < IAnimatedMeshBase > {
	private:
		std::string m_filename;
		SMS3DData * m_data;
		SMS3DInternalData * m_internalData;


	public:
		/**
		* Constructor
		*/
		CMS3DLoader();

		/**
		* Destructor
		*/
		virtual ~CMS3DLoader();

		/**
		* Load a ms3d mesh from a file
		* @param filename The ms3d file name
		* @return The mesh
		*/
		virtual IAnimatedMeshBase * LoadFromFile(const std::string & filename) override;

	private:
		/**
		* Initialize the parents
		* Find the parent for each bone
		*/
		void InitializeParents();

		/**
		* Initialize the skeleton matrixes
		*/
		void InitializeSkeletonsMatrixes();

		/**
		/* Initialize the internal data
		*/
		void InitializeInternalData();

		/**
		* Animate the MS3D file
		* @param frame The frame
		*/
		void Animate(const float frame);

		/**
		* Build the global matrix
		*/
		void BuildGlobalMatrix();

		/**
		* Perform calculation to have the final vertices
		*/
		void TransformedVerticesCalculation();

		/**
		* Perform calculation to have the final normals
		*/
		void TransformedNormalsCalculation();
	};
}

#endif
