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

#ifndef __CPLANE3D_H
#define __CPLANE3D_H

#include "../Core/Core.h"
#include "../Core/Utility/CVector3.h"

namespace DE
{
	enum EPointOrientation
	{
		POINT_ORIENTATION_POSITIVE_SIDE,
		POINT_ORIENTATION_NEGATIVE_SIDE,
		POINT_ORIENTATION_ON_PLANE
	};

	/**
	* Class that handle 3D planes.
	*/
	template <class T> class CPlane3D 
	{
	private:
		CVector3<T> m_normal;
		float m_constant;

	public:
		/**
		* Constructor
		*/
		CPlane3D();

		/**
		* Constructor
		* @param plane A CPlane3D
		*/
		CPlane3D(const CPlane3D<T> & plane);

		/**
		* Constructor
		* @param vertex A 3 sized CVector3 vertices pointer
		*/
		CPlane3D(const CVector3<T> * vertex);

		/**
		* Destructor
		*/
		~CPlane3D();

		/**
		* = operator override
		* @param plane A CPlane3D
		* @return The plane
		*/
		CPlane3D<T> & operator=(const CPlane3D<T> & plane);

		/**
		* == operator override
		* @param plane CPlane3D
		* @return The equal result
		*/
		const bool operator==(const CPlane3D<T> & plane) const;

		/**
		* != operator override
		* @param plane CPlane3D
		* @return The !equal result
		*/
		const bool operator!=(const CPlane3D<T> & plane) const;

		/**
		* Inverse the plane equation
		*/
		void Inverse();

		/**
		* Build the plane
		* @param vertex An array that contains 3 points
		*/
		void Build(const CVector3<T> * vertex);

		/**
		* Perform a distance calculation between the plane and a point
		* @param point The point
		* @return A positive value if the point is in front of the plane, a negative value if the point is behind the plane or a nul value if the point is on the plane
		*/
		const float PointDistance(const CVector3<T> & point) const;

		/**
		* Classify a point
		* @param point The point to test
		* @return The classify result, that can be a value in the EPointOrientation enumeration
		*/
		EPointOrientation ClassifyPoint(const CVector3<T> & point) const;

		/**
		* Intersect a segment with the plane a find the intersection point
		* The function suppose that the segment IS NOT PARALLEL to the plane
		* @param p1 The first point of the segment
		* @param p2 The second point of the segment
		* @param k The intersection factor
		* @return The intersection point
		*/
		CVector3<T> IntersectSegment(const CVector3<T> & p1, const CVector3<T> & p2, float & k) const;

		/**
		* Get the normal
		* @return The normal
		*/
		const CVector3<T> & GetNormal() const;
	};

	template <class T> CPlane3D<T>::CPlane3D() {}

	template <class T> CPlane3D<T>::CPlane3D(const CPlane3D<T> & plane) 
	{
		this->m_normal = plane.m_normal;
		this->m_constant = plane.m_constant;
	}

	template <class T> CPlane3D<T>::CPlane3D(const CVector3<T> * vertex) 
	{
		Build(vertex);
	}

	template <class T> CPlane3D<T>::~CPlane3D() {}
	
	template <class T> CPlane3D<T> & CPlane3D<T>::operator=(const CPlane3D<T> & plane)
	{
		this->m_normal = plane.m_normal;
		this->m_constant = plane.m_constant;
		return *this;
	}

	template <class T> const bool CPlane3D<T>::operator==(const CPlane3D<T> & plane) const
	{
		return this->m_normal == plane.m_normal && std::abs(this->m_constant - plane.m_constant) < EPSILON;
	}

	template <class T> const bool CPlane3D<T>::operator!=(const CPlane3D<T> & plane) const
	{
		return !(*this == plane);
	}

	template <class T> void CPlane3D<T>::Inverse()
	{
		this->m_normal *= -1.0f;
	}

	template <class T> void CPlane3D<T>::Build(const CVector3<T> * vertex)
	{
		// Normal calculation
		CVector3F ab = vertex[1] - vertex[0];
		CVector3F ac = vertex[2] - vertex[0];
		this->m_normal = ab.CrossProduct(ac).Normalize();

		// Constant calculation
		this->m_constant = -(vertex[1].x * this->m_normal.x + vertex[1].y * this->m_normal.y + vertex[1].z * this->m_normal.z);
	}

	template <class T> const float CPlane3D<T>::PointDistance(const CVector3<T> & point) const
	{
		return this->m_constant + this->m_normal.DotProduct(point);
	}

	template <class T> EPointOrientation CPlane3D<T>::ClassifyPoint(const CVector3<T> & point) const
	{
		float result = PointDistance(point);
		EPointOrientation orientation = POINT_ORIENTATION_ON_PLANE;
		if(result > EPSILON)
		{
			orientation = POINT_ORIENTATION_POSITIVE_SIDE;
		}
		else
		{
			if(result < -EPSILON)
				orientation = POINT_ORIENTATION_NEGATIVE_SIDE;
		}

		return orientation;
	}

	template <class T> CVector3<T> CPlane3D<T>::IntersectSegment(const CVector3<T> & p1, const CVector3<T> & p2, float & k) const
	{
		CVector3F u, result;
		u = p1 - p2;

		k = (this->m_normal.x * p1.x + this->m_normal.y * p1.y + this->m_normal.z * p1.z + this->m_constant) / (this->m_normal.x * u.x + this->m_normal.y * u.y + this->m_normal.z * u.z);

		result.x = p1.x - k * u.x;
		result.y = p1.y - k * u.y;
		result.z = p1.z - k * u.z;

		return result;
	}

	template <class T> const CVector3<T> & CPlane3D<T>::GetNormal() const
	{
		return this->m_normal;
	}

	typedef CPlane3D<int> CPlane3DI;
	typedef CPlane3D<float> CPlane3DF;
}

#endif
