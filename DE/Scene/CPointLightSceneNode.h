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

#ifndef __CPOINTLIGHTSCENENODE_H
#define __CPOINTLIGHTSCENENODE_H

#include <array>
#include "../Core/Utility/CVector3.h"
#include "../Core/Utility/CColor.h"
#include "../Scene/CSphere.h"
#include "../Scene/IBoundedLightSceneNode.h"
#include "../Scene/CProjectionCameraSceneNode.h"

namespace DE
{
	/**
	* Handle a point light
	*/
	class CPointLightSceneNode : public IBoundedLightSceneNode
	{
	protected:
		std::array<CProjectionCameraSceneNode*, 6> m_cameras;

	public:
		/**
		* Constructor
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		*/
		CPointLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position);

		/**
		* Constructor
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param color The color
		* @param range The cone range (distance)
		* @param falloff The cone falloff
		* @param shadowCaster If the light cast shadows
		* @param enabled If the light is enabled by default
		*/
		CPointLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CColor & color, const float range, const float falloff, const bool shadowCaster, const bool enabled = true);

		/**
		* Destructor
		*/
		~CPointLightSceneNode();

		/**
		* Update the light
		* @param time The elapsed time
		*/
		void Update(const float time = 0);
	};
}

#endif
