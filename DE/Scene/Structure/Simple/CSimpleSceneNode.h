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

#ifndef __CSIMPLESCENENODE_H
#define __CSIMPLESCENENODE_H

#include "../../../Scene/CMeshBuffer.h"
#include "../../../Scene/ICameraSceneNode.h"
#include "../../../Scene/ILevelSceneNode.h"
#include "../../../Scene/CSphere.h"

namespace DE
{
	/**
	* The structure used by the node
	*/
	struct SSimpleBrush
	{
		CSphereF * visibilityShape;
		std::vector<CTriangle3D*> geometry;
	};

	/**
	* Handle a 3d simple node
	*/
	class CSimpleSceneNode : public ILevelSceneNode
	{
	protected:
		std::vector<CTriangle3D*> m_visibleTriangles;
		std::vector<SSimpleBrush> m_brushes;
	
	public:
		/**
		* Constructor
		*/
		CSimpleSceneNode(IScene * scene);

		/**
		* Destructor
		*/
		~CSimpleSceneNode();

		/**
		* Load a geometry in the node
		* @param brushes A vector of brush
		*/
		void LoadGeometry(const std::vector<SSimpleBrush> & brushes);

		/**
		* Render the node
		*/
		void Render();

		/**
		* Slide a shape on the bsp
		* @param shape The shape to slide
		* @param movement A CVector3F describing the shape movement
		* @return True if a collision occurred
		*/
		const bool Slide(IShape3DF & shape, const CVector3F & movement);

		/**
		* Get the collision point of a moving shape
		* @param shape The shape
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		const bool GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point);

	protected:
		/**
		* Test the brushes and register their geometry for rendering if the brush is visible
		* @param node A brush
		*/
		void RegisterBrushesForRendering();

	private:
		CSimpleSceneNode(const CSimpleSceneNode & copy);
		const CSimpleSceneNode & operator=(const CSimpleSceneNode & copy);
	};
}

#endif
