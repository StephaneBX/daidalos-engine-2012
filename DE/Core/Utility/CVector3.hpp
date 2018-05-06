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

#ifndef __CVECTOR3_HPP
#define __CVECTOR3_HPP

#include <math.h>
#include <sstream>
#include "../../Core/Utility/CTokenizer.hpp"
#include "../../Core/Core.hpp"

namespace daidalosengine {
	/**
	* CVector3 class
	* Basic math class for vectors of size 3
	*/
	template <class T> class CVector3 {
	public:
		T x;
		T y;
		T z;

		CVector3();
		CVector3(const CVector3<T> & vec);
		CVector3(const T x, const T y, const T z);
		CVector3(const T * const f);
		const CVector3<T> operator+(const CVector3<T> & vec) const;
		const CVector3<T> operator-(const CVector3<T> & vec) const;
		const CVector3<T> operator*(const CVector3<T> & vec) const;
		const CVector3<T> operator/(const CVector3<T> & vec) const;
		const CVector3<T> operator-() const;
		const CVector3<T> Normalize() const;
		const CVector3<T> & operator=(const CVector3<T> & vec);
		const CVector3<T> & operator+=(const CVector3<T> & vec);
		const CVector3<T> & operator-=(const CVector3<T> & vec);
		const CVector3<T> & operator*=(const CVector3<T> & vec);
		const CVector3<T> & operator*=(const T number);
		const CVector3<T> & operator/=(const CVector3<T> & vec);
		const CVector3<T> CrossProduct(const CVector3<T> & vec) const;
		const CVector3<T> LinearInterpolation(const CVector3<T> & vec, const float interpolation) const;
		const CVector3<T> CosineInterpolation(const CVector3<T> & vec, const float interpolation) const;
		const CVector3<T> Min(const CVector3<T> & vec) const;
		const CVector3<T> Max(const CVector3<T> & vec) const;
		const bool operator==(const CVector3<T> & vec) const;
		const bool operator!=(const CVector3<T> & vec) const;
		const T Length() const;
		const T DotProduct(const CVector3<T> & vec) const;
		const T Distance(const CVector3<T> & vec) const;
		const std::string ToString(const std::string separator = ":") const;
		operator T * ();
		operator const T * () const;
		static const CVector3<T> CrossProduct(const CVector3<T> & vec1, const CVector3<T> & vec2);
		static const CVector3<T> LinearInterpolation(const CVector3<T> & vec1, const CVector3<T> & vec2, const float interpolation);
		static const CVector3<T> CosineInterpolation(const CVector3<T> & vec1, const CVector3<T> & vec2, const float interpolation);
		static const CVector3<T> Min(const CVector3<T> & vec1, const CVector3<T> & vec2);
		static const CVector3<T> Max(const CVector3<T> & vec1, const CVector3<T> & vec2);
		static const CVector3<T> GetFrom(const std::string text, const CVector3<T> & defaultValue = CVector3<T>(), const std::string separator = ":");
		static const T Length(const CVector3<T> & vec);
		static const T DotProduct(const CVector3<T> & vec1, const CVector3<T> & vec2);
		static const T Distance(const CVector3<T> & vec1, const CVector3<T> & vec2);
		static void Normalize(CVector3<T> & vec);
	};

	template <class T> const CVector3<T> operator*(const CVector3<T> & vector, const T number);
	template <class T> const CVector3<T> operator*(const T number, const CVector3<T> & vector);
	template <class T> const CVector3<T> operator/(const CVector3<T> & vector, const T number);
	template <class T> const CVector3<T> operator/(const T number, const CVector3<T> & vector);

