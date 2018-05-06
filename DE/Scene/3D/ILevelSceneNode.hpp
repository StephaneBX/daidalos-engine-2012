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

#ifndef __ILEVELSCENENODE_HPP
#define __ILEVELSCENENODE_HPP

#include <set>
#include "../../Scene/3D/CMeshBuffer.hpp"
#include "../../Scene/3D/ICameraSceneNode.hpp"
#include "../../Scene/3D/ISceneNode.hpp"
#include "../../Scene/3D/CAABoundingBox.hpp"

namespace daidalosengine {
	/**
	* Base class for level nodes
	*/
	class ILevelSceneNode : public ISceneNode {
	private:
		EMeshShadingType m_shadingType;
		bool m_useGlobalAmbientColor;
		int m_numberOfVisibleTriangles;
		CVector4F m_ambientColor;
		std::set<ITexture2DBase*> m_diffuseTextures;
		std::set<ITexture2DBase*> m_normalTextures;
		CMeshBuffer m_levelBuffer;
		ICameraSceneNode * m_activeCamera;
		CAABoundingBoxF m_levelBoundingBox;

		std::string m_skyName;
		CVector4F m_skyAmbientColor;

	public:
		/**
		* Constructor
		*/
		explicit ILevelSceneNode(I3DScene * scene);

		/**
		* Destructor
		*/
		virtual ~ILevelSceneNode();

		/**
		* Set on/off the global ambient color
		* @param useGlobalAmbientColor The global ambient color flag
		*/
		void SetUseGlobalAmbientColor(const bool useGlobalAmbientColor);

		/**
		* Set the mesh format
		* @param format The mesh format
		*/
		void SetMeshFormat(const int format);

		/**
		* Set the numbe of visible triangles
		* @param visibleTriangles The number of visible triangles
		*/
		void SetNumberOfVisibleTriangles(const int visibleTriangles);

		/**
		* Set the mesh shading type
		* @param shadingType The mesh shading type
		*/
		void SetMeshShadingType(EMeshShadingType shadingType);

		/**
		* Set the ambient color of the level
		* @param ambientColor The ambient color
		*/
		void SetAmbientColor(const CVector4F & ambientColor);

		/**
		* Set the sky name
		* @param name The sky name
		*/
		void SetSkyName(const std::string & name);

		/**
		* Set the sky ambient color
		* @param ambientColor The sky ambient color
		*/
		void SetSkyAmbientColor(const CVector4F & ambientColor);

		/**
		* Add a diffuse texture
		* @param texture The diffuse texture
		*/
		void AddDiffuseTexture(ITexture2DBase * texture);

		/**
		* Add a normal texture
		* @param texture The normal texture
		*/
		void AddNormalTexture(ITexture2DBase * texture);

		/**
		* Get the global ambient color flag
		* @return The global ambient color flag
		*/
		const bool GetUseGlobalAmbientColor() const;

		/**
		* Get the number of visible triangles
		* @return The number of visible triangles
		*/
		const int GetNumberOfVisibleTriangles() const;

		/**
		* Get the mesh shading type
		* @return The mesh shading type
		*/
		const EMeshShadingType GetMeshShadingType() const;

		/**
		* Get the ambient color
		* @return The ambient color
		*/
		const CVector4F & GetAmbientColor() const;

		/**
		* Get the sky name
		* @return The sky name
		*/
		const std::string & GetSkyName() const;

		/**
		* Get the sky ambient color
		* @return The sky ambient color
		*/
		const CVector4F & GetSkyAmbientColor() const;

		/**
		* Get the level bounding box
		* @return The level bounding box
		*/
		const CAABoundingBoxF GetLevelBoundingBox();

		/**
		* Get the active camera
		* @return The active camera
		*/ 
		ICameraSceneNode * GetActiveCamera() const;

		/**
		* Render the node
		* @param filterId The filter id to apply the select the rendered nodes
		*/
		virtual void Render(const int filterId = SCENE_DEFAULT_ID) override = 0;

		/**
		* Render the transparent elements of the level
		*/
		virtual void RenderTransparent() override = 0;

		/**
		* Render the geometry that will let the skybox pass through
		*/
		virtual void RenderSkyboxMask() override = 0;

		/**
		* Set the collision state of a brush id
		*/
		virtual void SetBrushCollision(const int brushId, const bool collision) = 0;

		/**
		* Slide a shape in the level
		* @param shape The shape to slide
		* @param movement A CVector3F describing the shape movement
		* @return True if a collision occurred
		*/
		virtual const bool Slide(IShape3DF & shape, const CVector3F & movement) = 0;

		/**
		* Get the collision point of a moving point
		* @param position The point
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		virtual const bool GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) = 0;

		/**
		* Get the collision point of a moving shape
		* @param shape The shape
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		virtual const bool GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) = 0;

	protected:
		/**
		* Set the level bounding box
		* @param levelBoundingBox The level bonding box
		*/
		void SetLevelBoundingBox(const CAABoundingBoxF & levelBoundingBox);

		/**
		* Set the active camera
		* @param camera The active camera
		*/
		void SetActiveCamera(ICameraSceneNode * camera);

		/**
		* Set the level buffer
		* @param triangles The triangles
		*/
		void SetLevelBuffer(const std::vector<CTriangle3D *> triangles);

		/**
		* Render the primtives from the buffer
		*/
		void RenderPrimitives();

		/**
		* Check if thetexturing is enabled
		* @return True if the texturing is enabled
		*/
		const bool IsTexturingEnabled() const;
	};
}

#endif
