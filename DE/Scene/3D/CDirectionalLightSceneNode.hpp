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

#ifndef __CDIRECTIONALLIGHTSCENENODE_HPP
#define __CDIRECTIONALLIGHTSCENENODE_HPP

#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CColor.hpp"
#include "../../Scene/3D/CSphere.hpp"
#include "../../Scene/3D/ILightSceneNode.hpp"
#include "../../Scene/3D/COrthographicCameraSceneNode.hpp"

namespace daidalosengine {
	/**
	* Handle a directional light
	*/
	class CDirectionalLightSceneNode : public ILightSceneNode {
	protected:
		COrthographicCameraSceneNode * m_camera;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param rotation The rotation
		*/
		CDirectionalLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & rotation);

		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param rotation The rotation
		* @param color The color
		* @param shadowCaster If the light cast shadows
		* @param enabled If the light is enabled by default
		*/
		CDirectionalLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & rotation, const CColor & color, const bool shadowCaster, const bool enabled = true);

		/**
		* Destructor
		*/
		virtual ~CDirectionalLightSceneNode();

		/**
		* Update the light
		* @param time The elapsed time
		*/
		virtual void Update(const float time = 0) override;

		/**
		* Get a camera node
		* @param id The camera id
		* @return The camera
		*/
		virtual ICameraSceneNode * GetCameraNode(const unsigned int id = 0) const override;
	};
}

#endif
