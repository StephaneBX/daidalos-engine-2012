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

#include "../Core/Core.h"
#include "../Scene/ISceneNode.h"

namespace DE
{
	ISceneNode::ISceneNode(ISceneNode * parent, const std::string & name) 
	{
		this->m_parent = nullptr;
		if(parent != nullptr && parent->GetType() != SCENE_NODE_ROOT)
			this->m_parent = parent;

		this->m_name = name;
		this->m_type = SCENE_NODE_ROOT;
		this->m_localPosition = CVector3F(0,0,0);
		this->m_localRotation = CVector3F(0,0,0);
		this->m_localScale = CVector3F(1,1,1);
		this->m_active = true;
		this->m_enabled = true;
	}

	ISceneNode::ISceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation)
	{
		this->m_parent = nullptr;
		if(parent != nullptr && parent->GetType() != SCENE_NODE_ROOT)
			this->m_parent = parent;

		this->m_name = name;
		this->m_type = SCENE_NODE_ROOT;
		this->m_localPosition = position;
		this->m_localRotation = rotation;
		this->m_localScale = CVector3F(1,1,1);
		this->m_active = true;
		this->m_enabled = true;
	}

	
	ISceneNode::~ISceneNode() 
	{
		for(auto it = this->m_children.begin() ; it != this->m_children.end() ; ++it)
			SAFE_DELETE(*it);
	}

	void ISceneNode::AddChild(ISceneNode * child)
	{
		this->m_children.push_back(child);
	}

	void ISceneNode::SetAttributes(const std::map<std::string, std::string> & attributes)
	{
		this->m_attributes = attributes;
	}

	void ISceneNode::AddAttribute(const std::string & attributeName, const std::string attributeValue)
	{
		this->m_attributes[attributeName] = attributeValue;
	}

	void ISceneNode::SetEnabled(const bool enabled)
	{
		this->m_enabled = enabled;
	}

	void ISceneNode::SetPosition(const CVector3F & position)
	{
		this->m_localPosition = position;
	}

	void ISceneNode::SetRotation(const CVector3F & rotation)
	{
		this->m_localRotation = rotation;
	}

	void ISceneNode::SetScale(const CVector3F & scale)
	{
		this->m_localScale = scale;
	}

	void ISceneNode::Update(const float time)
	{
		CVector3F rot = CVector3F(DegreeToRadian(this->m_localRotation.x), DegreeToRadian(this->m_localRotation.y), DegreeToRadian(this->m_localRotation.z));
		this->m_localPositionMatrix = this->m_localPositionMatrix.SetTranslation(this->m_localPosition.x, this->m_localPosition.y, this->m_localPosition.z);
		this->m_localRotationMatrix = CMatrix(CVector3F(1,1,1), rot, CVector3F(0,0,0));
		this->m_localScaleMatrix = this->m_localScaleMatrix.SetScale(this->m_localScale.x, this->m_localScale.y, this->m_localScale.z);
		this->m_localMatrix = this->m_localScaleMatrix * this->m_localRotationMatrix * this->m_localPositionMatrix;

		// Check the existence of a parent
		if(this->m_parent != nullptr)
		{
			// The node has a parent
			this->m_absoluteRotationMatrix = this->m_localRotationMatrix * this->m_parent->GetAbsoluteRotationMatrix();
			this->m_absoluteScaleMatrix = this->m_localScaleMatrix * this->m_parent->GetAbsoluteScaleMatrix();

			this->m_absolutePosition = this->m_parent->GetAbsolutePosition() + this->m_absoluteRotationMatrix.Transform(this->m_localPosition);
			this->m_absolutePositionMatrix = this->m_absolutePositionMatrix.SetTranslation(this->m_absolutePosition.x, this->m_absolutePosition.y, this->m_absolutePosition.z);

			this->m_worldMatrix = this->m_absoluteScaleMatrix * this->m_absoluteRotationMatrix * this->m_absolutePositionMatrix;
		}
		else
		{
			// The node is a root
			this->m_absoluteRotationMatrix = this->m_localRotationMatrix;
			this->m_absoluteScaleMatrix = this->m_localScaleMatrix;

			this->m_absolutePosition = this->m_localPosition;
			this->m_absolutePositionMatrix = this->m_localPositionMatrix;

			this->m_worldMatrix = this->m_localMatrix;
		}

		// Update the children
		for(auto it = this->m_children.begin() ; it != this->m_children.end() ; ++it)
			(*it)->Update(time);
	}

	void ISceneNode::SetMeshFormat(const int format)
	{
		// Set the mesh format for the children
		for(auto it = this->m_children.begin() ; it != this->m_children.end() ; ++it)
			(*it)->SetMeshFormat(format);
	}

	void ISceneNode::SetMeshShadingType(EMeshShadingType shadingType)
	{
		// Set the mesh shading type for the children
		for(auto it = this->m_children.begin() ; it != this->m_children.end() ; ++it)
			(*it)->SetMeshShadingType(shadingType);
	}

	void ISceneNode::Render()
	{
		// Render the children
		for(auto it = this->m_children.begin() ; it != this->m_children.end() ; ++it)
			(*it)->Render();
	}

	ESceneNodeType ISceneNode::GetType() const
	{
		return this->m_type;
	}

	const bool ISceneNode::IsCollidable() const
	{
		return false;
	}

	const bool ISceneNode::IsEnabled() const
	{
		return this->m_enabled;
	}

	const bool ISceneNode::IsActive() const
	{
		return this->m_active;
	}

	const bool ISceneNode::RemoveChild(ISceneNode * child)
	{
		return true;
	}

	const std::string & ISceneNode::GetName() const
	{
		return this->m_name;
	}

	const CVector3F & ISceneNode::GetPosition() const
	{
		return this->m_localPosition;
	}

	const std::string & ISceneNode::GetAttribute(const std::string & attributeName)
	{
		return this->m_attributes[attributeName];
	}

	const CVector3F & ISceneNode::GetRotation() const
	{
		return this->m_localRotation;
	}

	const CVector3F & ISceneNode::GetScale() const
	{
		return this->m_localScale;
	}

	const CVector3F & ISceneNode::GetAbsolutePosition() const
	{
		return this->m_absolutePosition;
	}

	const CMatrix & ISceneNode::GetTransformationMatrix() const
	{
		return this->m_localMatrix;
	}

	const CMatrix & ISceneNode::GetAbsoluteTransformationMatrix() const
	{
		return this->m_worldMatrix;
	}

	const CMatrix & ISceneNode::GetAbsoluteRotationMatrix() const
	{
		return this->m_absoluteRotationMatrix;
	}

	const CMatrix & ISceneNode::GetAbsoluteScaleMatrix() const
	{
		return this->m_absoluteScaleMatrix;
	}

	const CMatrix & ISceneNode::GetAbsoluteTranslationMatrix() const
	{
		return this->m_absolutePositionMatrix;
	}

	const std::list<ISceneNode*> & ISceneNode::GetChildren() const
	{
		return this->m_children;
	}
}
