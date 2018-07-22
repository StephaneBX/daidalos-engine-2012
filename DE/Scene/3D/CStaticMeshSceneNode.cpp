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

#include <algorithm>
#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/CStaticMeshSceneNode.hpp"

namespace daidalosengine {
	CStaticMeshSceneNode::CStaticMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename) : IMeshSceneNode(scene, parent, name) {
		LOG_TRACE();

		SetType(SCENE_NODE_STATICMESH);

		CStaticMeshSceneNode::SetMesh(filename);
		CStaticMeshSceneNode::SetMeshMaterial("default.mtl");
	}

	CStaticMeshSceneNode::CStaticMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale) : IMeshSceneNode(scene, parent, name, position, rotation, scale) {
		LOG_TRACE();

		SetType(SCENE_NODE_STATICMESH);

		CStaticMeshSceneNode::SetMesh(filename);
		CStaticMeshSceneNode::SetMeshMaterial("default.mtl");
	}

	CStaticMeshSceneNode::~CStaticMeshSceneNode() = default;

	/**
	* Set the mesh
	* @param filename The mesh filename
	*/
	void CStaticMeshSceneNode::SetMesh(const std::string & filename) {
		LOG_TRACE();

		SetFileName(filename);
		this->m_mesh.LoadFromFile(filename);

		InitializeGeometry(this->m_mesh.GetMesh()->GetNumberOfGroups());
	}

	void CStaticMeshSceneNode::Update(const float time) {
		LOG_TRACE();

		IMeshSceneNode::Update(time);

		// Update the activation shape
		const auto radius = GetAbsoluteTransformationMatrix().GetScale().Transform(CVector3F(this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth()));
		GetVisibilityShape()->SetPosition(GetAbsoluteTransformationMatrix().Transform(this->m_mesh.GetMesh()->GetVisibilityShape()->GetPosition()));
		GetVisibilityShape()->SetRadius(std::max(radius.x, std::max(radius.y, radius.z)));
	}

	std::string CStaticMeshSceneNode::GetMeshName() const {
		LOG_TRACE();

		std::string name;
		if(this->m_mesh.GetMesh() != nullptr) {
			name = this->m_mesh.GetMesh()->GetName();
		}

		return name;
	}

	void CStaticMeshSceneNode::FinalizeGeometry() {
		LOG_TRACE();

		this->m_mesh.FinalizeGeometry(GetMeshShadingType());
		for(auto i = 0; i < GetNumberOfGroups(); ++i) {
			PushMeshData(this->m_mesh.GetMesh(), i);
		}
		SetNeedGeometryUpdate(false);
	}
}
