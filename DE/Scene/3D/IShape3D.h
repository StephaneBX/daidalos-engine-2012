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

#ifndef __ISHAPE3D_H
#define __ISHAPE3D_H

#include "../../Core/Utility/CVector3.h"
#include "../../Scene/3D/CPlane3D.h"

namespace DE {
	enum EShapeType {
		SHAPE_SPHERE,
		SHAPE_ELLIPSE,
		SHAPE_AABOX
	};

	/**
	* Basic class for 3d shapes.
	*/
	template <class T> class IShape3D {
	protected:
		CVector3<T> m_position;
		EShapeType m_type;

	public:
		/**
		* Constructor
		*/
		IShape3D();

		/**
		* Constructor
		* @param position The position
		*/
		IShape3D(const CVector3<T> & position);

		/**
		* Destructor
		*/
		~IShape3D();

		/**
		* Set the position
		* @param position The new position
		*/
		virtual void SetPosition(const CVector3<T> & position);

		/**
		* Check for a plane collision
		* @param plane The plane
		* @return The collision result
		*/
		virtual const bool IsCollision(const CPlane3D<T> & plane) const;

		/**
		* Check a collision between two shapes
		* @param shape The shape to test with
		* @return If a collision occured
		*/
		virtual const bool IsCollision(const IShape3D * shape) const;

		/**
		* Get the offset with a plane
		* @param plane The plane
		* @return The offset
		*/
		virtual const float GetInnerDepth(const CPlane3D<T> & plane) const;

		/**
		* Get the position
		* @return the position
		*/
		const CVector3<T> & GetPosition() const;

		/**
		* Get the type
		* @return The shape type
		*/
		EShapeType GetType() const;
	};

	template <class T> IShape3D<T>::IShape3D() {}

	template <class T> IShape3D<T>::IShape3D(const CVector3<T> & position) {
		this->m_position = position;
	}

	template <class T> IShape3D<T>::~IShape3D() {}

	template <class T> void IShape3D<T>::SetPosition(const CVector3<T> & position) {
		this->m_position = position;
	}

	template <class T> const bool IShape3D<T>::IsCollision(const CPlane3D<T> & plane) const {
		return plane.PointDistance(this->m_position) <= GetInnerDepth(plane);
	}

	template <class T> const bool IShape3D<T>::IsCollision(const IShape3D * shape) const {

		// Special case when the shapes are at the same exact spot
		if(this->m_position == shape->GetPosition()) {
			return true;
		}

		// Get the plane perpendicular to the shapes position in order to get the inner depth

		// Get the z axis vector for the plane
		CVector3F zAxis = shape->GetPosition() - this->m_position;
		CVector3F tmpYAxis = CVector3F(0, 1, 0);

		// Get the x axis vector for the plane
		CVector3F xAxis = tmpYAxis.CrossProduct(zAxis);

		// Get the y axis vector for the plane
		CVector3F yAxis = zAxis.CrossProduct(xAxis);

		CVector3F vertex[] = { (this->m_position + xAxis), this->m_position, (this->m_position + yAxis) };
		CPlane3DF plane(vertex);

		// Get the distance between the two centers
		float distance = this->m_position.Distance(shape->GetPosition());

		return distance <= (GetInnerDepth(plane) + shape->GetInnerDepth(plane));
	}

	template <class T> const float IShape3D<T>::GetInnerDepth(const CPlane3D<T> & plane) const {
		return 0;
	}

	template <class T> const CVector3<T> & IShape3D<T>::GetPosition() const {
		return this->m_position;
	}

	template <class T> EShapeType IShape3D<T>::GetType() const {
		return this->m_type;
	}

	typedef IShape3D<int> IShape3DI;
	typedef IShape3D<float> IShape3DF;
}

#endif
