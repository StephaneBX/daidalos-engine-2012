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

#ifndef __IBOUNDEDLIGHTSCENENODE_HPP
#define __IBOUNDEDLIGHTSCENENODE_HPP

#include <memory>
#include "../../Scene/3D/ILightSceneNode.hpp"
#include "../../Scene/3D/CSphere.hpp"

namespace daidalosengine {
	/**
	* Basic class for all bounded / finite lights.
	*/
	class IBoundedLightSceneNode : public ILightSceneNode {
	private:
		float m_range;
		float m_falloff;
		CRectangleI m_scissor;
		std::unique_ptr<CSphereF>m_visibilityShape;

	public:
		/**
		* Destructor
		*/
		virtual ~IBoundedLightSceneNode();

		/**
		* Set the light range
		* @param range The light range
		*/
		void SetRange(const float range);

		/**
		* Set the falloff
		* @param falloff The falloff
		*/
		void SetFalloff(const float falloff);

		/**
		* Update the camera
		* @param time The elapsed time
		*/
		virtual void Update(const float time) override;

		/**
		* Get the light range
		* @return The light range
		*/
		const float GetRange() const;

		/**
		* Get the light falloff
		* @return The light falloff
		*/
		const float GetFalloff() const;

		/**
		* Get the light scissor
		* @return The light scissor
		*/
		const CRectangleI & GetScissor() const;

		/**
		* Get the light visibility sphere
		* @return The light visibility sphere
		*/
		CSphereF * GetVisibilitySphere() const;

	protected:
		/**
		* Constructor
		* @param scene The scene
		* @param parent The parent
		* @param name The node name
		* @param position The position
		* @param rotation The rotation
		* @param color The color
		* @param shadowCaster If the light cast shadows
		* @param range The range
		* @param falloff The falloff
		* @param enabled If the light is enabled
		*/
		IBoundedLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CColor & color, const bool shadowCaster, const float range, const float falloff, const bool enabled);

	private:
		IBoundedLightSceneNode();
		IBoundedLightSceneNode(const IBoundedLightSceneNode & copy);
	};
}

#endif
