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

#ifndef __ICAMERASCENENODE_H
#define __ICAMERASCENENODE_H

#include "../Core/Utility/CMatrix.h"
#include "../Core/Utility/CVector3.h"
#include "../Scene/ISceneNode.h"

namespace DE
{
	/**
	* Structure that hold view frustum information
	*/
	struct SViewFrustum
	{
		CVector3F forward;
		CVector3F absolutePoints[FRUSTUM_POINT_SIZE];
		CVector3F relativePoints[FRUSTUM_POINT_SIZE];
		CPlane3DF planes[FRUSTUM_PLANE_SIZE];
	};

	/**
	* Handle a camera
	*/
	class ICameraSceneNode : public ISceneNode
	{
	protected:
		float m_nearDistance;
		float m_farDistance;
		CMatrix m_viewMatrix;
		CMatrix m_projMatrix;
		CMatrix m_viewProjMatrix;
		CVector3F m_localUp;
		CVector3F m_localLookAt;
		CVector3F m_absoluteUp;
		CVector3F m_absoluteLookAt;
		SViewFrustum m_frustum;

	public:
		/**
		* Destructor
		*/
		virtual ~ICameraSceneNode();

		/**
		* Set the near distance
		* @param nearDistance The near distance
		*/
		void SetNearDistance(const float nearDistance);

		/**
		* Set the far distance
		* @param farDistance The far distance
		*/
		void SetFarDistance(const float farDistance);

		/**
		* Update the camera information
		* @param time The elapsed time
		*/
		virtual void Update(const float time = 0);

		/**
		* Perform a frustum culling test with the face to determine if it need to be discared for the rendering process
		* The goal here is to verify the following restriction in order to be considered as visible:
		* - The face is within the view range
		* - The face is is the frustum represented by six planes
		* @param shape The shape that need to be tested
		* @return True if the shape can be culled or false otherwise
		*/
		virtual const bool FrustumCull(IShape3DF * shape) const;

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
		* Get the view matrix
		* @return The view matrix
		*/
		const CMatrix & GetViewMatrix() const;

		/**
		* Get the projection matrix
		* @return The projection matrix
		*/
		const CMatrix & GetProjMatrix() const;

		/**
		* Get the view projection matrix
		* @return The view projection matrix
		*/
		const CMatrix & GetViewProjMatrix() const;

		/**
		* Get a view frustm point
		* @param point The point to get
		* @return A view frustum point
		*/
		const CVector3F & GetViewFrustumRelativePoint(EFrustumPoint point) const;

		/**
		* Get a view frustum plane
		* @param plane The plane name
		* @return The plane
		*/
		const CPlane3DF & GetViewFrustumPlane(EFrustumPlane plane) const;

		/**
		* Get the forward vector
		* @return The forward vector
		*/
		const CVector3F & GetForwardVector() const;

		/**
		* Get the up vector
		* @return The up vector
		*/
		const CVector3F & GetUpVector() const;

	protected:
		/**
		* Constructor
		* @param parent The parent
		* @param name The node name
		* @param position The position
		* @param rotation The rotation
		* @param nearDistance The near distance
		* @param farDistance The far distance
		*/
		ICameraSceneNode(ISceneNode * parent, const std::string & name,  const CVector3F & position, const CVector3F & rotation, const float nearDistance, const float farDistance);

	private:
		ICameraSceneNode();
		ICameraSceneNode(const ICameraSceneNode & copy);
	};
}

#endif
