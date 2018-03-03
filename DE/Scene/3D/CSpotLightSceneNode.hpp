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

#ifndef __CSPOTLIGHTSCENENODE_HPP
#define __CSPOTLIGHTSCENENODE_HPP

#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CColor.hpp"
#include "../../Scene/3D/CSphere.hpp"
#include "../../Scene/3D/IBoundedLightSceneNode.hpp"
#include "../../Scene/3D/CProjectionCameraSceneNode.hpp"

namespace daidalosengine {
	/**
	* Handle a spot light
	*/
	class CSpotLightSceneNode : public IBoundedLightSceneNode {
	private:
		float m_radius;
		float m_tightness;
		CProjectionCameraSceneNode * m_camera;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param rotation The rotation
		*/
		CSpotLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation);

		/**
		* Constructor
		* @param scene The associated
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param rotation The rotation
		* @param color The color
		* @param range The cone range (distance)
		* @param radius The cone radius
		* @param falloff The cone falloff
		* @param tightness The cone tightness
		* @param shadowCaster If the light cast shadows
		* @param enabled If the light is enabled by default
		*/
		CSpotLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CColor & color, const float range, const float radius, const float falloff, const float tightness, const bool shadowCaster, const bool enabled = true);

		/**
		* Destructor
		*/
		virtual ~CSpotLightSceneNode();

		/**
		* Set the radius
		* @param radius The radius
		*/
		void SetRadius(const float radius);

		/**
		* Set the tightness
		* @param tightness The tightness
		*/
		void SetTightness(const float tightness);

		/**
		* Update the light
		* @param time The elapsed time
		*/
		virtual void Update(const float time = 0) override;

		/**
		* Get the radius
		* @return The radius
		*/
		const float GetRadius() const;

		/**
		* Get the tightness
		* @return The tightness
		*/
		const float GetTightness() const;

		/**
		* Get a camera node
		* @param id The camera id
		* @return The camera
		*/
		virtual ICameraSceneNode * GetCameraNode(const unsigned int id = 0) const override;
	};
}

#endif