	/**
	* Constructor
	*/
	template <class T> inline CVector3<T>::CVector3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	/**
	* Constructor
	* @param vec a CVector3
	*/
	template <class T> inline CVector3<T>::CVector3(const CVector3<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	/**
	* Constructor
	* @param x The x coordinate
	* @param y The y coordinate
	* @param z The z coordinate
	*/
	template <class T> inline CVector3<T>::CVector3(const T x, const T y, const T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/**
	* Constructor
	* @param f The coordinate
	*/
	template <class T> inline CVector3<T>::CVector3(const T * const f) {
		this->x = f[0];
		this->y = f[1];
		this->z = f[2];
	}

	/**
	* + operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::operator+(const CVector3<T> & vec) const {
		CVector3<T> tmp(*this);
		tmp.x += vec.x;
		tmp.y += vec.y;
		tmp.z += vec.z;
		return tmp;
	}

	/**
	* - operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::operator-(const CVector3<T> & vec) const {
		CVector3<T> tmp(*this);
		tmp.x -= vec.x;
		tmp.y -= vec.y;
		tmp.z -= vec.z;
		return tmp;
	}

	/**
	* * operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::operator*(const CVector3<T> & vec) const {
		CVector3<T> tmp(*this);
		tmp.x *= vec.x;
		tmp.y *= vec.y;
		tmp.z *= vec.z;
		return tmp;
	}

	/**
	* / operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::operator/(const CVector3<T> & vec) const {
		CVector3<T> tmp(*this);
		tmp.x /= vec.x;
		tmp.y /= vec.y;
		tmp.z /= vec.z;
		return tmp;
	}

	/**
	* - operator override
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::operator-() const {
		CVector3<T> tmp(*this);
		tmp.x *= -1;
		tmp.y *= -1;
		tmp.z *= -1;
		return tmp;
	}

	/**
	* Normalize the vector CVector3
	*/
	template <class T> inline const CVector3<T> CVector3<T>::Normalize() const {
		CVector3<T> tmp = *this;
		T length = Length();

		if(length > 0) {
			tmp = CVector3(this->x / length, this->y / length, this->z / length);
		}

		return tmp;
	}

	/**
	* = operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator=(const CVector3<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}

	/**
	* += operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator+=(const CVector3<T> & vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}

	/**
	* -= operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator-=(const CVector3<T> & vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}

	/**
	* *= operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator*=(const CVector3<T> & vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}

	/**
	* *= operator override
	* @param number A number
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator*=(const T number) {
		this->x *= number;
		this->y *= number;
		this->z *= number;
		return *this;
	}

	/**
	* /= operator override
	* @param vec CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> & CVector3<T>::operator/=(const CVector3<T> & vec) {
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}

	/**
	* Perform a cross product
	* @param vec A CVector3
	* @return The cross product
	*/
	template <class T> inline const CVector3<T> CVector3<T>::CrossProduct(const CVector3<T> & vec) const {
		return CVector3<T>(this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x);
	}

	/**
	* Do a linear interpolation of two vectors
	* @param vec The second vector
	* @param interpolation The interpolation value
	* @return The result vector
	*/
	template <class T> inline const CVector3<T> CVector3<T>::LinearInterpolation(const CVector3<T> & vec, const float interpolation) const {
		return *this + interpolation * (vec - *this);
	}

	/**
	* Do a cosine interpolation of two vectors
	* @param vec The second vector
	* @param interpolation The interpolation value
	* @return The result vector
	*/
	template <class T> inline const CVector3<T> CVector3<T>::CosineInterpolation(const CVector3<T> & vec, const float interpolation) const {
		CVector3<T> ret;

		float interpolation2 = (1 - std::cosf(interpolation * PI)) / 2.0f;

		ret.x = this->x + interpolation2 * (vec.x - this->x);
		ret.y = this->y + interpolation2 * (vec.y - this->y);
		ret.z = this->z + interpolation2 * (vec.z - this->z);

		return ret;
	}

	/**
	* Return the min values
	* @param vec The second vector
	* @return A vector containing the minimum values
	*/
	template <class T> inline const CVector3<T> CVector3<T>::Min(const CVector3<T> & vec) const {
		CVector3<T> ret;
		ret.x = std::min(this->x, vec.x);
		ret.y = std::min(this->y, vec.y);
		ret.z = std::min(this->z, vec.z);

		return ret;
	}

	/**
	* Return the max values
	* @param vec The second vector
	* @return A vector containing the maximum values
	*/
	template <class T> inline const CVector3<T> CVector3<T>::Max(const CVector3<T> & vec) const {
		CVector3<T> ret;
		ret.x = std::max(this->x, vec.x);
		ret.y = std::max(this->y, vec.y);
		ret.z = std::max(this->z, vec.z);

		return ret;
	}

	/**
	* == operator override
	* @param vec CVector3
	* @return The equal result
	*/
	template <class T> inline const bool CVector3<T>::operator==(const CVector3<T> & vec) const {
		return std::abs(this->x - vec.x) < EPSILON && std::abs(this->y - vec.y) < EPSILON && std::abs(this->z - vec.z) < EPSILON;
	}

	/**
	* != operator override
	* @param vec CVector3
	* @return The !equal result
	*/
	template <class T> inline const bool CVector3<T>::operator!=(const CVector3<T> & vec) const {
		return !(*this == vec);
	}

	/**
	* Get the length
	* @return The length
	*/
	template <class T> inline const T CVector3<T>::Length() const {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	/**
	* Perform a dot product
	* @param vec A CVector3
	* @return The dot product
	*/
	template <class T> inline const T CVector3<T>::DotProduct(const CVector3<T> & vec) const {
		return this->x * vec.x + this->y * vec.y + this->z * vec.z;
	}

	/**
	* Perform a distance calculation
	* @param vec A CVector3
	* @return The distance
	*/
	template <class T> inline const T CVector3<T>::Distance(const CVector3<T> & vec) const {
		return CVector3<T>(vec.x - this->x, vec.y - this->y, vec.z - this->z).Length();
	}

	/**
	* Get a string from a CVector3
	* @param separator The separator
	* @return A string
	*/
	template <class T> inline const std::string CVector3<T>::ToString(const std::string separator) const {
		std::stringstream ss;

		ss << this->x << separator << this->y << separator << this->z;
		return ss.str();
	}

	/**
	* T * cast operator override
	* @return A T pointer
	*/
	template <class T> inline CVector3<T>::operator T * () {
		return &this->x;
	}

	/**
	* T * cast operator override
	* @return A T pointer
	*/
	template <class T> inline CVector3<T>::operator const T * () const {
		return &this->x;
	}

	/**
	* Perform a cross product
	* @param vec1 A CVector3
	* @param vec2 A CVector3
	* @return The cross product
	*/
	template <class T> inline const CVector3<T> CVector3<T>::CrossProduct(const CVector3<T> & vec1, const CVector3<T> & vec2) {
		return vec1.CrossProduct(vec2);
	}

	/**
	* Do a linear interpolation of two vectors
	* @param vec1 The first vector
	* @param vec2 The second vector
	* @param interpolation The interpolation value
	* @return The vector
	*/
	template <class T> inline const CVector3<T> CVector3<T>::LinearInterpolation(const CVector3<T> & vec1, const CVector3<T> & vec2, const float interpolation) {
		return vec1.LinearInterpolation(vec2, interpolation);
	}

	/**
	* Do a cosine interpolation of two vectors
	* @param vec1 The first vector
	* @param vec2 The second vector
	* @param interpolation The interpolation value
	* @return The vector
	*/
	template <class T> inline const CVector3<T> CVector3<T>::CosineInterpolation(const CVector3<T> & vec1, const CVector3<T> & vec2, const float interpolation) {
		return vec1.CosineInterpolation(vec2, interpolation);
	}

	/**
	* Return the min values
	* @param vec1 The first vector
	* @param vec2 The second vector
	* @return A vector containing the minimum values
	*/
	template <class T> inline const CVector3<T> CVector3<T>::Min(const CVector3<T> & vec1, const CVector3<T> & vec2) {
		return vec1.Min(vec2);
	}

	/**
	* Return the max values
	* @param vec1 The first vector
	* @param vec2 The second vector
	* @return A vector containing the maximum values
	*/
	template <class T> inline const CVector3<T> CVector3<T>::Max(const CVector3<T> & vec1, const CVector3<T> & vec2) {
		return vec1.Max(vec2);
	}

	/**
	* Get a CVector3 from a string
	* @param text The text
	* @param defaultValue The default value
	* @param separator The separator
	* @return The vector
	*/
	template <class T> inline const CVector3<T> CVector3<T>::GetFrom(const std::string text, const CVector3<T> & defaultValue, const std::string separator) {
		CVector3<T> result;
		CTokenizer token;

		token.AddLine(text);

		if(text.length() > 0 && token.Tokenize(separator, 3) && token.GetTokens().size() == 3) {
			std::istringstream ss;

			ss.clear();
			ss.str(token.GetTokens()[0]);
			ss >> result.x;

			ss.clear();
			ss.str(token.GetTokens()[1]);
			ss >> result.y;

			ss.clear();
			ss.str(token.GetTokens()[2]);
			ss >> result.z;
		} else {
			result = defaultValue;
		}

		return result;
	}

	/**
	* Get the length of a CVector3
	* @param vec A CVector3
	* @return The length
	*/
	template <class T> inline const T CVector3<T>::Length(const CVector3<T> & vec) {
		return vec.Length();
	}

	/**
	* Perform a dot product
	* @param vec1 A CVector3
	* @param vec2 A CVector3
	* @return The dot product
	*/
	template <class T> inline const T CVector3<T>::DotProduct(const CVector3<T> & vec1, const CVector3<T> & vec2) {
		return vec1.DotProduct(vec2);
	}

	/**
	* Perform a distance calculation
	* @param vec1 A CVector3
	* @param vec2 A CVector3
	* @return The distance
	*/
	template <class T> inline const T CVector3<T>::Distance(const CVector3<T> & vec1, const CVector3<T> & vec2) {
		return vec1.Distance(vec2);
	}

	/**
	* Normalize a CVector3
	* @param vec A CVector3
	*/
	template <class T> inline void CVector3<T>::Normalize(CVector3<T> & vec) {
		return vec.Normalize();
	}

	/**
	* * operator override
	* @param vector A CVector3
	* @param number A number
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> operator*(const CVector3<T> & vector, const T number) {
		CVector3<T> tmp(vector);
		tmp.x *= number;
		tmp.y *= number;
		tmp.z *= number;
		return tmp;
	}

	/**
	* * operator override
	* @param number A number
	* @param vector A CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> operator*(const T number, const CVector3<T> & vector) {
		return vector * number;
	}

	/**
	* / operator override
	* @param vector A CVector3
	* @param number A number
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> operator/(const CVector3<T> & vector, const T number) {
		CVector3<T> tmp(vector);
		tmp.x /= number;
		tmp.y /= number;
		tmp.z /= number;
		return tmp;
	}

	/**
	* / operator override
	* @param number A number
	* @param vector A CVector3
	* @return The CVector3
	*/
	template <class T> inline const CVector3<T> operator/(const T number, const CVector3<T> & vector) {
		return CVector3<T>(number / vector.x, number / vector.y, number / vector.z);
	}

	typedef CVector3<int> CVector3I;
	typedef CVector3<float> CVector3F;
}

#endif
