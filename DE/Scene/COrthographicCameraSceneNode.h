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

#ifndef __CORTHOGRAPHICCAMERASCENENODE_H
#define __CORTHOGRAPHICCAMERASCENENODE_H

#include "../Core/Utility/CMatrix.h"
#include "../Core/Utility/CVector3.h"
#include "../Scene/ICameraSceneNode.h"

namespace DE
{
	/**
	* Handle an orthographic typed camera
	*/
	class COrthographicCameraSceneNode : public ICameraSceneNode
	{
	private:
		CRectangleF m_bounds;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param position The position
		* @param rotation The rotation
		* @param bounds The view bounds
		* @param nearDistance The near distance
		* @param farDistance The far distance
		*/
		COrthographicCameraSceneNode(IScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CRectangleF & bounds, const float nearDistance, const float farDistance);

		/**
		* Destructor
		*/
		~COrthographicCameraSceneNode();

		/**
		* Set the view bounds
		* @param bounds The view bounds
		*/
		void SetBounds(const CRectangleF & bounds);

		/**
		* Update the camera
		* @param time The elapsed time
		*/
		void Update(const float time = 0);

		/**
		* Update The projection
		*/
		void UpdateProjection();

		/**
		* Perform a frustum culling test with the face to determine if it need to be discarded for the rendering process
		* The goal here is to verify the following restriction in order to be considered as visible:
		* - The face is within the view range
		* - The face is is the frustum represented by six planes
		* @param shape The shape that need to be tested
		* @return True if the shape can be culled or false otherwise
		*/
		const bool FrustumCull(IShape3DF * shape) const;

		/**
		* Get the view bounds
		* @return The view bounds
		*/
		const CRectangleF & GetBounds() const;

	private:
		COrthographicCameraSceneNode();
		COrthographicCameraSceneNode(const COrthographicCameraSceneNode & copy);
	};
}

#endif
