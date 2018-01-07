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

#include <limits>
#include <Core/IRenderer.hpp>
#include <Scene/3D/CProjectionCameraSceneNode.hpp>
#include <Scene/3D/CEllipse.hpp>
#include "../Level/ILevel.hpp"
#include "CCinematicCamera.hpp"

namespace GE {
	/**
	* Constructor
	* @param parent The parent
	* @param steps The steps
	*/
	CCinematicCamera::CCinematicCamera(ILevel * parent, const std::vector<SCinematicCameraStep> & steps) : ICollidableEntity(parent) {

		DE::IRenderer * renderer = DE::IRenderer::GetRenderer();

		this->m_soundContext = DE::CSoundContextManager::GetInstance();

		this->m_enabled = false;
		this->m_done = false;
		this->m_currentStep = 0;
		this->m_steps = steps;
		this->m_node = new DE::CProjectionCameraSceneNode(parent->GetScene()->GetScene(), parent->GetScene()->GetRootNode(), "cinematic_camera", DE::CVector3F(), DE::CVector3F(), DE::DegreeToRadian(55.0f), renderer->GetResolution().x / (float)renderer->GetResolution().y, 0.1f, 100.0f);
		parent->GetScene()->GetRootNode()->AddChild(this->m_node);

		this->SetCollisionState(false);
		this->SetCollisionShape(new DE::CEllipseF(this->m_node->GetPosition() - DE::CVector3F(0, 1, 0), DE::CVector2F(1, 1)));
	}

	/**
	* Set the player active state
	* @param enabled The player active state
	*/
	void CCinematicCamera::SetEnabled(const bool enabled) {
		this->m_enabled = enabled;
	}

	/**
	* Start the cinematic
	*/
	void CCinematicCamera::Start() {
		SetEnabled(true);
	}

	/**
	* Pause the Cinematic
	*/
	void CCinematicCamera::Pause() {
		SetEnabled(false);
	}

	/**
	* Stop the Cinematic
	*/
	void CCinematicCamera::Stop() {
		SetEnabled(false);
		this->m_currentStep = 0;
		this->m_done = false;
	}

	void CCinematicCamera::Update(const float time) {
		float timeSecond = time / 1000.0f;
		
		if(this->m_enabled) {
			DE::CVector3F position;
			DE::CVector3F rotation;

			// Update the step
			this->m_currentStep += this->m_steps[(int)this->m_currentStep].speed * timeSecond;
			if (this->m_currentStep >= this->m_steps.size() - 1) {
				position = this->m_steps.back().position;
				rotation = this->m_steps.back().rotation;

				if (this->m_currentStep >= this->m_steps.size()) {
					this->m_done = true;
					this->m_enabled = false;
				}

			} else {

				// Depending on the transition type
				if (this->m_steps[(int)this->m_currentStep].transition == TRANSITION_NONE) {
					position = this->m_steps[(int)this->m_currentStep].position;
					rotation = this->m_steps[(int)this->m_currentStep].rotation;

				} else if (this->m_steps[(int)this->m_currentStep].transition == TRANSITION_INTERPOLATE) {
					float diff = this->m_currentStep - (int)this->m_currentStep;

					// Position interpolation - Splines
					DE::CVector3F y0 = this->m_steps[(int)this->m_currentStep].position;
					DE::CVector3F y1 = this->m_steps[(int)this->m_currentStep].position;
					DE::CVector3F y2 = this->m_steps[(int)this->m_currentStep + 1].position;
					DE::CVector3F y3 = this->m_steps[(int)this->m_currentStep + 1].position;

					if (this->m_currentStep >= 1) {
						y0 = this->m_steps[(int)this->m_currentStep - 1].position;
					}
					if ((this->m_currentStep + 2) <= this->m_steps.size() - 1) {
						y3 = this->m_steps[(int)this->m_currentStep + 2].position;
					}
					position = DE::CubicInterpolate<DE::CVector3F>(y0, y1, y2, y3, diff);

					// Rotation interpolation - Cosine
					float interpolation = (1 - cos(diff * PI)) / 2.0f;
					rotation = (this->m_steps[(int)this->m_currentStep].rotation * (1 - interpolation) + this->m_steps[(int)this->m_currentStep + 1].rotation * interpolation);
				}
			}

			// Update the fade value
			GetParent()->SetBrightness(0);
			if (this->m_steps[(int)this->m_currentStep].fade != FADE_NONE) {
				float diff = this->m_currentStep - (int)this->m_currentStep;
				float fadeDiff = this->m_steps[(int)this->m_currentStep].speed * (this->m_steps[(int)this->m_currentStep].fadeDuration / 1000.0f);

				if (diff < fadeDiff && (this->m_steps[(int)this->m_currentStep].fade == FADE_IN || this->m_steps[(int)this->m_currentStep].fade == FADE_BOTH)) {
					// Fade in
					GetParent()->SetBrightness(-1.0f + (diff / fadeDiff));
				}

				if (diff >= 1.0f - fadeDiff && (this->m_steps[(int)this->m_currentStep].fade == FADE_OUT || this->m_steps[(int)this->m_currentStep].fade == FADE_BOTH)) {
					// Fade out
					GetParent()->SetBrightness(-(diff - (1.0f - fadeDiff)) / fadeDiff);
				}
			}

			// Update the node
			this->m_node->SetPosition(position);
			this->m_node->SetRotation(rotation);

			// Update the listener
			this->m_soundContext->SetListenerPosition(position);
			float orientation[] = { this->m_node->GetForwardVector().x, this->m_node->GetForwardVector().y, this->m_node->GetForwardVector().z, this->m_node->GetUpVector().x, this->m_node->GetUpVector().y, this->m_node->GetUpVector().z };
			this->m_soundContext->SetListenerOrientation(orientation);
		}
	}

	DE::ICameraSceneNode * CCinematicCamera::GetCameraNode() const {
		return this->m_node;
	}

	/**
	* Check if the player can see a shape
	* @param shape A shape
	* @return True if the player can see the shape
	*/
	const bool CCinematicCamera::IsShapeVisible(DE::IShape3DF * shape) {
		return !this->m_node->FrustumCull(shape);
	}

	/**
	* If the player interaction action is active
	* @return True if the player interaction action is active
	*/
	const bool CCinematicCamera::IsInteracting() const {
		return false;
	}

	/**
	* If the cinematic is done
	* @return True if the  cinematic is done
	*/
	const bool CCinematicCamera::IsDone() const {
		return this->m_done;
	}

	/**
	* Callback used to handle event when the entity fall and touch the ground
	* @param gravity The gravity value before touching the ground
	*/
	void CCinematicCamera::OnEntityTouchedGround(const DE::CVector3F gravity) {
		// Nothing to do
	}
}
