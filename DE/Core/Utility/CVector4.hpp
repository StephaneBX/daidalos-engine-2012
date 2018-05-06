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

#ifndef __CVECTOR4_HPP
#define __CVECTOR4_HPP

#include "../../Core/Utility/CVector3.hpp"

namespace daidalosengine {
	/**
	* CVector4 class
	* Basic math class for vectors of size 4
	*/
	template <class T> class CVector4 {
	public:
		T x;
		T y;
		T z;
		T w;

		CVector4();
		CVector4(const CVector4<T> & vec);
		CVector4(const CVector3<T> & vec, const T w);
		CVector4(const T x, const T y, const T z, const T w);
		CVector4(const T * const f);
		const CVector4<T> operator+(const CVector4<T> & vec) const;
		const CVector4<T> operator-(const CVector4<T> & vec) const;
		const CVector4<T> operator*(const CVector4<T> & vec) const;
		const CVector4<T> operator/(const CVector4<T> & vec) const;
		const CVector4<T> operator-() const;
		const CVector4<T> Normalize() const;
		const CVector4<T> & operator=(const CVector4<T> & vec);
		const CVector4<T> & operator+=(const CVector4<T> & vec);
		const CVector4<T> & operator-=(const CVector4<T> & vec);
		const CVector4<T> & operator*=(const CVector4<T> & vec);
		const CVector4<T> & operator/=(const CVector4<T> & vec);
		const bool operator==(const CVector4<T> & vec) const;
		const bool operator!=(const CVector4<T> & vec) const;
		const T Length() const;
		const std::string ToString(const std::string separator = ":") const;
		operator T * ();
		operator const T * () const;
		static const CVector4<T> GetFrom(const std::string text, const CVector4<T> & defaultValue = CVector4<T>(), const std::string separator = ":");
		static const T Length(const CVector4<T> & vec);
		static void Normalize(CVector4<T> & vec);
	};

	template <class T> const CVector4<T> operator*(const CVector4<T> & vec, const T number);
	template <class T> const CVector4<T> operator*(const T number, const CVector4<T> & vec);
	template <class T> const CVector4<T> operator/(const CVector4<T> & vec, const T number);
	template <class T> const CVector4<T> operator/(const T number, const CVector4<T> & vec);

	/**
	* Constructor
	*/
	template <class T> inline CVector4<T>::CVector4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	/**
	* Constructor
	* @param vec A CVector4
	*/
	template <class T> inline CVector4<T>::CVector4(const CVector4<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
	}

