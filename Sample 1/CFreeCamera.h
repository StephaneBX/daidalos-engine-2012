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

#ifndef __CFREECAMERA_H
#define __CFREECAMERA_H

#include <Scene/ICameraSceneNode.h>
#include <Scene/CSceneManager.h>
#include <Scene/CEllipse.h>
#include <Core/Utility/CVector3.h>
#include <Core/CInputDevice.h>
#include <Core/CSoundContextManager.h>

namespace SAMPLE1 
{
	enum EFPSCameraMovement
	{
		FPS_CAMERA_EXIT			= 1 << 1,
		FPS_CAMERA_STRAFELEFT	= 1 << 2,
		FPS_CAMERA_STRAFERIGHT	= 1 << 3,
		FPS_CAMERA_FORWARD		= 1 << 4,
		FPS_CAMERA_BACKWARD		= 1 << 5,
		FPS_CAMERA_LOOKUP		= 1 << 6,
		FPS_CAMERA_LOOKDOWN		= 1 << 7,
		FPS_CAMERA_LOOKLEFT		= 1 << 8,
		FPS_CAMERA_LOOKRIGHT	= 1 << 9
	};

	/**
	* Free type camera implementation
	*/
	class CFreeCamera
	{
	private:
		unsigned int m_state;
		float m_smooth;
		DE::CVector2F m_previousMouseMovement;
		DE::CVector3F m_rotation;
		DE::ICameraSceneNode * m_node;
		DE::CInputDevice * m_inputDevice;
		DE::CEllipseF m_shape;
		DE::CSceneManager * m_scene;
		DE::CSoundContextManager * m_soundContext;

	public:
		/**
		* @param cameraNode The camera node
		* @param smooth The smooth view factor
		*/
		CFreeCamera(DE::ICameraSceneNode * cameraNode, const float smooth);

		/**
		* Update the camera
		* @param time Elapsed time
		*/
		void Update(float const time = 0);

		/**
		* Get the camera node
		* @return The camera node
		*/
		DE::ICameraSceneNode * GetCameraNode() const;

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
