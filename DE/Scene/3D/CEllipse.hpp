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

#ifndef __CELLIPSE_HPP
#define __CELLIPSE_HPP

#include "../../Core/Utility/CVector2.hpp"
#include "../../Scene/3D/IShape3D.hpp"

namespace daidalosengine {
	/**
	* The IShape3D implementation of an ellipse.
	*/
	template <class T> class CEllipse : public IShape3D < T > {
	private:
		CVector2<T> m_radius;

	public:
		/**
		* Constructor
		*/
		CEllipse();

		/**
		* Constructor
		* @param position The position
		* @param offset The offset
		*/
		CEllipse(const CVector3<T> & position, const CVector2<T> & offset);

		/**
		* Destructor
		*/
		virtual ~CEllipse();

		/**
		* Set the radius
		* @param radius The radius
		*/
		void SetRadius(const CVector2<T> & radius);

		/**
		* Get the offset with a plane
		* @param plane The plane
		* @return The offset
		*/
		virtual float GetInnerDepth(const CPlane3D<T> & plane) const override;
	};

	template <class T> CEllipse<T>::CEllipse() : IShape3D<T>(SHAPE_ELLIPSE) {}

	template <class T> CEllipse<T>::CEllipse(const CVector3<T> & position, const CVector2<T> & radius) : IShape3D<T>(position, SHAPE_ELLIPSE) {
		this->m_radius = radius;
	}


	template <class T> CEllipse<T>::~CEllipse() {}

	template <class T> void CEllipse<T>::SetRadius(const CVector2<T> & radius) {
		this->m_radius = radius;
	}

	template <class T> float CEllipse<T>::GetInnerDepth(const CPlane3D<T> & plane) const {
		return this->m_radius.x * plane.GetNormal().x * plane.GetNormal().x + this->m_radius.y * plane.GetNormal().y * plane.GetNormal().y + this->m_radius.x * plane.GetNormal().z * plane.GetNormal().z;
	}

	typedef CEllipse<int> CEllipseI;
	typedef CEllipse<float> CEllipseF;
}

#endif
