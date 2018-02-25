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

#ifndef __CBSPBRUSHNODE_HPP
#define __CBSPBRUSHNODE_HPP

#include <vector>
#include <memory>
#include "../../../../Core/Utility/CVector2.hpp"
#include "../../../../Core/Utility/CVector3.hpp"
#include "../../../../Core/Utility/CVector4.hpp"
#include "../../../../Scene/3D/CPlane3D.hpp"
#include "../../../../Scene/3D/IShape3D.hpp"
#include "../../../../Scene/3D/CImpact.hpp"
#include "../../../../Scene/3D/Structure/BSP/CBSPTriangle.hpp"

namespace daidalosengine {
	class CBSPTreeNode;

	/**
	* A bsp brush
	*/
	struct SBSPBrush {
		int id;
		bool tested;
		bool enabled;
		std::unique_ptr<CBSPTreeNode> root;
	};

	/**
	* The bsp brush node class
	* Handle the node of a BSP brush
	*/
	class CBSPBrushNode {
	private:
		EBSPNodeType m_type;
		std::vector<SBSPBrush*> m_brushes;
		CPlane3DF m_plane;
		CBSPBrushNode * m_children[2];

	public:
		/**
		* Constructor
		* @param root The tree node to duplicate
		*/
		explicit CBSPBrushNode(CBSPTreeNode * root);

		/**
		* Destructor
		*/
		~CBSPBrushNode();

		/**
		* Add a brush to the node
		* @param brush The brush to add
		* @param triangles The brush geometry
		*/
		void AddBrush(SBSPBrush * brush, const std::vector<CBSPTriangle*> & triangles);

		/**
		* Detect a collison with a shape in movement
		* @param shape The object' shape
		* @param impact The final impact
		* @param iBrush The temporaty impact
		*/
		void CollisionDetection(IShape3DF & shape, CImpact & impact, CImpact & iBrush);

		/**
		* Detect a collison with a point in movement
		* @param position The object's position
		* @param impact The final impact
		* @param iBrush The temporaty impact
		*/
		void CollisionDetection(const CVector3F & position, CImpact & impact, CImpact & iBrush);

	private:
		CBSPBrushNode(const CBSPBrushNode & copy);
		const CBSPBrushNode & operator=(const CBSPBrushNode & copy);
	};
}

#endif
