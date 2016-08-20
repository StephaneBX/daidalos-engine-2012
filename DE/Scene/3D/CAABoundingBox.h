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

#ifndef __CAABOUNDINGBOX_H
#define __CAABOUNDINGBOX_H

#include <array>
#include "../../Core/Utility/CVector3.h"
#include "../../Scene/3D/CPlane3D.h"
#include "../../Scene/3D/IShape3D.h"

namespace DE {
	enum EAABoundingBoxPlane {
		AABOUNDINGBOX_PLANE_LEFT,
		AABOUNDINGBOX_PLANE_RIGHT,
		AABOUNDINGBOX_PLANE_UP,
		AABOUNDINGBOX_PLANE_DOWN,
		AABOUNDINGBOX_PLANE_TOP,
		AABOUNDINGBOX_PLANE_BOTTOM
	};

	/**
	* The IShape3D implementation of an axis-aligned bounding box.
	*/
	template <class T> class CAABoundingBox : public IShape3D < T > {
	private:
		CVector3<T> m_size;
		CVector3<T> m_min;
		CVector3<T> m_max;
		std::array<CPlane3DF, 6> m_planes;

	public:
		/**
		* Constructor
		*/
		CAABoundingBox();

		/**
		* Constructor
		* @param position The center position
		* @param size The size
		*/
		CAABoundingBox(const CVector3<T> & position, const CVector3<T> & size);

		/**
		* Destructor
		*/
		~CAABoundingBox();

		/**
		* Set the position
		* @param position The new position
		*/
		void SetPosition(const CVector3<T> & position);

		/**
		* Set the size
		* @param size The size
		*/
		void SetSize(const CVector3<T> & size);

		/**
		* The the min and max positions
		* @param minPosition The min position
		* @param maxPosition The max position
		*/
		void SetMinMax(const CVector3<T> & minPosition, const CVector3<T> & maxPosition);

		/**
		* Build the planes
		*/
		void BuildPlanes();

		/**
		* Get the offset with a plane
		* @param plane The plane
		* @return The offset
		*/
		const float GetInnerDepth(const CPlane3D<T> & plane) const;

		/**
		* Get the min coordinate
		* @return The min coordinate
		*/
		const CVector3<T> & GetMin() const;

		/**
		* Get the max coordinate
		* @return The max coordinate
		*/
		const CVector3<T> & GetMax() const;
	};

	template <class T> CAABoundingBox<T>::CAABoundingBox() : IShape3D<T>() {
		this->m_type = SHAPE_AABOX;
	}

	template <class T> CAABoundingBox<T>::CAABoundingBox(const CVector3<T> & position, const CVector3<T> & size) : IShape3D<T>(position) {
		this->m_position = position;
		this->m_type = SHAPE_AABOX;

		SetSize(size);
	}

	template <class T> CAABoundingBox<T>::~CAABoundingBox() {}

	template <class T> void CAABoundingBox<T>::SetPosition(const CVector3<T> & position) {
		IShape3D::SetPosition(position);
		SetSize(this->m_size);
	}

	template <class T> void CAABoundingBox<T>::SetSize(const CVector3<T> & size) {
		this->m_size = size;
		this->m_min = this->m_position - (this->m_size / 2.0f);
		this->m_max = this->m_position + (this->m_size / 2.0f);

		BuildPlanes();
	}

	template <class T> void CAABoundingBox<T>::SetMinMax(const CVector3<T> & minPosition, const CVector3<T> & maxPosition) {
		this->m_min = minPosition;
		this->m_max = maxPosition;
		this->m_size = maxPosition - minPosition;
		this->m_position = minPosition + (this->m_size / 2.0f);

		BuildPlanes();
	}

	template <class T> void CAABoundingBox<T>::BuildPlanes() {
		CVector3<T> left[3] = { CVector3<T>(this->m_min.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_min.x, this->m_min.y, this->m_max.z), CVector3<T>(this->m_min.x, this->m_max.y, this->m_max.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_LEFT].Build(left);

		CVector3<T> right[3] = { CVector3<T>(this->m_max.x, this->m_min.y, this->m_max.z), CVector3<T>(this->m_max.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_max.x, this->m_max.y, this->m_min.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_RIGHT].Build(right);

		CVector3<T> up[3] = { CVector3<T>(this->m_max.x, this->m_min.y, this->m_max.z), CVector3<T>(this->m_min.x, this->m_min.y, this->m_max.z), CVector3<T>(this->m_min.x, this->m_max.y, this->m_max.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_UP].Build(up);

		CVector3<T> down[3] = { CVector3<T>(this->m_min.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_max.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_max.x, this->m_max.y, this->m_min.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_DOWN].Build(down);

		CVector3<T> top[3] = { CVector3<T>(this->m_min.x, this->m_max.y, this->m_max.z), CVector3<T>(this->m_max.x, this->m_max.y, this->m_max.z), CVector3<T>(this->m_max.x, this->m_max.y, this->m_min.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_TOP].Build(top);

		CVector3<T> bottom[3] = { CVector3<T>(this->m_min.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_max.x, this->m_min.y, this->m_min.z), CVector3<T>(this->m_max.x, this->m_min.y, this->m_max.z) };
		this->m_planes[AABOUNDINGBOX_PLANE_BOTTOM].Build(bottom);
	}

	template <class T> const float CAABoundingBox<T>::GetInnerDepth(const CPlane3D<T> & plane) const {
		CVector3<T> endSegment = this->m_position + CVector3<T>(plane.GetNormal().x * this->m_size.x, plane.GetNormal().y * this->m_size.y, plane.GetNormal().z * this->m_size.z);
		float factor = 1.0f;
		CVector3<T> intersection;

		// Check the intersection between each plane and the segment (plane - position)
		for(auto it = this->m_planes.begin(); it != this->m_planes.end(); ++it) {
			intersection = (*it).IntersectSegment(this->m_position, endSegment, factor);
			if(factor > 0 && factor < 1.0f) {
				break;
			}
		}

		return (intersection - this->m_position).Length();
	}

	template <class T> const CVector3<T> & CAABoundingBox<T>::GetMin() const {
		return this->m_min;
	}

	template <class T> const CVector3<T> & CAABoundingBox<T>::GetMax() const {
		return this->m_max;
	}

	typedef CAABoundingBox<int> CAABoundingBoxI;
	typedef CAABoundingBox<float> CAABoundingBoxF;
}

#endif
