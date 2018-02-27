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

#include "../../../../Core/Logger/ILogger.hpp"
#include "../../../../Scene/3D/Structure/BSP/CBSPTriangle.hpp"

namespace daidalosengine {
	CBSPTriangle::CBSPTriangle() {
		LOG_TRACE();

		this->m_separationPlane = false;
		this->m_visible = true;
		this->m_drawn = false;
	}

	void CBSPTriangle::SetSeparationPlane(const bool separationPlane) {
		LOG_TRACE();

		this->m_separationPlane = separationPlane;
	}

	void CBSPTriangle::SetVisible(const bool visible) {
		LOG_TRACE();

		this->m_visible = visible;
	}

	void CBSPTriangle::SetDrawn(const bool drawn) {
		LOG_TRACE();

		this->m_drawn = drawn;
	}

	void CBSPTriangle::SetPlane(const CPlane3DF & plane) {
		LOG_TRACE();

		this->m_plane = plane;
	}

	void CBSPTriangle::CreatePlane() {
		LOG_TRACE();

		this->m_plane = CPlane3DF(this->m_vertices);
		this->m_flatNormal = this->m_plane.GetNormal();
	}

	const bool CBSPTriangle::IsSeparationPlane() const {
		LOG_TRACE();

		return this->m_separationPlane;
	}

	const bool CBSPTriangle::IsVisible() const {
		LOG_TRACE();

		return this->m_visible;
	}

	const bool CBSPTriangle::IsDrawn() const {

		LOG_TRACE();
		return this->m_drawn;
	}

	const CPlane3DF & CBSPTriangle::GetPlane() const {
		LOG_TRACE();

		return this->m_plane;
	}

	EBSPTriangleOrientation CBSPTriangle::Classify(const CPlane3DF & plane) {
		LOG_TRACE();

		int frontCounter = 0;
		int backCounter = 0;
		int onCounter = 0;

		// Test the vertices
		for(int i = 0; i < 3; ++i) {
			switch(plane.ClassifyPoint(GetVertex(i))) {
			case POINT_ORIENTATION_POSITIVE_SIDE:
				++frontCounter;
				break;

			case POINT_ORIENTATION_NEGATIVE_SIDE:
				++backCounter;
				break;

			case POINT_ORIENTATION_ON_PLANE:
				++frontCounter;
				++backCounter;
				++onCounter;
				break;

			default:
				break;
			}
		}

		// Get the appropriate result
		if(onCounter == 3) {
			return BSP_TRIANGLE_ORIENTATION_COPLANAR;
		} else {
			if(frontCounter == 3) {
				return BSP_TRIANGLE_ORIENTATION_FRONT;
			} else {
				if(backCounter == 3)
					return BSP_TRIANGLE_ORIENTATION_BACK;
				else
					return BSP_TRIANGLE_ORIENTATION_SPANNING;
			}
		}
	}

	CPlane3DF * CBSPTriangle::GetPlanePointer() {
		LOG_TRACE();

		return &this->m_plane;
	}
}
