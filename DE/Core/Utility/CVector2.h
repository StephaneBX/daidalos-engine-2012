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

#ifndef __CVECTOR2_H
#define __CVECTOR2_H

#include <math.h>
#include "../../Core/Utility/CTokenizer.h"
#include "../../Core/Core.h"

namespace DE {
	/**
	* CVector2 class
	* Basic math class for vectors of size 2
	*/
	template <class T> class CVector2 {
	public:
		T x;
		T y;

	public:
		CVector2();
		CVector2(const CVector2<T> & vec);
		CVector2(const T x, const T y);
		CVector2(const T * const f);
		const CVector2<T> operator+(const CVector2<T> & vec) const;
		const CVector2<T> operator-(const CVector2<T> & vec) const;
		const CVector2<T> operator*(const CVector2<T> & vec) const;
		const CVector2<T> operator/(const CVector2<T> & vec) const;
		const CVector2<T> operator-() const;
		const CVector2<T> Normalize() const;
		const CVector2<T> & operator=(const CVector2<T> & vec);
		const CVector2<T> & operator+=(const CVector2<T> & vec);
		const CVector2<T> & operator-=(const CVector2<T> & vec);
		const CVector2<T> & operator*=(const CVector2<T> & vec);
		const CVector2<T> & operator*=(const T number);
		const CVector2<T> & operator/=(const CVector2<T> & vec);
		const CVector2<T> & operator/=(const T number);
		const bool operator==(const CVector2<T> & vec) const;
		const bool operator!=(const CVector2<T> & vec) const;
		const T Length() const;
		const T DotProduct(const CVector2<T> & vec) const;
		const T Distance(const CVector2<T> & vec) const;
		operator T * ();
		operator const T * () const;
		static const CVector2<T> GetFrom(const std::string text, const CVector2<T> & defaultValue = CVector2<T>(), const std::string separator = ":");
		static const T Length(const CVector2<T> & vec);
		static const T DotProduct(const CVector2<T> & vec1, const CVector2<T> & vec2);
		static const T Distance(const CVector2<T> & vec1, const CVector2<T> & vec2);
		static void Normalize(CVector2<T> & vec);
	};

	template <class T> const CVector2<T> operator*(const CVector2<T> & vec, const T number);
	template <class T> const CVector2<T> operator*(const T number, const CVector2<T> & vec);
	template <class T> const CVector2<T> operator/(const CVector2<T> & vec, const T number);
	template <class T> const CVector2<T> operator/(const T number, const CVector2<T> & vec);

	/**
	* Constructor
	*/
	template <class T> inline CVector2<T>::CVector2() {
		this->x = 0;
		this->y = 0;
	}

	/**
	* Constructor
	* @param vec A CVector2
	*/
	template <class T> inline CVector2<T>::CVector2(const CVector2<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
	}

	/**
	* Constructor
	* @param x The x coordinate
	* @param y The y coordinate
	*/
	template <class T> inline CVector2<T>::CVector2(const T x, const T y) {
		this->x = x;
		this->y = y;
	}

	/**
	* Constructor
	* @param f The coordinate
	*/
	template <class T> inline CVector2<T>::CVector2(const T * const f) {
		this->x = f[0];
		this->y = f[1];
	}

	/**
	* + operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::operator+(const CVector2<T> & vec) const {
		CVector2<T> tmp(*this);
		tmp.x += vec.x;
		tmp.y += vec.y;
		return tmp;
	}

	/**
	* - operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::operator-(const CVector2<T> & vec) const {
		CVector2<T> tmp(*this);
		tmp.x -= vec.x;
		tmp.y -= vec.y;
		return tmp;
	}

	/**
	* * operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::operator*(const CVector2<T> & vec) const {
		CVector2<T> tmp(*this);
		tmp.x *= vec.x;
		tmp.y *= vec.y;
		return tmp;
	}

	/**
	* / operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::operator/(const CVector2<T> & vec) const {
		CVector2<T> tmp(*this);
		tmp.x /= vec.x;
		tmp.y /= vec.y;
		return tmp;
	}

	/**
	* - operator override
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::operator-() const {
		CVector2<T> tmp(*this);
		tmp.x *= -1;
		tmp.y *= -1;
		return tmp;
	}

	/**
	* Normalize the vector CVector2
	*/
	template <class T> inline const CVector2<T> CVector2<T>::Normalize() const {
		CVector2<T> tmp = *this;
		T length = Length();

		if(length > 0)
			tmp = CVector2(this->x / length, this->y / length);

		return tmp;
	}

