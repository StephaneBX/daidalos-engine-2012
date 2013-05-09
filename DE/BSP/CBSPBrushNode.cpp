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

#include "../BSP/CBSPBrushNode.h"
#include "../BSP/CBSPTreeNode.h"

namespace DE
{
	CBSPBrushNode::CBSPBrushNode(CBSPTreeNode * root)
	{
		this->m_type = root->GetType();
		this->m_children[BSP_NODE_CHILD_FRONT] = nullptr;
		this->m_children[BSP_NODE_CHILD_BACK] = nullptr;

		if(this->m_type != BSP_NODE_TYPE_NODE)
			return;

		this->m_plane = root->GetPlane();

		if(root->GetChild(BSP_NODE_CHILD_FRONT) != nullptr)
			this->m_children[BSP_NODE_CHILD_FRONT] = new CBSPBrushNode(root->GetChild(BSP_NODE_CHILD_FRONT));

		if(root->GetChild(BSP_NODE_CHILD_BACK) != nullptr)
			this->m_children[BSP_NODE_CHILD_BACK] = new CBSPBrushNode(root->GetChild(BSP_NODE_CHILD_BACK));
	}

	CBSPBrushNode::~CBSPBrushNode()
	{
		SAFE_DELETE(this->m_children[BSP_NODE_CHILD_FRONT]);
		SAFE_DELETE(this->m_children[BSP_NODE_CHILD_BACK]);
	}

	void CBSPBrushNode::AddBrush(SBSPBrush * brush, const std::vector<CBSPTriangle*> & triangles)
	{
		if(this->m_type != BSP_NODE_TYPE_NODE)
			return;

		bool front = false;
		bool back = false;
		bool coplanar = false;

		for(unsigned int i = 0 ; i < triangles.size() ; ++i)
		{
			switch(triangles.at(i)->Classify(this->m_plane))
			{
			case BSP_TRIANGLE_ORIENTATION_FRONT:
				front = true;
				break;

			case BSP_TRIANGLE_ORIENTATION_BACK:
				back = true;
				break;

			case BSP_TRIANGLE_ORIENTATION_SPANNING:
				front = true;
				back = true;
				break;

			case BSP_TRIANGLE_ORIENTATION_COPLANAR:
				coplanar = true;
				break;
			}
		}

		// Add the brush to the node if at least one triangle is coplanar to the plane
		this->m_brushes.push_back(brush);

		// Visit the child if necessary
		if(front)
			this->m_children[BSP_NODE_CHILD_FRONT]->AddBrush(brush, triangles);
		if(back)
			this->m_children[BSP_NODE_CHILD_BACK]->AddBrush(brush, triangles);
	}

	void CBSPBrushNode::CollisionDetection(IShape3DF & shape, CImpact & impact, CImpact & iBrush)
	{
		if(this->m_type != BSP_NODE_TYPE_NODE)
			return;

		// Get the distances
		float d1 = this->m_plane.PointDistance(impact.GetStartPoint());
		float d2 = this->m_plane.PointDistance(impact.GetEndPoint());

		// Get the offset
		float offset = shape.GetInnerDepth(this->m_plane);

		// Front check
		if(d1 > offset && d2 > offset)
		{
			// The impact is in front of the plane, we need to visit the front child
			this->m_children[BSP_NODE_CHILD_FRONT]->CollisionDetection(shape, impact, iBrush);
			return;
		}

		this->m_plane.Inverse();
		float offset2 = shape.GetInnerDepth(this->m_plane);
		this->m_plane.Inverse();

		// Back check
		if(d1 < -offset2 && d2 < -offset2)
		{
			// The impact is in the back of the plane, we need to visit the back child
			this->m_children[BSP_NODE_CHILD_BACK]->CollisionDetection(shape, impact, iBrush);
			return;
		}

		// Cross check
		if(d1 > offset && d2 <= offset2)
		{
			for(unsigned int i = 0 ; i < this->m_brushes.size() ; ++i)
			{
				if(this->m_brushes.at(i)->enabled)
					this->m_brushes.at(i)->root->CollisionDetection(shape, impact, iBrush);
			}
		}

		// Visit in the right order
		if(d1 > d2)
		{
			this->m_children[BSP_NODE_CHILD_FRONT]->CollisionDetection(shape, impact, iBrush);
			this->m_children[BSP_NODE_CHILD_BACK]->CollisionDetection(shape, impact, iBrush);
		}
		else
		{
			this->m_children[BSP_NODE_CHILD_FRONT]->CollisionDetection(shape, impact, iBrush);
			this->m_children[BSP_NODE_CHILD_BACK]->CollisionDetection(shape, impact, iBrush);
		}
	}
}
