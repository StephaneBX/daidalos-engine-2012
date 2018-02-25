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

#ifndef __CFRUSTUM_HPP
#define __CFRUSTUM_HPP

#include "../../Core/Utility/CVector2.hpp"
#include "../../Core/Utility/CVector3.hpp"
#include "../../Scene/3D/IShape3D.hpp"
#include "../../Scene/3D/CTriangle3D.hpp"
#include "../../Scene/3D/CPlane3D.hpp"

namespace daidalosengine {
	/**
	* Handle a cone frustum
	*/
	class CFrustum {
	private:
		float m_nearDistance;
		float m_farDistance;
		float m_angle;
		float m_ratio;
		float m_tang;

		CVector2F m_nearSize;
		CVector2F m_farSize;
		CVector3F m_position;
		CVector3F m_forward;
		CVector3F m_lookAt;
		CVector3F m_up;
		CVector3F m_worldSpacePoints[FRUSTUM_POINT_SIZE];
		CVector3F m_localSpacePoints[FRUSTUM_POINT_SIZE];
		CPlane3DF m_frustum[FRUSTUM_PLANE_SIZE];

	public:
		/**
		* Constructor
		*/
		CFrustum();

		/**
		* Constructor
		* @param angle The half angle in degree
		* @param ratio Tha ratio
		* @param nearDistance The near distance
		* @param farDistance The far distance
		*/
		CFrustum(const float angle, const float ratio, const float nearDistance, const float farDistance);

		/**
		* Destructor
		*/
		~CFrustum();

		/**
		* Set the frustum
		* @param angle The half angle in degree
		* @param ratio Tha ratio
		* @param nearDistance The near distance
		* @param farDistance The far distance
		*/
		void Set(const float angle, const float ratio, const float nearDistance, const float farDistance);

		/**
		* Update the frustum
		* @param position The new frustum's owner position
		* @param lookAt The new frustum's owner looking coordinate
		*/
		void Update(const CVector3F & position, const CVector3F & lookAt);

		/**
		* Perform a back-face culling test (test if the face is in front of the camera)
		* @param vertex The vertices
		* @param faceNormal The face normal
		* @return True if the face can be culled or false otherwise
		*/
		const bool BackfaceCull(const CVector3F & vertex, const CVector3F & faceNormal) const;

		/**
		* Perform a frustum culling test with the face to determin if it need to be discared for the rendering process
		* The goal here is to verify the following restriction in order to be considered as visible:
		* - The face is within the view range
		* - The face is is the frustum represented by six planes
		* @param shape The shape that need to be tested
		* @return True if the shape can be culled or false otherwise
		*/
		const bool FrustumCull(IShape3DF * shape) const;

		/**
		* Get the fov
		* @return The fov in degree
		*/
		const float GetFov() const;

		/**
		* Get the near distance
		* @return The near distance
		*/
		const float GetNearDistance() const;

		/**
		* Get the far distance
		* @return The far distance
		*/
		const float GetFarDistance() const;

		/**
		* Get the position
		* @return The position
		*/
		const CVector3F & GetPosition() const;

		/**
		* Get the forward vector
		* @return The foward vector
		*/
		const CVector3F & GetForward() const;

		/**
		* Get the look at point
		* @return The look at point
		*/
		const CVector3F & GetLookAt() const;

		/**
		* Get the Up vector
		* @return The up vector
		*/
		const CVector3F & GetUp() const;

		/**
		* Get the local space points of the frustum (used by the deferred shading to build the position information)
		* @return The local points
		*/
		const CVector3F * GetLocalSpacePoints() const;
	};
}

#endif
