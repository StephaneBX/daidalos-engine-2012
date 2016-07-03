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

#include <limits>
#include "../../../../Core/Core.h"
#include "../../../../Core/Logger/ILogger.h"
#include "../../../../Scene/3D/Structure/BSP/CBSPTreeNode.h"

#define BSP_MIN_DISTANCE 0.01f

namespace DE {
	CBSPTreeNode::CBSPTreeNode() {
		LOG_TRACE();

		this->m_type = BSP_NODE_TYPE_NODE;
		this->m_boundingBox = std::unique_ptr<CAABoundingBoxF>(new CAABoundingBoxF());
		this->m_children[BSP_NODE_CHILD_FRONT] = nullptr;
		this->m_children[BSP_NODE_CHILD_BACK] = nullptr;
	}

	CBSPTreeNode::CBSPTreeNode(EBSPNodeType type) {
		LOG_TRACE();

		this->m_type = type;
		this->m_boundingBox = std::unique_ptr<CAABoundingBoxF>(new CAABoundingBoxF());
		this->m_children[BSP_NODE_CHILD_FRONT] = nullptr;
		this->m_children[BSP_NODE_CHILD_BACK] = nullptr;
	}

	CBSPTreeNode::~CBSPTreeNode() {
		LOG_TRACE();

		SAFE_DELETE(this->m_children[BSP_NODE_CHILD_FRONT]);
		SAFE_DELETE(this->m_children[BSP_NODE_CHILD_BACK]);
	}

	void CBSPTreeNode::LoadTriangleList(const std::vector<CBSPTriangle*> & triangles) {
		LOG_TRACE();

		// Get the bounding box
		CalculateBoundingBox(triangles);

		// Find a splitter
		CBSPTriangle * splitter = FindBestSplitter(triangles);

		// Get its plane
		this->m_plane = splitter->GetPlane();

		// Classify the triangles
		std::vector<CBSPTriangle*> front;
		std::vector<CBSPTriangle*> back;

		for(auto it = triangles.begin(); it != triangles.end(); ++it) {
			switch((*it)->Classify(this->m_plane)) {
			case BSP_TRIANGLE_ORIENTATION_COPLANAR:
				// We need to test if the triangle is in the same orientation as the plane
				if((*it)->GetPlane().GetNormal().DotProduct(this->m_plane.GetNormal()) >= 0) {
					// Goes to the front child
					front.push_back((*it));
				} else {
					// Fix?
					// Changed from back to front in order to correct collision problems with 2 coplanar plans with opposite normal
					front.push_back((*it));
				}
				break;

			case BSP_TRIANGLE_ORIENTATION_FRONT:
				// Goes to the front child
				front.push_back((*it));
				break;

			case BSP_TRIANGLE_ORIENTATION_BACK:
				// Goes to the back child
				back.push_back((*it));
				break;

			default:
			case BSP_TRIANGLE_ORIENTATION_SPANNING:
				// Split the triangle
				SplitTriangle((*it), front, back);
				break;
			}
		}

		// Check front splitters
		int usableCount = 0;
		for(auto it = front.begin(); it != front.end(); ++it) {
			if(!(*it)->IsSeparationPlane()) {
				++usableCount;
			}
		}

		// If all the triangles are splitters, we have a leaf
		if(usableCount == 0) {
			this->m_children[BSP_NODE_CHILD_FRONT] = new CBSPTreeNode(BSP_NODE_TYPE_EMPTY_LEAF);
			this->m_children[BSP_NODE_CHILD_FRONT]->AddTriangles(front);
			this->m_children[BSP_NODE_CHILD_FRONT]->CalculateBoundingBox(front);
		} else {
			this->m_children[BSP_NODE_CHILD_FRONT] = new CBSPTreeNode(BSP_NODE_TYPE_NODE);
			this->m_children[BSP_NODE_CHILD_FRONT]->LoadTriangleList(front);
		}

		// Back child
		if(back.size() == 0) {
			this->m_children[BSP_NODE_CHILD_BACK] = new CBSPTreeNode(BSP_NODE_TYPE_SOLID_LEAF);
		} else {
			usableCount = 0;
			for(auto it = back.begin(); it != back.end(); ++it) {
				if(!(*it)->IsSeparationPlane()) {
					++usableCount;
				}
			}

			// Invalid geometry but still, we want to prevent crash
			if(usableCount == 0) {
				this->m_children[BSP_NODE_CHILD_BACK] = new CBSPTreeNode(BSP_NODE_TYPE_SOLID_LEAF);
				this->m_children[BSP_NODE_CHILD_BACK]->AddTriangles(back);
			} else {
				// Valid node
				this->m_children[BSP_NODE_CHILD_BACK] = new CBSPTreeNode(BSP_NODE_TYPE_NODE);
				this->m_children[BSP_NODE_CHILD_BACK]->LoadTriangleList(back);
			}
		}
	}

