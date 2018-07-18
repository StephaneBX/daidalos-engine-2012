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

#ifndef __GAMEENGINE_CCINEMATICCAMERA_HPP
#define __GAMEENGINE_CCINEMATICCAMERA_HPP

#include <Scene/3D/ICameraSceneNode.hpp>
#include <Scene/3D/C3DScene.hpp>
#include <Core/CSoundContextManager.hpp>
#include <Core/Utility/CVector3.hpp>
#include "../3D/ICollidableEntity.hpp"

namespace daidalosgameengine {

	enum ETransitionType {
		TRANSITION_NONE,
		TRANSITION_INTERPOLATE
	};

	enum EFadeType {
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
		FADE_BOTH
	};

	struct SCinematicCameraStep {
		daidalosengine::CVector3F position;
		daidalosengine::CVector3F rotation;
		unsigned int transition;
		unsigned int fade;
		float fadeDuration;
		float speed;
	};

	/**
	* Cinematic type camera implementation
	*/
	class CCinematicCamera : public ICollidableEntity {
	private:
		bool m_enabled;
		bool m_done;
		float m_currentStep;
		std::vector<SCinematicCameraStep> m_steps;
		daidalosengine::ICameraSceneNode * m_node;
		daidalosengine::C3DScene * m_scene;
		daidalosengine::CSoundContextManager * m_soundContext;

	public:
		/**
		* Constructor
		* @param parent The parent
		* @param steps The steps
		*/
		CCinematicCamera(ILevel * parent, const std::vector<SCinematicCameraStep> & steps);

		/**
		* Set the player active state
		* @param enabled The player active state
		*/
		void SetEnabled(const bool enabled) override;

		/**
		* Start the cinematic
		*/
		void Start();

		/**
		* Pause the Cinematic
		*/
		void Pause();

		/**
		* Stop the Cinematic
		*/
		void Stop();

		/**
		* Update the camera
		* @param time Elapsed time
		*/
		void Update(const float time = 0) override;

		/**
		* Check if the player can see a shape
		* @param shape A shape
		* @return True if the player can see the shape
		*/
		bool IsShapeVisible(daidalosengine::IShape3DF * shape) const;

		/**
		* If the player interaction action is active
		* @return True if the player interaction action is active
		*/
		bool IsInteracting() const;

		/**
		* If the cinematic is done
		* @return True if the  cinematic is done
		*/
		bool IsDone() const;

		/**
		* Get the camera node
		* @return The camera node
		*/
		daidalosengine::ICameraSceneNode * GetCameraNode() const;

	protected:
		/**
		* Callback used to handle event when the entity fall and touch the ground
		* @param gravity The gravity value before touching the ground
		*/
		void OnEntityTouchedGround(const daidalosengine::CVector3F gravity) override;
	};
}

#endif
