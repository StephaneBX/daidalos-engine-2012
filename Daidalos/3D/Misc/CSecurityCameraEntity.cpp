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

#include <Scene/3D/CSpotLightSceneNode.hpp>
#include <Core/Utility/CMatrix.hpp>
#include <Level/ILevel.hpp>
#include <Enums.hpp>
#include "CSecurityCameraEntity.hpp"

#define DEFAULT_HORIZONTAL_ROTATION_SPEED 40.0F
#define DEFAULT_VERTICAL_ROTATION_SPEED 20.0F
#define DEFAULT_MAX_HORIZONTAL_ROTATION 0
#define DEFAULT_MAX_VERTICAL_ROTATION 10
#define DEFAULT_RANGE 60.0F

namespace daidalos {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The alarm node
	*/
	CSecurityCameraEntity::CSecurityCameraEntity(daidalosgameengine::ILevel * parent, daidalosengine::CStaticMeshSceneNode * meshNode) : m_meshRootNode(meshNode), IEntity(parent) {

		this->SetId(meshNode->GetId());
		this->SetType(daidalosgameengine::ENTITY_TYPE_SECURITY_CAMERA);
		this->m_meshCameraNode = nullptr;
		this->m_originalHorizontalRotation = this->m_meshRootNode->GetRotation().y;
		this->m_originalVerticalRotation = this->m_meshRootNode->GetRotation().x;
		this->m_horizontalRotationSpeed = DEFAULT_HORIZONTAL_ROTATION_SPEED;
		this->m_verticalRotationSpeed = DEFAULT_VERTICAL_ROTATION_SPEED;
		this->m_maxHorizontalRotation = DEFAULT_MAX_HORIZONTAL_ROTATION;
		this->m_maxVerticalRotation = DEFAULT_MAX_VERTICAL_ROTATION;
		daidalosengine::CColor lightColor = daidalosengine::CColor(1.0F, 1.0F, 1.0F);
		float range = DEFAULT_RANGE;

		// Get the camera node
		auto meshChildren = this->m_meshRootNode->GetChildren();
		if(!meshChildren.empty()) {
			this->m_meshCameraNode = dynamic_cast<daidalosengine::CStaticMeshSceneNode *>(meshChildren.front());
		}

		// Read the attributes
		if(!meshNode->GetAttribute("horizontal_rotation_speed").empty()) {
			std::istringstream ss(meshNode->GetAttribute("horizontal_rotation_speed"));
			ss >> this->m_horizontalRotationSpeed;
		}
		if(!meshNode->GetAttribute("vertical_rotation_speed").empty()) {
			std::istringstream ss(meshNode->GetAttribute("vertical_rotation_speed"));
			ss >> this->m_verticalRotationSpeed;
		}
		if(!meshNode->GetAttribute("max_horizontal_rotation").empty()) {
			std::istringstream ss(meshNode->GetAttribute("max_horizontal_rotation"));
			ss >> this->m_maxHorizontalRotation;
		}
		if(!meshNode->GetAttribute("max_vertical_rotation").empty()) {
			std::istringstream ss(meshNode->GetAttribute("max_vertical_rotation"));
			ss >> this->m_maxVerticalRotation;
		}
		if(!meshNode->GetAttribute("range").empty()) {
			std::istringstream ss(meshNode->GetAttribute("range"));
			ss >> range;
		}
		if(!meshNode->GetAttribute("light_color").empty()) {
			lightColor = daidalosengine::CColor::GetFrom(meshNode->GetAttribute("light_color"), lightColor);
		}

		// Add the light
		if(this->m_meshCameraNode != nullptr) {
			daidalosengine::CSpotLightSceneNode * spot = new daidalosengine::CSpotLightSceneNode(GetParent()->GetScene()->GetScene(), this->m_meshCameraNode, "", daidalosengine::CVector3F(0, 0, 2), daidalosengine::CVector3F(0, 0, 0), lightColor, range, 20, 0.7F, 0.8F, true, true);
			this->m_meshCameraNode->AddChild(spot);
		}
	}

	/**
	* Update the door
	* @param time Elapsed time
	*/
	void CSecurityCameraEntity::Update(const float time) {

		if (this->GetParent()->GetPlayer()->IsEnabled()) {
			UpdateHorizontalRotation(time);
			UpdateVerticalRotation(time);
		}
	}

