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

#include "../../Scene/3D/I3DScene.h"
#include "../../Scene/3D/CAnimatedMeshSceneNode.h"

namespace DE {
	CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename) : IMeshSceneNode(scene, parent, name) {
		this->m_type = SCENE_NODE_ANIMATEDMESH;
		this->m_filename = filename;
		this->m_mesh.LoadFromFile(filename);
		this->m_numberOfGroups = this->m_mesh.GetMesh()->GetNumberOfGroups();

		this->m_buffer = new CMeshBuffer[this->m_numberOfGroups];
		for(int i = 0; i < this->m_numberOfGroups; i++) {
			this->m_buffer[i].SetScene(scene);
		}

		this->m_diffuseTexture = new CTexture2D[this->m_numberOfGroups];
		this->m_normalTexture = new CTexture2D[this->m_numberOfGroups];

		SetMeshMaterial("default.mtl");

		this->m_animationStart = 0;
		this->m_animationEnd = 0;
		this->m_animationSpeed = 0;
		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;

		this->m_mesh.Animate(0);
	}

	CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale) : IMeshSceneNode(scene, parent, name, position, rotation, scale) {
		this->m_type = SCENE_NODE_ANIMATEDMESH;
		this->m_filename = filename;
		this->m_mesh.LoadFromFile(filename);
		this->m_numberOfGroups = this->m_mesh.GetMesh()->GetNumberOfGroups();

		this->m_buffer = new CMeshBuffer[this->m_numberOfGroups];
		for(int i = 0; i < this->m_numberOfGroups; i++) {
			this->m_buffer[i].SetScene(scene);
		}

		this->m_diffuseTexture = new CTexture2D[this->m_numberOfGroups];
		this->m_normalTexture = new CTexture2D[this->m_numberOfGroups];

		SetMeshMaterial("default.mtl");

		this->m_animationStart = 0;
		this->m_animationEnd = 0;
		this->m_animationSpeed = 0;
		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;

		this->m_mesh.Animate(0);
	}

	void CAnimatedMeshSceneNode::StartAnimation(const float startFrame, const float endFrame, const float speed) {
		this->m_animationStart = startFrame;
		this->m_animationEnd = endFrame;
		this->m_animationSpeed = speed;
		this->m_animationState = MESH_ANIMATION_STATE_PLAYING;
	}

	void CAnimatedMeshSceneNode::PauseAnimation() {
		this->m_animationState = MESH_ANIMATION_STATE_PAUSED;
	}

	void CAnimatedMeshSceneNode::StopAnimation() {
		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;
	}

	void CAnimatedMeshSceneNode::Update(const float time) {
		IMeshSceneNode::Update(time);

		// Animation update
		if(this->m_animationState == MESH_ANIMATION_STATE_PLAYING && time > 0) {
			this->m_animationStart += (this->m_animationSpeed * time) / 1000.0f;
			if(this->m_animationStart >= this->m_animationEnd)
				StopAnimation();

			this->m_mesh.Animate(this->m_animationStart);
			this->m_needGeometryUpdate = true;
		}

		// Update the activation shape
		if(this->m_needVisibilityShapeUpdate) {
			this->m_visibilityShape->SetPosition(this->m_worldMatrix.Transform(this->m_mesh.GetMesh()->GetVisibilityShape()->GetPosition()));
			CVector3F radius = this->m_worldMatrix.GetScale().Transform(CVector3F(this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth()));

			this->m_visibilityShape->SetRadius(std::max(radius.x, std::max(radius.y, radius.z)));
			this->m_needVisibilityShapeUpdate = false;
		}
	}

	const int CAnimatedMeshSceneNode::GetNumberOfFrames() const {
		return this->m_mesh.GetNumberOfFrames();
	}

	EMeshAnimationState CAnimatedMeshSceneNode::GetAnimationState() const {
		return this->m_animationState;
	}

	void CAnimatedMeshSceneNode::FinalizeGeometry() {
		this->m_mesh.FinalizeGeometry(this->m_shadingType);
		for(int i = 0; i < this->m_numberOfGroups; ++i)
			this->m_buffer[i].Transform(this->m_mesh.GetMesh()->GetVertices(i), this->m_mesh.GetMesh()->GetTexcoords(i), this->m_mesh.GetMesh()->GetNormals(i), this->m_mesh.GetMesh()->GetTangents(i), this->m_mesh.GetMesh()->GetBinormals(i), this->m_mesh.GetMesh()->GetGroupSize(i));
		this->m_needGeometryUpdate = false;
	}
}
