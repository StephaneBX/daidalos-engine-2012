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

#ifndef __CPROJECTIONCAMERASCENENODE_H
#define __CPROJECTIONCAMERASCENENODE_H

#include "../Core/Utility/CMatrix.h"
#include "../Core/Utility/CVector3.h"
#include "../Scene/ICameraSceneNode.h"

namespace DE {
	/**
	* Handle a projection typed camera
	*/
	class CProjectionCameraSceneNode : public ICameraSceneNode {
	private:
		float m_angle;
		float m_ratio;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param position The position
		* @param rotation The rotation
		* @param angle The vertical view angle
		* @param ratio The view ratio
		* @param nearDistance The near distance
		* @param farDistance The far distance
		*/
		CProjectionCameraSceneNode(IScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const float angle, const float ratio, const float nearDistance, const float farDistance);

		/**
		* Destructor
		*/
		~CProjectionCameraSceneNode();

		/**
		* Set the angle in radian
		* @param angle The angle in radian
		*/
		void SetAngle(const float angle);

		/**
		* Set the ratio
		* @param ratio The ratio
		*/
		void SetRatio(const float ratio);

		/**
		* Update the camera
		* @param time The elapsed time
		*/
		void Update(const float time = 0);

		/**
		* Get the angle
		* @return The angle in radian
		*/
		const float GetAngle() const;

		/**
		* Get the ratio
		* @return The ratio
		*/
		const float GetRatio() const;

	private:
		CProjectionCameraSceneNode();
		CProjectionCameraSceneNode(const CProjectionCameraSceneNode & copy);
	};
}

#endif
