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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/CAnimatedMeshSceneNode.hpp"

namespace DE {
	CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename) : IMeshSceneNode(scene, parent, name) {
		LOG_TRACE();

		this->m_looped = false;
		this->m_type = SCENE_NODE_ANIMATEDMESH;
		this->m_buffer = nullptr;
		this->m_diffuseTexture = nullptr;
		this->m_normalTexture = nullptr;

		SetMesh(filename);
		SetMeshMaterial("default.mtl");

		this->m_animationStart = 0;
		this->m_animationEnd = 0;
		this->m_animationSpeed = 0;
		this->m_animationCurrent = 0;
		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;

		this->m_mesh.Animate(0);
	}

	CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale) : IMeshSceneNode(scene, parent, name, position, rotation, scale) {
		LOG_TRACE();

		this->m_looped = false;
		this->m_type = SCENE_NODE_ANIMATEDMESH;
		this->m_buffer = nullptr;
		this->m_diffuseTexture = nullptr;
		this->m_normalTexture = nullptr;
		
		SetMesh(filename);
		SetMeshMaterial("default.mtl");

		this->m_animationStart = 0;
		this->m_animationEnd = 0;
		this->m_animationSpeed = 0;
		this->m_animationCurrent = 0;
		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;

		this->m_mesh.Animate(0);
	}

	CAnimatedMeshSceneNode::~CAnimatedMeshSceneNode() {

	}

	void CAnimatedMeshSceneNode::SetMesh(const std::string filename) {
		LOG_TRACE();

		SAFE_DELETE_ARRAY(this->m_buffer);
		SAFE_DELETE_ARRAY(this->m_diffuseTexture);
		SAFE_DELETE_ARRAY(this->m_normalTexture);

		this->m_filename = filename;
		this->m_mesh.LoadFromFile(filename);
		this->m_numberOfGroups = this->m_mesh.GetMesh()->GetNumberOfGroups();

		this->m_buffer = new CMeshBuffer[this->m_numberOfGroups];
		for(int i = 0; i < this->m_numberOfGroups; i++) {
			this->m_buffer[i].SetScene(this->m_scene);
		}

		this->m_diffuseTexture = new CTexture2D[this->m_numberOfGroups];
		this->m_normalTexture = new CTexture2D[this->m_numberOfGroups];
	}

	void CAnimatedMeshSceneNode::SetLooped(const bool looped) {
		LOG_TRACE();

		this->m_looped = looped;
	}

	void CAnimatedMeshSceneNode::SetStartFrame(const float startFrame) {
		LOG_TRACE();

		this->m_animationStart = startFrame;
	}

	void CAnimatedMeshSceneNode::SetEndFrame(const float endFrame) {
		LOG_TRACE();

		this->m_animationEnd = endFrame;
	}

	void CAnimatedMeshSceneNode::SetCurrentFrame(const float currentFrame) {
		LOG_TRACE();

		this->m_animationCurrent = currentFrame;
	}

	void CAnimatedMeshSceneNode::SetAnimationSpeed(const float speed) {
		LOG_TRACE();

		this->m_animationSpeed = speed;
	}

	void CAnimatedMeshSceneNode::StartAnimation(const float startFrame, const float endFrame, const float speed) {
		LOG_TRACE();

		this->m_animationStart = startFrame;
		this->m_animationEnd = endFrame;
		this->m_animationSpeed = speed;
		this->m_animationCurrent = startFrame;
		this->m_animationState = MESH_ANIMATION_STATE_PLAYING;
	}

	void CAnimatedMeshSceneNode::PauseAnimation() {
		LOG_TRACE();

		this->m_animationState = MESH_ANIMATION_STATE_PAUSED;
	}

	void CAnimatedMeshSceneNode::StopAnimation() {
		LOG_TRACE();

		this->m_animationState = MESH_ANIMATION_STATE_STOPPED;
	}

	void CAnimatedMeshSceneNode::ResumeAnimation() {
		LOG_TRACE();

		this->m_animationState = MESH_ANIMATION_STATE_PLAYING;
	}

	void CAnimatedMeshSceneNode::Update(const float time) {
		LOG_TRACE();

		IMeshSceneNode::Update(time);

		// Animation update
		if(this->m_animationState == MESH_ANIMATION_STATE_PLAYING && time > 0) {
			this->m_animationCurrent += (this->m_animationSpeed * time) / 1000.0F;
			if(this->m_animationCurrent >= this->m_animationEnd) {
				if(this->m_looped) {
					this->m_animationCurrent = (this->m_animationCurrent - this->m_animationEnd) + this->m_animationStart;
				} else {
					this->m_animationCurrent = this->m_animationEnd;
					StopAnimation();
				}
			}
		}
		this->m_needGeometryUpdate = true;

		// Update the activation shape
		this->m_visibilityShape->SetPosition(this->m_worldMatrix.Transform(this->m_mesh.GetMesh()->GetVisibilityShape()->GetPosition()));
		CVector3F radius = this->m_worldMatrix.GetScale().Transform(CVector3F(this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth(), this->m_mesh.GetMesh()->GetVisibilityShape()->GetInnerDepth()));
		this->m_visibilityShape->SetRadius(std::max(radius.x, std::max(radius.y, radius.z)));
	}

	const std::string CAnimatedMeshSceneNode::GetMeshName() const {
		LOG_TRACE();

		std::string name = "";
		if(this->m_mesh.GetMesh() != nullptr) {
			name = this->m_mesh.GetMesh()->GetName();
		}

		return name;
	}

	const int CAnimatedMeshSceneNode::GetNumberOfFrames() const {
		LOG_TRACE();

		return this->m_mesh.GetNumberOfFrames();
	}

	EMeshAnimationState CAnimatedMeshSceneNode::GetAnimationState() const {
		LOG_TRACE();

		return this->m_animationState;
	}

	void CAnimatedMeshSceneNode::FinalizeGeometry() {
		LOG_TRACE();

		this->m_mesh.Animate(this->m_animationCurrent);
		this->m_mesh.FinalizeGeometry(this->m_shadingType);
		for(int i = 0; i < this->m_numberOfGroups; ++i)
			this->m_buffer[i].Transform(this->m_mesh.GetMesh()->GetVertices(i), this->m_mesh.GetMesh()->GetTexcoords(i), this->m_mesh.GetMesh()->GetNormals(i), this->m_mesh.GetMesh()->GetTangents(i), this->m_mesh.GetMesh()->GetBinormals(i), this->m_mesh.GetMesh()->GetGroupSize(i));
		this->m_needGeometryUpdate = false;
	}
}
