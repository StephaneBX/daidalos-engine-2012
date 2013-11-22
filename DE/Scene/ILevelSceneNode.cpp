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

#include "../Scene/ILevelSceneNode.h"

namespace DE
{
	/**
	* Set the mesh format
	* @param format The mesh format
	*/
	void ILevelSceneNode::SetMeshFormat(const int format)
	{
		this->m_levelBuffer.SetMeshFormat(format);
		ISceneNode::SetMeshFormat(format);
	}

	/**
	* Set the mesh shading type
	* @param shadingType The mesh shading type
	*/
	void ILevelSceneNode::SetMeshShadingType(EMeshShadingType shadingType)
	{
		this->m_shadingType = shadingType;

		ISceneNode::SetMeshShadingType(shadingType);
	}

	/**
	* Set the ambient color of the level
	* @param ambientColor The ambient color
	*/
	void ILevelSceneNode::SetAmbientColor(const CVector4F & ambientColor)
	{
		this->m_ambientColor = ambientColor;
	}

	/**
	* Set the sky name
	* @param name The sky name
	*/
	void ILevelSceneNode::SetSkyName(const std::string & name)
	{
		this->m_skyName = name;
	}

	/**
	* Set the sky ambient color
	* @param ambientColor The sky ambient color
	*/
	void ILevelSceneNode::SetSkyAmbientColor(const CVector4F & ambientColor)
	{
		this->m_skyAmbientColor = ambientColor;
	}

	/**
	* Get the number of visible triangles
	* @return The number of visible triangles
	*/
	const int ILevelSceneNode::GetNumberOfVisibleTriangles() const
	{
		return this->m_numberOfVisibleTriangles;
	}

	/**
	* Get the ambient color
	* @return The ambient color
	*/
	const CVector4F & ILevelSceneNode::GetAmbientColor() const
	{
		return this->m_ambientColor;
	}

	/**
	* Get the sky name
	* @return The sky name
	*/
	const std::string & ILevelSceneNode::GetSkyName() const
	{
		return this->m_skyName;
	}

	/**
	* Get the sky ambient color
	* @return The sky ambient color
	*/
	const CVector4F & ILevelSceneNode::GetSkyAmbientColor() const
	{
		return this->m_skyAmbientColor;
	}

	/**
	* Get the bounding box
	* @return The bounding box
	*/
	const CAABoundingBoxF ILevelSceneNode::GetBoundingBox()
	{
		return this->m_boundingBox;
	}
}