	/**
	* Trigger the entity action
	*/
	void CSecurityCameraEntity::DoAction() {
		// Nothing to do
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	bool CSecurityCameraEntity::CanDoAction() const {
		return false;
	}

	/**
	* Update the security camera horizontal rotation
	*/
	void CSecurityCameraEntity::UpdateHorizontalRotation(const float time) {
		const float timePerSecond = time / 1000.0F;

		// Update the camera rotation so that it follows the player
		const float horizontalRotation = this->m_horizontalRotationSpeed * timePerSecond;

		// Check the camera - player angle
		daidalosengine::CVector3F cameraForward = daidalosengine::CVector3F(0, 0, 1.0F);
		const daidalosengine::CVector3F cameraToPlayer = (this->GetParent()->GetPlayer()->GetVisiblePosition() - this->m_meshRootNode->GetAbsolutePosition()).Normalize();
		cameraForward = this->m_meshRootNode->GetAbsoluteRotationMatrix().Transform(cameraForward, false).Normalize();

		// Horizontal rotation way
		const daidalosengine::CVector2F cameraToPlayerHorizontal = daidalosengine::CVector2F(cameraToPlayer.x, cameraToPlayer.z).Normalize();
		const daidalosengine::CVector2F cameraForwardHorizontal = daidalosengine::CVector2F(cameraForward.x, cameraForward.z).Normalize();
		float rotationY = this->m_meshRootNode->GetRotation().y;
		if(atan2f(cameraForwardHorizontal.y, cameraForwardHorizontal.x) - atan2f(cameraToPlayerHorizontal.y, cameraToPlayerHorizontal.x) > 0) {
			rotationY += horizontalRotation;
		} else {
			rotationY -= horizontalRotation;
		}

		// Boundary check
		if(rotationY > this->m_originalHorizontalRotation + this->m_maxHorizontalRotation) {
			rotationY = this->m_originalHorizontalRotation + this->m_maxHorizontalRotation;
		}

		if(rotationY < this->m_originalHorizontalRotation - this->m_maxHorizontalRotation) {
			rotationY = this->m_originalHorizontalRotation - this->m_maxHorizontalRotation;
		}

		// Update the camera rotation
		this->m_meshRootNode->SetRotation(daidalosengine::CVector3F(this->m_meshRootNode->GetRotation().x, rotationY, this->m_meshRootNode->GetRotation().z));
	}

	/**
	* Update the security camera vertical rotation
	*/
	void CSecurityCameraEntity::UpdateVerticalRotation(const float time) {
		const float timePerSecond = time / 1000.0F;

		// Update the camera rotation so that it follows the player
		const float verticalRotation = this->m_verticalRotationSpeed * timePerSecond;

		// Check the camera - player angle
		daidalosengine::CVector3F cameraForward = daidalosengine::CVector3F(0, 0, 1.0F);
		const daidalosengine::CVector3F cameraToPlayer = (this->GetParent()->GetPlayer()->GetVisiblePosition() - this->m_meshCameraNode->GetAbsolutePosition()).Normalize();
		cameraForward = this->m_meshCameraNode->GetAbsoluteRotationMatrix().Transform(cameraForward, false).Normalize();

		// Vertical rotation way
		const daidalosengine::CVector2F cameraToPlayerVertical = daidalosengine::CVector2F(cameraToPlayer.y, cameraToPlayer.z).Normalize();
		const daidalosengine::CVector2F cameraForwardVertical = daidalosengine::CVector2F(cameraForward.y, cameraForward.z).Normalize();
		float rotationX = this->m_meshCameraNode->GetRotation().x;
		if(atan2f(cameraForwardVertical.y, cameraForwardVertical.x) - atan2f(cameraToPlayerVertical.y, cameraToPlayerVertical.x) > 0) {
			rotationX -= verticalRotation;
		} else {
			rotationX += verticalRotation;
		}

		// Boundary check
		if(rotationX > this->m_originalVerticalRotation + this->m_maxVerticalRotation) {
			rotationX = this->m_originalVerticalRotation + this->m_maxVerticalRotation;
		}

		if(rotationX < this->m_originalVerticalRotation - this->m_maxVerticalRotation) {
			rotationX = this->m_originalVerticalRotation - this->m_maxVerticalRotation;
		}

		// Update the camera rotation
		this->m_meshCameraNode->SetRotation(daidalosengine::CVector3F(rotationX, this->m_meshCameraNode->GetRotation().y, this->m_meshCameraNode->GetRotation().z));
	}
}
