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

#ifndef __CVIEWCAMERA_HPP
#define __CVIEWCAMERA_HPP

#include <Scene/3D/ICameraSceneNode.hpp>
#include <Scene/3D/C3DScene.hpp>
#include <Scene/3D/CEllipse.hpp>
#include <Core/Utility/CVector3.hpp>
#include <Core/CInputDevice.hpp>
#include <Core/CSoundContextManager.hpp>
#include <SiniP/SiniP.h>

namespace daidaloseditor {
	/**
	* Viewer camera
	*/
	class CViewCamera {
	private:
		unsigned int m_state;
		daidalosengine::CVector2F m_previousMouseMovement;
		daidalosengine::CVector3F m_rotation;
		daidalosengine::ICameraSceneNode * m_node;
		daidalosengine::CInputDevice * m_inputDevice;
		daidalosengine::CEllipseF m_shape;
		daidalosengine::C3DScene * m_scene;
		daidalosengine::CSoundContextManager * m_soundContext;
		SiniP m_options;

	public:
		/**
		* @param cameraNode The camera node
		* @param smooth The smooth view factor
		*/
		explicit CViewCamera(daidalosengine::ICameraSceneNode * cameraNode);

		/**
		* Set the associated scene
		* @param scene The associated scene
		*/
		void SetScene(daidalosengine::C3DScene * scene);

		/**
		* Set the camera position
		* @param position The position
		*/
		void SetPosition(const daidalosengine::CVector3F & position);

		/**
		* Set the camera rotation
		* @param rotation The rotation
		*/
		void SetRotation(const daidalosengine::CVector3F & rotation);

		/**
		* Update the camera
		* @param time Elapsed time
		*/
		void Update(const float time = 0);

		/**
		* Get the camera node
		* @return The camera node
		*/
		daidalosengine::ICameraSceneNode * GetCameraNode() const;

	private:
		/**
		* Update the state
		*/
		void UpdateState();

		/**
		* Update the movement
		* @param timeSecond Elapsed time
		*/
		void UpdateMovement(const float timeSecond);

		/**
		* Update the view
		* @param timeSecond Elapsed time
		*/
		void UpdateView(const float timeSecond);
	};
}

#endif
