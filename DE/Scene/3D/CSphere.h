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

#ifndef __CSPHERE_H
#define __CSPHERE_H

#include "../../Scene/3D/IShape3D.h"

namespace DE {
	/**
	* The IShape3D implementation of a sphere.
	*/
	template <class T> class CSphere : public IShape3D < T > {
	private:
		float m_radius;

	public:
		/**
		* Constructor
		*/
		CSphere();

		/**
		* Constructor
		* @param position The position
		* @param radius The radius
		*/
		CSphere(const CVector3<T> & position, const float radius);

		/**
		* Destructor
		*/
		virtual ~CSphere();

		/**
		* Set the radius
		* @param radius The radius
		*/
		void SetRadius(const float radius);

		/**
		* Get the offset with a plane
		* @param plane The plane
		* @return The offset
		*/
		virtual const float GetInnerDepth(const CPlane3D<T> & plane) const override;

		/**
		* Get the offset of the sphere with a plane
		* @return The offset
		*/
		const float GetInnerDepth() const;

		/**
		* Get the union sphere of the current sphere with the given sphere
		* @param sphere The second sphere
		* @return The union sphere
		*/
		CSphere GetUnion(const CSphere & sphere) const;
	};

	template <class T> CSphere<T>::CSphere() : IShape3D<T>() {
		this->m_type = SHAPE_SPHERE;
	}

	template <class T> CSphere<T>::CSphere(const CVector3<T> & position, const float radius) : IShape3D<T>(position) {
		this->m_type = SHAPE_SPHERE;
		this->m_position = position;
		this->m_radius = radius;
	}

	template <class T> CSphere<T>::~CSphere() {}

	template <class T> void CSphere<T>::SetRadius(const float radius) {
		this->m_radius = radius;
	}

	template <class T> const float CSphere<T>::GetInnerDepth(const CPlane3D<T> & plane) const {
		return this->m_radius;
	}

	template <class T> const float CSphere<T>::GetInnerDepth() const {
		return this->m_radius;
	}

	template <class T> CSphere<T> CSphere<T>::GetUnion(const CSphere<T> & sphere) const {
		// Get the minimum and maximum values
		CVector3<T> direction = (sphere.GetPosition() - this->m_position).Normalize();
		CVector3<T> start = this->m_position - (direction * this->m_radius);
		CVector3<T> end = sphere.GetPosition() + (direction * sphere.GetInnerDepth());

		return CSphere<T>((start + end) / 2.0f, (end - start).Length() / 2.0f);
	}

	typedef CSphere<int> CSphereI;
	typedef CSphere<float> CSphereF;
}

#endif
