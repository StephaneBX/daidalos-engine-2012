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

#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/ILevelSceneNode.h"

namespace DE {

	ILevelSceneNode::ILevelSceneNode(I3DScene * scene) : ISceneNode(scene) {
		LOG_TRACE();

		this->m_levelBuffer.SetScene(scene);
		this->m_useGlobalAmbientColor = false;
	}

	void ILevelSceneNode::SetUseGlobalAmbientColor(const bool useGlobalAmbientColor) {
		this->m_useGlobalAmbientColor = useGlobalAmbientColor;
	}

	void ILevelSceneNode::SetMeshFormat(const int format) {
		LOG_TRACE();

		this->m_levelBuffer.SetMeshFormat(format);
		ISceneNode::SetMeshFormat(format);
	}

	void ILevelSceneNode::SetMeshShadingType(EMeshShadingType shadingType) {
		LOG_TRACE();

		this->m_shadingType = shadingType;

		ISceneNode::SetMeshShadingType(shadingType);
	}

	void ILevelSceneNode::SetAmbientColor(const CVector4F & ambientColor) {
		LOG_TRACE();

		this->m_ambientColor = ambientColor;
	}

	void ILevelSceneNode::SetSkyName(const std::string & name) {
		LOG_TRACE();

		this->m_skyName = name;
	}

	void ILevelSceneNode::SetSkyAmbientColor(const CVector4F & ambientColor) {
		LOG_TRACE();

		this->m_skyAmbientColor = ambientColor;
	}

	const bool ILevelSceneNode::GetUseGlobalAmbientColor() const {
		return this->m_useGlobalAmbientColor;
	}

	const int ILevelSceneNode::GetNumberOfVisibleTriangles() const {
		LOG_TRACE();

		return this->m_numberOfVisibleTriangles;
	}

	const CVector4F & ILevelSceneNode::GetAmbientColor() const {
		LOG_TRACE();

		return this->m_ambientColor;
	}

	const std::string & ILevelSceneNode::GetSkyName() const {
		LOG_TRACE();

		return this->m_skyName;
	}

	const CVector4F & ILevelSceneNode::GetSkyAmbientColor() const {
		LOG_TRACE();

		return this->m_skyAmbientColor;
	}

	const CAABoundingBoxF ILevelSceneNode::GetBoundingBox() {
		LOG_TRACE();

		return this->m_boundingBox;
	}
}
