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

#ifndef __CSIMPLESCENENODE_HPP
#define __CSIMPLESCENENODE_HPP

#include "../../../../Scene/3D/ICameraSceneNode.hpp"
#include "../../../../Scene/3D/ILevelSceneNode.hpp"
#include "../../../../Scene/3D/CSphere.hpp"

namespace daidalosengine {
	/**
	* The structure used by the node
	*/
	struct SSimpleBrush {
		bool isMultiBrush;
		std::string filename;
		CVector3F position;
		CVector3F rotation;
		CVector3F scale;
		CSphereF * visibilityShape;
		std::vector<CTriangle3D*> geometry;
	};

	/**
	* Handle a 3d simple node
	*/
	class CSimpleSceneNode : public ILevelSceneNode {
	private:
		std::vector<CTriangle3D*> m_visibleTriangles;
		std::vector<SSimpleBrush> m_brushes;

	public:
		/**
		* Constructor
		*/
		explicit CSimpleSceneNode(I3DScene * scene);

		/**
		* Destructor
		*/
		virtual ~CSimpleSceneNode();

		/**
		* Load a geometry in the node
		* @param brushes A vector of brush
		*/
		void LoadGeometry(const std::vector<SSimpleBrush> & brushes);

		/**
		* Render the node
		* @param filterId The filter id to apply the select the rendered nodes
		*/
		virtual void Render(const int filterId = SCENE_DEFAULT_ID) override;

		/**
		* Render the transparent elements of the level
		*/
		virtual void RenderTransparent() override;

		/**
		* Render the geometry that will let the skybox pass through
		*/
		virtual void RenderSkyboxMask() override;

		/**
		* Set the collision state of a brush id
		*/
		virtual void SetBrushCollision(const int brushId, const bool collision) override;

		/**
		* Slide a shape on the bsp
		* @param shape The shape to slide
		* @param movement A CVector3F describing the shape movement
		* @return True if a collision occurred
		*/
		virtual bool Slide(IShape3DF & shape, const CVector3F & movement) override;

		/**
		* Get the collision point of a moving point
		* @param position The point
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		virtual bool GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) override;

		/**
		* Get the collision point of a moving shape
		* @param shape The shape
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		virtual bool GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) override;

	protected:
		/**
		* Render the node with an optional filterId
		* @param filterId The filter id or -1 to render all
		*/
		void RenderWithOptions(const int filterId);

		/**
		* Test the brushes and register their geometry for rendering if the brush is visible
		* @param filterId Limit the geometry to the id filterId or -1 to render all
		*/
		void RegisterBrushesForRendering(const int filterId);

	public:
		CSimpleSceneNode(const CSimpleSceneNode & copy) = delete;
		const CSimpleSceneNode & operator=(const CSimpleSceneNode & copy) = delete;
	};
}

#endif