	/**
	* Constructor
	* @param vec A CVector3
	* @param w The w coordinate
	*/
	template <class T> inline CVector4<T>::CVector4(const CVector3<T> & vec, const T w) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = w;
	}

	/**
	* Constructor
	* @param x The x coordinate
	* @param y The y coordinate
	* @param z The z coordinate
	* @param w The w coordinate
	*/
	template <class T> inline CVector4<T>::CVector4(const T x, const T y, const T z, const T w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	* Constructor
	* @param f The coordinate
	*/
	template <class T> inline CVector4<T>::CVector4(const T * const f) {
		this->x = f[0];
		this->y = f[1];
		this->z = f[2];
		this->w = f[3];
	}

	/**
	* + operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::operator+(const CVector4<T> & vec) const {
		CVector4<T> tmp(*this);
		tmp.x += vec.x;
		tmp.y += vec.y;
		tmp.z += vec.z;
		tmp.w += vec.w;
		return tmp;
	}

	/**
	* - operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::operator-(const CVector4<T> & vec) const {
		CVector4<T> tmp(*this);
		tmp.x -= vec.x;
		tmp.y -= vec.y;
		tmp.z -= vec.z;
		tmp.w -= vec.w;
		return tmp;
	}

	/**
	* * operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::operator*(const CVector4<T> & vec) const {
		CVector4<T> tmp(*this);
		tmp.x *= vec.x;
		tmp.y *= vec.y;
		tmp.z *= vec.z;
		tmp.w *= vec.w;
		return tmp;
	}

	/**
	* / operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::operator/(const CVector4<T> & vec) const {
		CVector4<T> tmp(*this);
		tmp.x /= vec.x;
		tmp.y /= vec.y;
		tmp.z /= vec.z;
		tmp.w /= vec.w;
		return tmp;
	}

	/**
	* - operator override
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::operator-() const {
		CVector4<T> tmp(*this);
		tmp.x *= -1;
		tmp.y *= -1;
		tmp.z *= -1;
		tmp.w *= -1;
		return tmp;
	}

	/**
	* Normalize the vector CVector4
	*/
	template <class T> inline const CVector4<T> CVector4<T>::Normalize() const {
		CVector4<T> tmp = *this;
		T length = Length();

		if(length > 0) {
			tmp = CVector4(this->x / length, this->y / length, this->z / length, this->w / length);
		}

		return tmp;
	}

	/**
	* = operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> & CVector4<T>::operator=(const CVector4<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
		return *this;
	}

	/**
	* += operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> & CVector4<T>::operator+=(const CVector4<T> & vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		this->w += vec.w;
		return *this;
	}

	/**
	* -= operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> & CVector4<T>::operator-=(const CVector4<T> & vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		this->w -= vec.w;
		return *this;
	}

	/**
	* *= operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> & CVector4<T>::operator*=(const CVector4<T> & vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		this->w *= vec.w;
		return *this;
	}

	/**
	* /= operator override
	* @param vec CVector4
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> & CVector4<T>::operator/=(const CVector4<T> & vec) {
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;
		this->w /= vec.w;
		return *this;
	}

	/**
	* == operator override
	* @param vec CVector4
	* @return The equal result
	*/
	template <class T> inline const bool CVector4<T>::operator==(const CVector4<T> & vec) const {
		return std::abs(this->x - vec.x) < EPSILON && std::abs(this->y - vec.y) < EPSILON && std::abs(this->z - vec.z) < EPSILON && std::abs(this->w - vec.w) < EPSILON;
	}

	/**
	* != operator override
	* @param vec CVector4
	* @return The !equal result
	*/
	template <class T> inline const bool CVector4<T>::operator!=(const CVector4<T> & vec) const {
		return !(*this == vec);
	}

	/**
	* Get the length
	* @return The length
	*/
	template <class T> inline const T CVector4<T>::Length() const {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
	}

	/**
	* Get a string from a CVector3
	* @param separator The separator
	* @return A string
	*/
	template <class T> inline const std::string CVector4<T>::ToString(const std::string separator) const {
		std::stringstream ss;

		ss << this->x << separator << this->y << separator << this->z << separator << this->w;
		return ss.str();
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	template <class T> inline CVector4<T>::operator T * () {
		return &this->x;
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	template <class T> inline CVector4<T>::operator const T * () const {
		return &this->x;
	}

	/**
	* Get a CVector4 from a string
	* @param text The text
	* @param defaultValue The default value
	* @param separator The separator
	* @return The vector
	*/
	template <class T> inline const CVector4<T> CVector4<T>::GetFrom(const std::string text, const CVector4<T> & defaultValue, const std::string separator) {
		CVector4<T> result;
		CTokenizer token;
		token.AddLine(text);

		if(text.length() > 0 && token.Tokenize(separator, 4) && token.GetTokens().size() == 4) {
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

			ss.clear();
			ss.str(token.GetTokens()[3]);
			ss >> result.w;
		} else {
			result = defaultValue;
		}

		return result;
	}

	/**
	* Get the length of a CVector4
	* @param vec A CVector4
	* @return The length
	*/
	template <class T> inline const T CVector4<T>::Length(const CVector4<T> & vec) {
		return vec.Length();
	}

	/**
	* Normalize a CVector4
	*/
	template <class T> inline void CVector4<T>::Normalize(CVector4<T> & vec) {
		return vec.Normalize();
	}

	/**
	* * operator override
	* @param vec A CVector2
	* @param number A number
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> operator*(const CVector4<T> & vec, const T number) {
		CVector4<T> tmp(vec);
		tmp.x *= number;
		tmp.y *= number;
		tmp.z *= number;
		tmp.w *= number;
		return tmp;
	}

	/**
	* * operator override
	* @param number A number
	* @param vec A CVector2
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> operator*(const T number, const CVector4<T> & vec) {
		return vec * number;
	}

	/**
	* / operator override
	* @param vec A CVector2
	* @param number A number
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> operator/(const CVector4<T> & vec, const T number) {
		CVector4<T> tmp(vec);
		tmp.x /= number;
		tmp.y /= number;
		tmp.z /= number;
		tmp.w /= number;
		return tmp;
	}

	/**
	* / operator override
	* @param number A number
	* @param vec A CVector2
	* @return The CVector4
	*/
	template <class T> inline const CVector4<T> operator/(const T number, const CVector4<T> & vec) {
		return CVector4<T>(number / vec.x, number / vec.y, number / vec.z, number / vec.w);
	}

	typedef CVector4<int> CVector4I;
	typedef CVector4<float> CVector4F;
}

#endif