	/**
	* = operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator=(const CVector2<T> & vec) {
		this->x = vec.x;
		this->y = vec.y;
		return *this;
	}

	/**
	* += operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator+=(const CVector2<T> & vec) {
		this->x += vec.x;
		this->y += vec.y;
		return *this;
	}

	/**
	* -= operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator-=(const CVector2<T> & vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		return *this;
	}

	/**
	* *= operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator*=(const CVector2<T> & vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		return *this;
	}

	/**
	* *= operator override
	* @param number A number
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator*=(const T number) {
		*this = *this * number;
		return *this;
	}

	/**
	* /= operator override
	* @param vec CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator/=(const CVector2<T> & vec) {
		this->x /= vec.x;
		this->y /= vec.y;
		return *this;
	}

	/**
	* /= operator override
	* @param number A number
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> & CVector2<T>::operator/=(const T number) {
		this->x /= number;
		this->y /= number;
		return *this;
	}

	/**
	* == operator override
	* @param vec CVector2
	* @return The equal result
	*/
	template <class T> inline const bool CVector2<T>::operator==(const CVector2<T> & vec) const {
		return std::abs(this->x - vec.x) < EPSILON && std::abs(this->y - vec.y) < EPSILON;
	}

	/**
	* != operator override
	* @param vec CVector2
	* @return The !equal result
	*/
	template <class T> inline const bool CVector2<T>::operator!=(const CVector2<T> & vec) const {
		return !(*this == vec);
	}

	/**
	* Get the length
	* @return The length
	*/
	template <class T> inline const T CVector2<T>::Length() const {
		return sqrt(this->x * this->x + this->y * this->y);
	}

	/**
	* Perform a dot product
	* @param vec A CVector2
	* @return The dot product
	*/
	template <class T> inline const T CVector2<T>::DotProduct(const CVector2<T> & vec) const {
		return this->x * vec.x + this->y * vec.y;
	}

	/**
	* Perform a distance calculation
	* @param vec A CVector2
	* @return The distance
	*/
	template <class T> inline const T CVector2<T>::Distance(const CVector2<T> & vec) const {
		return CVector2<T>(vec.x - this->x, vec.y - this->y).Length();
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	template <class T> inline CVector2<T>::operator T * () {
		return &this->x;
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	template <class T> inline CVector2<T>::operator const T * () const {
		return &this->x;
	}

	/**
	* Get a CVector2 from a string
	* @param text The text
	* @param defaultValue The default value
	* @param separator The separator
	* @return The vector
	*/
	template <class T> inline const CVector2<T> CVector2<T>::GetFrom(const std::string text, const CVector2<T> & defaultValue, const std::string separator) {
		CVector2<T> result;
		CTokenizer token;

		token.AddLine(text);

		if(text.length() > 0 && token.Tokenize(separator, 2) && token.GetTokens().size() == 2) {
			std::istringstream ss;

			ss.clear();
			ss.str(token.GetTokens().at(0));
			ss >> result.x;

			ss.clear();
			ss.str(token.GetTokens().at(1));
			ss >> result.y;
		} else {
			result = defaultValue;
		}

		return result;
	}

	/**
	* Get the length of a CVector2
	* @param vec A CVector2
	* @return The length
	*/
	template <class T> inline const T CVector2<T>::Length(const CVector2<T> & vec) {
		return vec.Length();
	}

	/**
	* Perform a dot product
	* @param vec1 A CVector2
	* @param vec2 A CVector2
	* @return The dot product
	*/
	template <class T> inline const T CVector2<T>::DotProduct(const CVector2<T> & vec1, const CVector2<T> & vec2) {
		return vec1.DotProduct(vec2);
	}

	/**
	* Perform a distance calculation
	* @param vec1 A CVector2
	* @param vec2 A CVector2
	* @return The distance
	*/
	template <class T> inline const T CVector2<T>::Distance(const CVector2<T> & vec1, const CVector2<T> & vec2) {
		return vec1.Distance(vec2);
	}

	/**
	* Normalize a CVector2
	* @param vec A CVector2
	*/
	template <class T> inline void CVector2<T>::Normalize(CVector2<T> & vec) {
		return vec.Length();
	}

	/**
	* * operator override
	* @param vec A CVector2
	* @param number A number
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> operator*(const CVector2<T> & vec, const T number) {
		CVector2<T> tmp(vec);
		tmp.x *= number;
		tmp.y *= number;
		return tmp;
	}

	/**
	* * operator override
	* @param number A number
	* @param vec A CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> operator*(const T number, const CVector2<T> & vec) {
		return vec * number;
	}

	/**
	* / operator override
	* @param vec A CVector2
	* @param number A number
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> operator/(const CVector2<T> & vec, const T number) {
		CVector2<T> tmp(vec);
		tmp.x /= number;
		tmp.y /= number;
		return tmp;
	}

	/**
	* / operator override
	* @param number A number
	* @param vec A CVector2
	* @return The CVector2
	*/
	template <class T> inline const CVector2<T> operator/(const T number, const CVector2<T> & vec) {
		return CVector2<T>(number / vec.x, number / vec.y);
	}

	typedef CVector2<int> CVector2I;
	typedef CVector2<float> CVector2F;
}

#endif
