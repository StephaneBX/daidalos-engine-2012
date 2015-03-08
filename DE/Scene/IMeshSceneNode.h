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

#ifndef __IMESHSCENENODE_H
#define __IMESHSCENENODE_H

#include "../Core/Enums.h"
#include "../Medias/CTexture2D.h"
#include "../Medias/CMaterial.h"
#include "../Scene/CSphere.h"
#include "../Scene/CMeshBuffer.h"
#include "../Scene/ISceneNode.h"


namespace DE {
	/**
	* Basic class for mesh nodes
	*/
	class IMeshSceneNode : public ISceneNode {
	protected:
		bool m_needGeometryUpdate;
		bool m_needVisibilityShapeUpdate;
		int m_numberOfGroups;
		std::string m_filename;
		EMeshShadingType m_shadingType;
		CMeshBuffer * m_buffer;
		CTexture2D * m_diffuseTexture;
		CTexture2D * m_normalTexture;
		CMaterial m_material;
		std::vector<std::string> m_materialElement;

		CSphereF * m_visibilityShape;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		*/
		IMeshSceneNode(IScene * scene, ISceneNode * parent = nullptr, const std::string & name = "");

		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param rotation The rotation
		* @param scale The scale
		*/
		IMeshSceneNode(IScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale);

		/**
		* Destructor
		*/
		virtual ~IMeshSceneNode();

		/**
		* Set a material
		* @param filename The material filename
		*/
		virtual void SetMeshMaterial(const std::string & filename);

		/**
		* Set a texture
		* @param textureType The texture type
		* @param filename The texture name
		* @param format The texture format
		* @param filter The texture filter policy
		* @param group The group
		*/
		virtual void SetMeshTexture(ETextureType textureType, const std::string & filename, EPixelFormat format, const STexture2DFilterPolicy * filter = nullptr, const int group = MESHSCENE_ALL_GROUPS);

		/**
		* Update the absolute position
		* @param time The elapsed time
		*/
		virtual void Update(const float time = 0);

		/**
		* Set the local position
		* @param position The local position
		*/
		virtual void SetPosition(const CVector3F & position);

		/**
		* Set the local scale
		* @param scale The local scale
		*/
		virtual void SetScale(const CVector3F & scale);

		/**
		* Set the mesh format
		* @param format The mesh format
		*/
		virtual void SetMeshFormat(const int format);

		/**
		* Set the mesh shading type
		* @param shadingType The mesh shading type
		*/
		virtual void SetMeshShadingType(EMeshShadingType shadingType);

		/**
		* Render the mesh
		*/
		virtual void Render();

	protected:
		/**
		* Finalize the geometry before rendering the mesh
		*/
		virtual void FinalizeGeometry();
	};
}

#endif