	void CBSPTreeNode::ResetDraw() {
		LOG_TRACE();

		for(auto it = this->m_triangles.begin(); it != this->m_triangles.end(); ++it)
			(*it)->SetDrawn(false);

		if(this->m_children[BSP_NODE_CHILD_FRONT] != nullptr)
			this->m_children[BSP_NODE_CHILD_FRONT]->ResetDraw();

		if(this->m_children[BSP_NODE_CHILD_BACK] != nullptr)
			this->m_children[BSP_NODE_CHILD_BACK]->ResetDraw();
	}

	void CBSPTreeNode::CollisionDetection(IShape3DF & shape, CImpact & impact, CImpact & iBrush) {
		LOG_TRACE();

		float impactFraction, realFraction;
		CVector3F normalTemp;

		// Empty leaf, no collision possible
		if(this->m_type == BSP_NODE_TYPE_EMPTY_LEAF) {
			iBrush.ClearImpact();
			return;
		}

		// Solid leaf, check for nearest impact
		if(this->m_type == BSP_NODE_TYPE_SOLID_LEAF) {
			impact.SetNearestImpact(iBrush.GetPlaneNormal(), iBrush.GetImpactFraction(), iBrush.GetRealFraction());
			iBrush.ClearImpact();
			return;
		}

		// A node

		// Get the distances
		float d1 = this->m_plane.PointDistance(impact.GetStartPoint());
		float d2 = this->m_plane.PointDistance(impact.GetEndPoint());

		// Get the offset
		float offset = shape.GetInnerDepth(this->m_plane);

		// Front check
		if(d1 > offset && d2 > offset) {
			// The impact is in front of the plane, we need to visit the front child
			this->m_children[BSP_NODE_CHILD_FRONT]->CollisionDetection(shape, impact, iBrush);
			return;
		}

		this->m_plane.Inverse();
		float offset2 = shape.GetInnerDepth(this->m_plane);
		this->m_plane.Inverse();

		// Back check
		if(d1 < -offset2 && d2 < -offset2) {
			// The impact is in the back of the plane, we need to visit the back child
			this->m_children[BSP_NODE_CHILD_BACK]->CollisionDetection(shape, impact, iBrush);
			return;
		}

		// Cross check
		if(d1 > offset && d2 <= offset2) {
			float d1r = d1 - offset;
			float d2r = d2 - offset;
			realFraction = d1r / (d1r - d2r);

			if(realFraction >= impact.GetRealFraction())
				return;

			if(realFraction < iBrush.GetRealFraction()) {
				normalTemp = this->m_plane.GetNormal();
				impactFraction = (d1r - BSP_MIN_DISTANCE) / (d1r - d2r);
				iBrush.SetImpact(normalTemp, impactFraction, realFraction);
			} else {
				// Memorize iBrush data
				normalTemp = iBrush.GetPlaneNormal();
				impactFraction = iBrush.GetImpactFraction();
				realFraction = iBrush.GetRealFraction();
			}
		} else {
			// Memorize iBrush data
			normalTemp = iBrush.GetPlaneNormal();
			impactFraction = iBrush.GetImpactFraction();
			realFraction = iBrush.GetRealFraction();
		}

		this->m_children[BSP_NODE_CHILD_BACK]->CollisionDetection(shape, impact, iBrush);
		iBrush.SetImpact(normalTemp, impactFraction, realFraction);
		this->m_children[BSP_NODE_CHILD_FRONT]->CollisionDetection(shape, impact, iBrush);
	}

	void CBSPTreeNode::ClearTriangles() {
		LOG_TRACE();

		if(this->m_type == BSP_NODE_TYPE_EMPTY_LEAF) {
			this->m_triangles.clear();
			return;
		}

		if(this->m_type == BSP_NODE_TYPE_SOLID_LEAF)
			return;

		this->m_children[BSP_NODE_CHILD_FRONT]->ClearTriangles();
		this->m_children[BSP_NODE_CHILD_BACK]->ClearTriangles();
	}

