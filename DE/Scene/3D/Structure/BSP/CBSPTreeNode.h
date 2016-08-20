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

#ifndef __CBSPTREENODE_H
#define __CBSPTREENODE_H

#include <vector>
#include <memory>
#include "../../../../Core/Utility/CVector2.h"
#include "../../../../Core/Utility/CVector3.h"
#include "../../../../Core/Utility/CVector4.h"
#include "../../../../Medias/ITexture2DBase.h"
#include "../../../../Scene/3D/CPlane3D.h"
#include "../../../../Scene/3D/CSphere.h"
#include "../../../../Scene/3D/CImpact.h"
#include "../../../../Scene/3D/CAABoundingBox.h"
#include "../../../../Scene/3D/Structure/BSP/CBSPTriangle.h"
#include "../../../../Scene/3D/Structure/BSP/CBSPPortal.h"

#define DEFAULT_BSP_SCORE 10000000.0f

namespace DE {
	/**
	* The bsp tree node class
	* Handle the node of a BSP tree
	*/
	class CBSPTreeNode {
	private:
		EBSPNodeType m_type;
		std::vector<std::unique_ptr<CBSPTriangle> > m_triangles;
		CPlane3DF m_plane;
		CBSPTreeNode * m_children[2];
		CSphereF m_visibilitySphere;

		// PVS generation-only attributes
		std::unique_ptr<CAABoundingBoxF> m_boundingBox;
		std::vector<CBSPPortal> m_portals;

	public:
		/**
		* Constructor
		*/
		CBSPTreeNode();

		/**
		* Constructor
		* @param type The node type
		*/
		CBSPTreeNode(EBSPNodeType type);

		/**
		* Destructor
		*/
		~CBSPTreeNode();

		/**
		* Load a triangle list
		* @param triangles The triangles list
		*/
		void LoadTriangleList(const std::vector<CBSPTriangle*> & triangles);

		/**
		* Rest the draw state of the geometry
		*/
		void ResetDraw();

		/**
		* Detect a collison with a shape in movement
		* @param shape The object' shape
		* @param impact The final impact
		* @param iBrush The temporaty impact
		*/
		void CollisionDetection(IShape3DF & shape, CImpact & impact, CImpact & iBrush);

		/**
		* Remove the triangles in the leafs
		*/
		void ClearTriangles();

		/**
		* Get the geometry node type
		* @return The node type
		*/
		EBSPNodeType GetType() const;

		/**
		* Get the geometry node plane
		* @return The geometry node plane
		*/
		const CPlane3DF & GetPlane() const;

		/**
		* Get the node visibility sphere
		*/
		CSphereF & GetVisibilitySphere();

		/**
		* Get the bounding box
		*/
		CAABoundingBoxF * GetBoundingBox() const;

		/**
		* Get the triangles
		* @return The triangles
		*/
		const std::vector<std::unique_ptr<CBSPTriangle> > & GetTriangles() const;

		/**
		* Get a child node
		* @param child The child to get
		* @return The child
		*/
		CBSPTreeNode * GetChild(EBSPNodeChild child);

	protected:
		/**
		* Add triangles to the node
		* @param triangles The triangles
		*/
		void AddTriangles(const std::vector<CBSPTriangle*> & triangles);

		/**
		* Split a triangle at the current plane position, creating a set of triangles
		* @param triangle The triangle to split
		* @param front The vector that will contain all the newly triangles in front of the plane
		* @param back The vector that will contain all the newly triangles at the back of the plane
		*/
		void SplitTriangle(CBSPTriangle * triangle, std::vector<CBSPTriangle*> & front, std::vector<CBSPTriangle*> & back);

		/**
		* Calculate the node's bounding box given a geometry
		* @param triangles A list of triangles
		*/
		void CalculateBoundingBox(const std::vector<CBSPTriangle*> triangles);

		/**
		* Calculate the node's visibility sphere
		* @param triangles A list of triangles
		*/
		void CalculateVisibilitySphere(const std::vector<CBSPTriangle*> triangles);

		/**
		* Build the initial portal of the node
		*/
		void BuildInitialPortal();

		/**
		* Find the best faces to be used as a splitter
		* @param triangles The list of triangles to test
		* @return The triangle choosen to be a splitter
		*/
		CBSPTriangle * FindBestSplitter(const std::vector<CBSPTriangle*> & triangles);

	private:
		CBSPTreeNode(const CBSPTreeNode & copy);
		const CBSPTreeNode & operator=(const CBSPTreeNode & copy);
	};
}

#endif
