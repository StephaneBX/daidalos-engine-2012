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

#ifndef __ISCENENODE_HPP
#define __ISCENENODE_HPP

#include <list>
#include <string>
#include <map>
#include "../../Scene/3D/IShape3D.hpp"
#include "../../Medias/ITexture2DBase.hpp"
#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CMatrix.hpp"

#define MESHSCENE_ALL_GROUPS -1
#define SCENE_DEFAULT_ID 0
#define SCENE_SKYBOX_ID -9999
#define SCENE_TRANSPARENT_ID -9998
#define SCENE_TOP_LAYER_ID -9997

namespace daidalosengine {

	class I3DScene;
	/**
	* Basic class for the scene nodes.
	*/
	class ISceneNode {
	private:
		bool m_enabled;
		bool m_active;
		bool m_saved;
		bool m_shadowCaster;
		int m_id;
		std::string m_name;
		std::string m_boundingBox;
		ESceneNodeType m_type;
		CVector3F m_localPosition;
		CVector3F m_localRotation;
		CVector3F m_localScale;
		CVector3F m_absolutePosition;
		CMatrix m_localPositionMatrix;
		CMatrix m_localRotationMatrix;
		CMatrix m_localScaleMatrix;
		CMatrix m_localMatrix;
		CMatrix m_absolutePositionMatrix;
		CMatrix m_absoluteRotationMatrix;
		CMatrix m_absoluteScaleMatrix;
		CMatrix m_worldMatrix;
		std::map<std::string, std::string> m_attributes;

		I3DScene * m_scene;
		ISceneNode * m_parent;
		std::list<ISceneNode*> m_children;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		*/
		ISceneNode(I3DScene * scene, ISceneNode * parent = nullptr, const std::string & name = "");

		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param rotation The rotation
		*/
		ISceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation);

		/**
		* Destructor
		*/
		virtual ~ISceneNode();

		/**
		* Add a child to the node
		* @param child The child node
		*/
		void AddChild(ISceneNode * child);

		/**
		* Set the attributes map
		* @param attributes The attributes map to set
		*/
		void SetAttributes(const std::map<std::string, std::string> & attributes);

		/**
		* Add an attribute to the attributes map
		* @param attributeName The attribute name
		* @param attributeValue The attribute value
		*/
		void AddAttribute(const std::string & attributeName, const std::string attributeValue);

		/**
		* Set the active status
		* @param active
		*/
		void SetActive(const bool active);

		/**
		* Set the node enabled status
		* @param enabled The enabled status
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the saved status
		* @param saved The saved status
		*/
		void SetSaved(const bool saved);

		/**
		* Set the cast shadows status
		* @param shadowCaster The cast shadows status
		*/
		void SetShadowCaster(const bool shadowCaster);

		/**
		* Set the node type
		* @param type The type
		*/
		void SetType(ESceneNodeType type);

		/**
		* Set the node id
		* @param id The id
		*/
		void SetId(const int id);

		/**
		* Set the node name
		* @param name The node name
		*/
		void SetName(const std::string name);

		/**
		* Set the bounding box name
		* @param boundingBox The bounding box name
		*/
		void SetBoundingBox(const std::string boundingBox);

		/**
		* Set the local position
		* @param position The local position
		*/
		virtual void SetPosition(const CVector3F & position);

		/**
		* Set the local rotation
		* @param rotation The local rotation
		*/
		virtual void SetRotation(const CVector3F & rotation);

		/**
		* Set the local scale
		* @param scale The local scale
		*/
		virtual void SetScale(const CVector3F & scale);

		/**
		* Update the absolute position
		* @param time The elapsed time
		*/
		virtual void Update(const float time = 0);

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
		* Render the node
		* @param filterId The filter id to apply the select the rendered nodes
		*/
		virtual void Render(const int filterId = SCENE_DEFAULT_ID);

		/**
		* Render the node transparent parts
		*/
		virtual void RenderTransparent();

		/**
		* Render the geometry that will let the skybox pass through
		*/
		virtual void RenderSkyboxMask();

		/**
		* Render debug information
		*/
		virtual void RenderDebugInformation();

		/**
		* Get the type
		* @return The node type
		*/
		ESceneNodeType GetType() const;

		/**
		* Check if the node is collidable
		*/
		virtual const bool IsCollidable() const;

		/**
		* Check if the node is enabled
		* @return True if the node is enabled
		*/
		const bool IsEnabled() const;

		/**
		* Check if the node is saved by the loader
		* @return True if the node is saved
		*/
		const bool IsSaved() const;

		/**
		* Check if the node is can cast shadows
		* @return True if the node can cast shadows
		*/
		const bool IsShadowCaster() const;

		/**
		* Check if the node is active
		* @return True if the node is active
		*/
		const bool IsActive() const;

		/**
		* Remove a child from the node
		* @param child The child to remove
		* @return The removal result
		*/
		const bool RemoveChild(ISceneNode * child);

		/**
		* Get the node id
		* return The id
		*/
		const int GetId() const;

		/**
		* Get the node name
		* @return The node name
		*/
		const std::string & GetName() const;

		/**
		* Get the bounding box name
		* @return The bounding box name
		*/
		const std::string & GetBoundingBox() const;

		/**
		* Get an attribute
		* @param attributeName The attribute name
		* @return The attribute value or an empty string if the attribute is not found
		*/
		const std::string & GetAttribute(const std::string & attributeName);

		/**
		* Get the attributes
		* @return The attributes
		*/
		const std::map<std::string, std::string> & GetAttributes() const;

		/**
		* Get the local position
		* @return The local position
		*/
		const CVector3F & GetPosition() const;

		/**
		* Get the local rotation
		* @return The local rotation
		*/
		const CVector3F & GetRotation() const;

		/**
		* Get the local scale
		* @return The local scale
		*/
		const CVector3F & GetScale() const;

		/**
		* Get the absolute position
		* @return The absolute position
		*/
		const CVector3F & GetAbsolutePosition() const;

		/**
		* Get the local rotation matrix
		* @return The local rotation matrix
		*/
		const CMatrix & GetRotationMatrix() const;

		/**
		* Get the local transformation
		* @return The local transformation
		*/
		const CMatrix & GetTransformationMatrix() const;

		/**
		* Get the absolute transformation matrix
		* @return The absolute transformation matrix
		*/
		const CMatrix & GetAbsoluteTransformationMatrix() const;

		/**
		* Get the absolute rotation matrix
		* @return The absolute rotation matrix
		*/
		const CMatrix & GetAbsoluteRotationMatrix() const;

		/**
		* Get the absolute scale matrix
		* @return The absolute scale matrix
		*/
		const CMatrix & GetAbsoluteScaleMatrix() const;

		/**
		* Get the absolute translation matrix
		* @return The absolute translation matrix
		*/
		const CMatrix & GetAbsoluteTranslationMatrix() const;

		/**
		* Get the scene
		* @return The scene
		*/
		I3DScene * GetScene() const;

		/**
		* Get the node parent
		* @return The node parent, or nullptr
		*/
		const ISceneNode * GetParent() const;

		/**
		* Get the node children
		* @return The children
		*/
		const std::list<ISceneNode*> & GetChildren() const;
	};
}

#endif