	EBSPNodeType CBSPTreeNode::GetType() const {
		LOG_TRACE();

		return this->m_type;
	}

	const CPlane3DF & CBSPTreeNode::GetPlane() const {
		LOG_TRACE();

		return this->m_plane;
	}

	CAABoundingBoxF * CBSPTreeNode::GetBoundingBox() const {
		LOG_TRACE();

		return this->m_boundingBox.get();
	}

	const std::vector<std::unique_ptr<CBSPTriangle>> & CBSPTreeNode::GetTriangles() const {
		LOG_TRACE();

		return this->m_triangles;
	}

	CBSPTreeNode * CBSPTreeNode::GetChild(EBSPNodeChild child) {
		LOG_TRACE();

		return this->m_children[child];
	}

	void CBSPTreeNode::AddTriangles(const std::vector<CBSPTriangle*> & triangles) {
		LOG_TRACE();

		for(auto it = triangles.begin(); it < triangles.end(); ++it)
			this->m_triangles.push_back(std::unique_ptr<CBSPTriangle>((*it)));
	}

	void CBSPTreeNode::SplitTriangle(CBSPTriangle * triangle, std::vector<CBSPTriangle*> & front, std::vector<CBSPTriangle*> & back) {
		LOG_TRACE();

		CVector3F pivot, positive1, positive2, negative1, negative2, intersection1, intersection2;
		bool split = true;
		EPointOrientation orientation[3];
		int orientationCounter = 0;
		int positiveIndex1, positiveIndex2, negativeIndex1, negativeIndex2, pivotIndex;
		float kFactor1, kFactor2;
		CBSPTriangle * tmp;
		int firstOrderIndex, secondOrderIndex, thirdOrderIndex;

		// Test the triangle in order to know its position regarding the plane
		for(int i = 0; i < 3; ++i) {
			orientation[i] = this->m_plane.ClassifyPoint(triangle->GetVertex(i));
			switch(orientation[i]) {
			case POINT_ORIENTATION_POSITIVE_SIDE:
				++orientationCounter;
				break;

			case POINT_ORIENTATION_NEGATIVE_SIDE:
				--orientationCounter;
				break;
			}
		}

		// Apply modifications depending on the triangle position
		switch(orientationCounter) {
			// The triangle is on the positive side with one vertex on the plane
			// Or
			// The triangle is entirely on the positive side, no need to split
		case 2:
		case 3:
			front.push_back(triangle);
			break;

			// The triangle is on the negative side with one vertex on the plane
			// Or
			// The triangle is entirely on the negative side, no need to split
		case -2:
		case -3:
			back.push_back(triangle);
			break;

			// The triangle has one vertex on the negative side, one vertex on the positive side and one vertex on the plane
		case 0:

			// Fill the variables depending on the orientation of the vertices
			if(orientation[0] == POINT_ORIENTATION_ON_PLANE) {
				pivotIndex = 0;
				if(orientation[1] == POINT_ORIENTATION_POSITIVE_SIDE) {
					positiveIndex1 = 1;
					negativeIndex1 = 2;
				} else {
					positiveIndex1 = 2;
					negativeIndex1 = 1;
				}
			} else {
				if(orientation[1] == POINT_ORIENTATION_ON_PLANE) {
					pivotIndex = 1;
					if(orientation[0] == POINT_ORIENTATION_POSITIVE_SIDE) {
						positiveIndex1 = 0;
						negativeIndex1 = 2;
					} else {
						positiveIndex1 = 2;
						negativeIndex1 = 0;
					}
				} else {
					pivotIndex = 2;
					if(orientation[0] == POINT_ORIENTATION_POSITIVE_SIDE) {
						positiveIndex1 = 0;
						negativeIndex1 = 1;
					} else {
						positiveIndex1 = 1;
						negativeIndex1 = 0;
					}
				}
			}

			// Find the intersection point
			intersection1 = this->m_plane.IntersectSegment(triangle->GetVertex(positiveIndex1), triangle->GetVertex(negativeIndex1), kFactor1);

			// Default index
			firstOrderIndex = 0;
			secondOrderIndex = 1;
			thirdOrderIndex = 2;

			// Check the element order 
			if(triangle->GetFlatNormal().DotProduct((intersection1 - triangle->GetVertex(positiveIndex1)).CrossProduct(triangle->GetVertex(pivotIndex) - triangle->GetVertex(positiveIndex1)).Normalize()) < 0) {
				secondOrderIndex = 2;
				thirdOrderIndex = 1;
			}


			// Fill the front triangle
			tmp = new CBSPTriangle(*triangle);
			tmp->SetId(triangle->GetId());
			tmp->SetVertex(triangle->GetVertex(positiveIndex1), firstOrderIndex);
			tmp->SetVertex(intersection1, secondOrderIndex);
			tmp->SetVertex(triangle->GetVertex(pivotIndex), thirdOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1), firstOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1) + kFactor1 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex1)), secondOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(pivotIndex), thirdOrderIndex);
			tmp->SetNormal(triangle->GetNormal(positiveIndex1), firstOrderIndex);
			tmp->SetNormal(triangle->GetNormal(positiveIndex1) + kFactor1 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex1)), secondOrderIndex);
			tmp->SetNormal(triangle->GetNormal(pivotIndex), thirdOrderIndex);
			tmp->SetColor(triangle->GetColor(positiveIndex1), firstOrderIndex);
			tmp->SetColor(triangle->GetColor(positiveIndex1) + kFactor1 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex1)), secondOrderIndex);
			tmp->SetColor(triangle->GetColor(pivotIndex), thirdOrderIndex);
			tmp->Update();
			front.push_back(tmp);


			// Default index
			firstOrderIndex = 0;
			secondOrderIndex = 1;
			thirdOrderIndex = 2;

			// Check the element order 
			if(triangle->GetFlatNormal().DotProduct((triangle->GetVertex(pivotIndex) - triangle->GetVertex(negativeIndex1)).CrossProduct(intersection1 - triangle->GetVertex(negativeIndex1)).Normalize()) < 0) {
				secondOrderIndex = 2;
				thirdOrderIndex = 1;
			}

			// Fill the back triangle
			tmp = new CBSPTriangle(*triangle);
			tmp->SetId(triangle->GetId());
			tmp->SetVertex(triangle->GetVertex(negativeIndex1), firstOrderIndex);
			tmp->SetVertex(triangle->GetVertex(pivotIndex), secondOrderIndex);
			tmp->SetVertex(intersection1, thirdOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1), firstOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(pivotIndex), secondOrderIndex);
			tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1) + kFactor1 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex1)), thirdOrderIndex);
			tmp->SetNormal(triangle->GetNormal(negativeIndex1), firstOrderIndex);
			tmp->SetNormal(triangle->GetNormal(pivotIndex), secondOrderIndex);
			tmp->SetNormal(triangle->GetNormal(positiveIndex1) + kFactor1 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex1)), thirdOrderIndex);
			tmp->SetColor(triangle->GetColor(negativeIndex1), firstOrderIndex);
			tmp->SetColor(triangle->GetColor(pivotIndex), secondOrderIndex);
			tmp->SetColor(triangle->GetColor(positiveIndex1) + kFactor1 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex1)), thirdOrderIndex);
			tmp->Update();
			back.push_back(tmp);

			// Delete the original triangle
			delete triangle;
			break;

			// The triangle has one vertex on the negative side and two vertices on the positive side
			// Or,
			// The triangle has two vertices on the plane and on vertex on the positive side
		case 1:
			// Check the presence of vertice on the plane
			for(int i = 0; i < 3; ++i) {
				if(orientation[i] == POINT_ORIENTATION_ON_PLANE)
					split = false;
			}

			// Fill the variables depending on the orientation of the vertices
			if(split) {
				if(orientation[0] == POINT_ORIENTATION_NEGATIVE_SIDE) {
					negativeIndex1 = 0;
					positiveIndex1 = 1;
					positiveIndex2 = 2;
				} else {
					if(orientation[1] == POINT_ORIENTATION_NEGATIVE_SIDE) {
						negativeIndex1 = 1;
						positiveIndex1 = 0;
						positiveIndex2 = 2;
					} else {
						negativeIndex1 = 2;
						positiveIndex1 = 0;
						positiveIndex2 = 1;
					}
				}

				// Find the intersections points
				intersection1 = this->m_plane.IntersectSegment(triangle->GetVertex(positiveIndex1), triangle->GetVertex(negativeIndex1), kFactor1);
				intersection2 = this->m_plane.IntersectSegment(triangle->GetVertex(positiveIndex2), triangle->GetVertex(negativeIndex1), kFactor2);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((intersection2 - triangle->GetVertex(positiveIndex1)).CrossProduct(intersection1 - triangle->GetVertex(positiveIndex1)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				// Fill the front triangles
				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(positiveIndex1), firstOrderIndex);
				tmp->SetVertex(intersection2, secondOrderIndex);
				tmp->SetVertex(intersection1, thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex2) + kFactor2 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex2)), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1) + kFactor1 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex1)), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex1), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex2) + kFactor2 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex2)), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex1) + kFactor1 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex1)), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex1), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex2) + kFactor2 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex2)), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex1) + kFactor1 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex1)), thirdOrderIndex);
				tmp->Update();
				front.push_back(tmp);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((triangle->GetVertex(positiveIndex2) - triangle->GetVertex(positiveIndex1)).CrossProduct(intersection2 - triangle->GetVertex(positiveIndex1)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(positiveIndex1), firstOrderIndex);
				tmp->SetVertex(triangle->GetVertex(positiveIndex2), secondOrderIndex);
				tmp->SetVertex(intersection2, thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex2), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex2) + kFactor2 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex2)), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex1), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex2), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex2) + kFactor2 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex2)), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex1), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex2), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex2) + kFactor2 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex2)), thirdOrderIndex);
				tmp->Update();
				front.push_back(tmp);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((intersection1 - triangle->GetVertex(negativeIndex1)).CrossProduct(intersection2 - triangle->GetVertex(negativeIndex1)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				// Fill the back triangle
				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(negativeIndex1), firstOrderIndex);
				tmp->SetVertex(intersection1, secondOrderIndex);
				tmp->SetVertex(intersection2, thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1) + kFactor1 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex1)), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex2) + kFactor2 * (triangle->GetTexcoord(negativeIndex1) - triangle->GetTexcoord(positiveIndex2)), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex1), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex1) + kFactor1 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex1)), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex2) + kFactor2 * (triangle->GetNormal(negativeIndex1) - triangle->GetNormal(positiveIndex2)), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex1), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex1) + kFactor1 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex1)), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex2) + kFactor2 * (triangle->GetColor(negativeIndex1) - triangle->GetColor(positiveIndex2)), thirdOrderIndex);
				tmp->Update();
				back.push_back(tmp);

				// Delete the original triangle
				delete triangle;
			} else
				front.push_back(triangle);
			break;

			// The triangle has two vertices on the negative side and one vertex on the positive side
			// Or,
			// The triangle has two vertices on the plane and one vertex on the negative side
		case -1:
			// Check the presence of vertice on the plane
			for(int i = 0; i < 3; ++i) {
				if(orientation[i] == POINT_ORIENTATION_ON_PLANE)
					split = false;
			}

			// Fill the variables depending on the orientation of the vertices
			if(split) {
				if(orientation[0] == POINT_ORIENTATION_POSITIVE_SIDE) {
					positiveIndex1 = 0;
					negativeIndex1 = 1;
					negativeIndex2 = 2;
				} else {
					if(orientation[1] == POINT_ORIENTATION_POSITIVE_SIDE) {
						positiveIndex1 = 1;
						negativeIndex1 = 0;
						negativeIndex2 = 2;
					} else {
						positiveIndex1 = 2;
						negativeIndex1 = 0;
						negativeIndex2 = 1;
					}
				}

				// Find the intersections points
				intersection1 = this->m_plane.IntersectSegment(triangle->GetVertex(negativeIndex1), triangle->GetVertex(positiveIndex1), kFactor1);
				intersection2 = this->m_plane.IntersectSegment(triangle->GetVertex(negativeIndex2), triangle->GetVertex(positiveIndex1), kFactor2);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((intersection1 - triangle->GetVertex(positiveIndex1)).CrossProduct(intersection2 - triangle->GetVertex(positiveIndex1)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				// Fill the front triangle
				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(positiveIndex1), firstOrderIndex);
				tmp->SetVertex(intersection1, secondOrderIndex);
				tmp->SetVertex(intersection2, thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(positiveIndex1), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1) + kFactor1 * (triangle->GetTexcoord(positiveIndex1) - triangle->GetTexcoord(negativeIndex1)), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex2) + kFactor2 * (triangle->GetTexcoord(positiveIndex1) - triangle->GetTexcoord(negativeIndex2)), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(positiveIndex1), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex1) + kFactor1 * (triangle->GetNormal(positiveIndex1) - triangle->GetNormal(negativeIndex1)), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex2) + kFactor2 * (triangle->GetNormal(positiveIndex1) - triangle->GetNormal(negativeIndex2)), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(positiveIndex1), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex1) + kFactor1 * (triangle->GetColor(positiveIndex1) - triangle->GetColor(negativeIndex1)), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex2) + kFactor2 * (triangle->GetColor(positiveIndex1) - triangle->GetColor(negativeIndex2)), thirdOrderIndex);
				tmp->Update();
				front.push_back(tmp);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((intersection2 - triangle->GetVertex(negativeIndex1)).CrossProduct(intersection1 - triangle->GetVertex(negativeIndex1)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				// Fill the back triangles
				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(negativeIndex1), firstOrderIndex);
				tmp->SetVertex(intersection2, secondOrderIndex);
				tmp->SetVertex(intersection1, thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex2) + kFactor2 * (triangle->GetTexcoord(positiveIndex1) - triangle->GetTexcoord(negativeIndex2)), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1) + kFactor1 * (triangle->GetTexcoord(positiveIndex1) - triangle->GetTexcoord(negativeIndex1)), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex1), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex2) + kFactor2 * (triangle->GetNormal(positiveIndex1) - triangle->GetNormal(negativeIndex2)), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex1) + kFactor1 * (triangle->GetNormal(positiveIndex1) - triangle->GetNormal(negativeIndex1)), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex1), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex2) + kFactor2 * (triangle->GetColor(positiveIndex1) - triangle->GetColor(negativeIndex2)), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex1) + kFactor1 * (triangle->GetColor(positiveIndex1) - triangle->GetColor(negativeIndex1)), thirdOrderIndex);
				tmp->Update();
				back.push_back(tmp);


				// Default index
				firstOrderIndex = 0;
				secondOrderIndex = 1;
				thirdOrderIndex = 2;

				// Check the element order 
				if(triangle->GetFlatNormal().DotProduct((intersection2 - triangle->GetVertex(negativeIndex2)).CrossProduct(triangle->GetVertex(negativeIndex1) - triangle->GetVertex(negativeIndex2)).Normalize()) < 0) {
					secondOrderIndex = 2;
					thirdOrderIndex = 1;
				}

				tmp = new CBSPTriangle(*triangle);
				tmp->SetId(triangle->GetId());
				tmp->SetVertex(triangle->GetVertex(negativeIndex2), firstOrderIndex);
				tmp->SetVertex(intersection2, secondOrderIndex);
				tmp->SetVertex(triangle->GetVertex(negativeIndex1), thirdOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex2), firstOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex2) + kFactor2 * (triangle->GetTexcoord(positiveIndex1) - triangle->GetTexcoord(negativeIndex2)), secondOrderIndex);
				tmp->SetTexcoord(triangle->GetTexcoord(negativeIndex1), thirdOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex2), firstOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex2) + kFactor2 * (triangle->GetNormal(positiveIndex1) - triangle->GetNormal(negativeIndex2)), secondOrderIndex);
				tmp->SetNormal(triangle->GetNormal(negativeIndex1), thirdOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex2), firstOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex2) + kFactor2 * (triangle->GetColor(positiveIndex1) - triangle->GetColor(negativeIndex2)), secondOrderIndex);
				tmp->SetColor(triangle->GetColor(negativeIndex1), thirdOrderIndex);
				tmp->Update();
				back.push_back(tmp);

				// Delete the original triangle
				delete triangle;
			} else
				back.push_back(triangle);

			break;
		}
	}

	void CBSPTreeNode::CalculateBoundingBox(const std::vector<CBSPTriangle*> triangles) {
		LOG_TRACE();

		// Initial values
		CVector3F minPosition = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CVector3F maxPosition = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		// Read each vertex for each triangle
		for(auto it = triangles.begin(); it != triangles.end(); ++it) {
			for(int i = 0; i < 3; ++i) {
				minPosition.x = std::min(minPosition.x, (*it)->GetVertex(i).x);
				minPosition.y = std::min(minPosition.y, (*it)->GetVertex(i).y);
				minPosition.z = std::min(minPosition.z, (*it)->GetVertex(i).z);

				maxPosition.x = std::max(maxPosition.x, (*it)->GetVertex(i).x);
				maxPosition.y = std::max(maxPosition.y, (*it)->GetVertex(i).y);
				maxPosition.z = std::max(maxPosition.z, (*it)->GetVertex(i).z);
			}
		}

		this->m_boundingBox->SetMinMax(minPosition, maxPosition);
	}

	void CBSPTreeNode::BuildInitialPortal() {
		LOG_TRACE();

		float distanceBoundingBoxToPlane = this->m_plane.PointDistance(this->m_boundingBox->GetPosition());
		CVector3F portalCenter = this->m_boundingBox->GetPosition() + (this->m_plane.GetNormal() * distanceBoundingBoxToPlane);

		CVector3F tempAxis;
		// Get the valid axis 
		if(std::fabsf(this->m_plane.GetNormal().y) > std::fabsf(this->m_plane.GetNormal().z)) {
			if(std::fabsf(this->m_plane.GetNormal().z) < std::fabsf(this->m_plane.GetNormal().x))
				tempAxis.z = 1;
			else
				tempAxis.x = 1;
		} else {
			if(std::fabsf(this->m_plane.GetNormal().y <= std::fabsf(this->m_plane.GetNormal().x))) {
				tempAxis.y = 1;
			} else {
				tempAxis.x = 1;
			}
		}

		// Get the right and up vectors
		CVector3F u = tempAxis.CrossProduct(this->m_plane.GetNormal()).Normalize();
		CVector3F v = this->m_plane.GetNormal().CrossProduct(u).Normalize();

		// Get the correct u and v length
		CVector3F boxHalfLength = this->m_boundingBox->GetMax() - this->m_boundingBox->GetPosition();
		u *= boxHalfLength.Length();
		v *= boxHalfLength.Length();

		// Create the vertex
		CVector3F vertex[4];
		vertex[0] = portalCenter - u + v; // top left
		vertex[1] = portalCenter + u + v; // top right
		vertex[3] = portalCenter + u - v; // bottom right
		vertex[4] = portalCenter - u - v; // bottom left

		// Create the portal
	}

	CBSPTriangle * CBSPTreeNode::FindBestSplitter(const std::vector<CBSPTriangle*> & triangles) {
		LOG_TRACE();

		float score;
		float bspScore = DEFAULT_BSP_SCORE;
		CBSPTriangle * chosenSplitter = nullptr;

		// No splitter chosen if there is no triangles!
		if(triangles.size() == 0)
			return nullptr;

		for(auto it = triangles.begin(); it != triangles.end(); ++it) {
			if(!(*it)->IsSeparationPlane()) {
				int frontCounter = 0;
				int backCounter = 0;
				int splitCounter = 0;
				int coplanarCounter = 0;

				for(auto it2 = triangles.begin(); it2 != triangles.end(); ++it2) {
					// Avoid comparing the same triangle or an already used triangle as a splitter
					if((*it) != (*it2) && !(*it2)->IsSeparationPlane()) {
						// Classify test
						switch((*it2)->Classify((*it)->GetPlane())) {
						case BSP_TRIANGLE_ORIENTATION_FRONT:
							++frontCounter;
							break;

						case BSP_TRIANGLE_ORIENTATION_BACK:
							++backCounter;
							break;

						case BSP_TRIANGLE_ORIENTATION_SPANNING:
							++splitCounter;
							break;

						default:
							++coplanarCounter;
							break;
						}
					}
				}

				// Score calculation
				score = (float)(2 * splitCounter + abs(frontCounter - backCounter) + coplanarCounter);

				// Update the chosen splitter if necessary
				if(score >= 0 && score < bspScore) {
					bspScore = score;
					chosenSplitter = (*it);
				}
			}
		}

		// Set the triangle with the same plane as the chosen splitter to used
		if(chosenSplitter != nullptr) {
			for(auto it = triangles.begin(); it != triangles.end(); ++it) {
				if((*it)->Classify(chosenSplitter->GetPlane()) == BSP_TRIANGLE_ORIENTATION_COPLANAR && (*it)->GetPlane().GetNormal().DotProduct(chosenSplitter->GetPlane().GetNormal()) >= 0)
					(*it)->SetSeparationPlane(true);
			}
		}

		return chosenSplitter;
	}
}