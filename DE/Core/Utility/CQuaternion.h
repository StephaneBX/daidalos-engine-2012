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

#ifndef __CQUATERNION_H
#define __CQUATERNION_H

#include <math.h>
#include "../../Core/Core.h"
#include "../../Core/Utility/CVector3.h"

namespace DE {
	/**
	* CQuaternion class
	* Basic math class for quaternions
	*/
	class CQuaternion {
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		CQuaternion();
		CQuaternion(const CQuaternion & quat);
		CQuaternion(const CVector3F & vec, const float angle);
		CQuaternion(const float x, const float y, const float z, const float w);
		CQuaternion(const float * const f);
		const CQuaternion operator+(const CQuaternion & quat) const;
		const CQuaternion operator-(const CQuaternion & quat) const;
		const CQuaternion operator*(const CQuaternion & quat) const;
		const CQuaternion operator/(const CQuaternion & quat) const;
		const CQuaternion operator-() const;
		const CQuaternion Inverse() const;
		const CQuaternion Normalize() const;
		const CQuaternion Log() const;
		const CQuaternion Exp() const;
		const CQuaternion Power(const float f) const;
		const CQuaternion YawPitchRoll(const float angleX, const float angleY, const float angleZ) const;
		const CQuaternion Lerp(const CQuaternion & quat2, const float t) const;
		const CQuaternion Slerp(const CQuaternion & quat2, const float t) const;
		const CQuaternion & operator=(const CQuaternion & quat);
		const CQuaternion & operator+=(const CQuaternion & quat);
		const CQuaternion & operator-=(const CQuaternion & quat);
		const CQuaternion & operator*=(const CQuaternion & quat);
		const CQuaternion & operator*=(const float number);
		const CQuaternion & operator/=(const CQuaternion & quat);
		const CQuaternion & operator/=(const float number);
		const bool operator==(const CQuaternion & quat) const;
		const bool operator!=(const CQuaternion & quat) const;
		const float Length() const;
		const float DotProduct(const CQuaternion & quat) const;
		operator float * ();
		operator const float * () const;
		static const float Length(const CQuaternion & quat);
		static const float DotProduct(const CQuaternion & quat1, const CQuaternion & quat2);
		static void Inverse(CQuaternion & quat);
		static void Normalize(CQuaternion & quat);
		static void Log(CQuaternion & quat);
		static void Exp(CQuaternion & quat);
		static void Power(CQuaternion & quat, const float f);
		static void YawPitchRoll(CQuaternion & out, const float angleX, const float angleY, const float angleZ);
		static void Lerp(CQuaternion & out, const CQuaternion & quat1, const CQuaternion & quat2, const float t);
		static void Slerp(CQuaternion & out, const CQuaternion & quat1, const CQuaternion & quat2, const float t);
	};

	const CQuaternion operator*(const CQuaternion & quat, const float number);
	const CQuaternion operator*(const float number, const CQuaternion & quat);
	const CQuaternion operator/(const CQuaternion & quat, const float number);
	const CQuaternion operator/(const float number, const CQuaternion & quat);

	/**
	* Constructor
	*/
	inline CQuaternion::CQuaternion() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 1;
	}

	/**
	* Constructor
	* @param quat A CQuaternion
	*/
	inline CQuaternion::CQuaternion(const CQuaternion & quat) {
		this->x = quat.x;
		this->y = quat.y;
		this->z = quat.z;
		this->w = quat.w;
	}

	/**
	* Constructor
	* @param vec A CVector3 representing euler equations
	* @param angle The angle
	*/
	inline CQuaternion::CQuaternion(const CVector3F & vec, const float angle) {
		float cos = std::cos(angle / 2);
		float sin = std::sin(angle / 2);

		this->x = vec.x * sin;
		this->y = vec.y * sin;
		this->z = vec.z * sin;
		this->w = cos;

		*this = Normalize();
	}

	/**
	* Constructor
	* @param x The x coordinate
	* @param y The y coordinate
	* @param z The z coordinate
	* @param w The w coordinate
	*/
	inline CQuaternion::CQuaternion(const float x, const float y, const float z, const float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	* Constructor
	* @param f The coordinate
	*/
	inline CQuaternion::CQuaternion(const float * const f) {
		this->x = f[0];
		this->y = f[1];
		this->z = f[2];
		this->w = f[3];
	}

	/**
	* + operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion CQuaternion::operator+(const CQuaternion & quat) const {
		CQuaternion tmp(*this);
		tmp.x += quat.x;
		tmp.y += quat.y;
		tmp.z += quat.z;
		tmp.w += quat.w;
		return tmp;
	}

	/**
	* - operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion CQuaternion::operator-(const CQuaternion & quat) const {
		CQuaternion tmp(*this);
		tmp.x -= quat.x;
		tmp.y -= quat.y;
		tmp.z -= quat.z;
		tmp.w -= quat.w;
		return tmp;
	}

	/**
	* * operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion CQuaternion::operator*(const CQuaternion & quat) const {
		CQuaternion tmp;

		tmp.x = (quat.w * this->x) + (quat.x * this->w) + (quat.y * this->z) - (quat.z * this->y);
		tmp.y = (quat.w * this->y) + (quat.y * this->w) + (quat.z * this->x) - (quat.x * this->z);
		tmp.z = (quat.w * this->z) + (quat.z * this->w) + (quat.x * this->y) - (quat.y * this->x);
		tmp.w = (quat.w * this->w) - (quat.x * this->x) - (quat.y * this->y) - (quat.z * this->z);

		return tmp;
	}

	/**
	* / operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion CQuaternion::operator/(const CQuaternion & quat) const {
		return *this * quat.Inverse();
	}

	/**
	* - operator override
	* @return The CQuaternion
	*/
	inline const CQuaternion CQuaternion::operator-() const {
		CQuaternion tmp(*this);
		tmp.x *= -1;
		tmp.y *= -1;
		tmp.z *= -1;
		tmp.w *= -1;
		return tmp;
	}

	/**
	* Inverse the CQuaternion
	*/
	inline const CQuaternion CQuaternion::Inverse() const {
		CQuaternion tmp(*this);
		tmp = tmp.Normalize();

		return CQuaternion(-tmp.x, -tmp.y, -tmp.z, tmp.w);
	}

	/**
	* Normalize the CQuaternion
	*/
	inline const CQuaternion CQuaternion::Normalize() const {
		const float length = Length();
		CQuaternion tmp;

		if(length > EPSILON)
			tmp = *this / Length();
		else
			tmp = *this;

		return tmp;
	}

	/**
	* Logarithm of a CQuaternion
	*/
	inline const CQuaternion CQuaternion::Log() const {
		CQuaternion tmp;
		const float theta = acos(this->w);
		const float sinTheta = sin(theta);

		if(fabs(sinTheta) > EPSILON) {
			tmp = (*this * theta) / sinTheta;
			tmp.w = 0;
		} else
			tmp = CQuaternion(0, 0, 0, 0);

		return tmp;
	}

	/**
	* Exponential of a CQuaternion
	*/
	inline const CQuaternion CQuaternion::Exp() const {
		CQuaternion tmp;
		const float theta = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		const float sinTheta = sin(theta);
		const float cosTheta = cos(theta);

		tmp.w = cosTheta;
		if(theta > EPSILON) {
			tmp.x = this->x * sinTheta / theta;
			tmp.y = this->y * sinTheta / theta;
			tmp.z = this->z * sinTheta / theta;
		}

		return tmp;
	}

	/**
	* Power of a CQuatenion
	* @param f The power factor
	* @return The quaternion
	*/
	inline const CQuaternion CQuaternion::Power(const float f) const {
		return (Log() * f).Exp();
	}

	/**
	* Rotate the CQuaternion
	* @param angleX The Yaw
	* @param angleY The Pitch
	* @param angleZ The Roll
	* @return The quaternion
	*/
	inline const CQuaternion CQuaternion::YawPitchRoll(const float angleX, const float angleY, const float angleZ) const {
		CQuaternion qx(CVector3F(1, 0, 0), angleX);
		CQuaternion qy(CVector3F(0, 1, 0), angleY);
		CQuaternion qz(CVector3F(0, 0, 1), angleZ);

		return qz * qx * qy;
	}

	/**
	* Linear interpolaton of two CQuaternion
	* @param quat2 The second CQuaternion
	* @param t The linear factor
	* @return The quaternion
	*/
	inline const CQuaternion CQuaternion::Lerp(const CQuaternion & quat2, const float t) const {
		return (*this * (1.0f - t) + quat2 * t).Normalize();
	}

	/**
	* Sphercial linear interpolaton of two CQuaternion
	* @param quat2 The second CQuaternion
	* @param t The linear factor
	* @return The quaternion
	*/
	inline const CQuaternion CQuaternion::Slerp(const CQuaternion & quat2, const float t) const {
		CQuaternion tmp;
		if(DotProduct(quat2) >= 0)
			tmp = *this * (Inverse() * quat2).Power(t);
		else
			tmp = *this * (Inverse() * -1 * quat2).Power(t);

		return tmp;
	}

	/**
	* = operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator=(const CQuaternion & quat) {
		this->x = quat.x;
		this->y = quat.y;
		this->z = quat.z;
		this->w = quat.w;
		return *this;
	}

	/**
	* += operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator+=(const CQuaternion & quat) {
		this->x += quat.x;
		this->y += quat.y;
		this->z += quat.z;
		this->w += quat.w;
		return *this;
	}

	/**
	* -= operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator-=(const CQuaternion & quat) {
		this->x -= quat.x;
		this->y -= quat.y;
		this->z -= quat.z;
		this->w -= quat.w;
		return *this;
	}

	/**
	* *= operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator*=(const CQuaternion & quat) {
		*this = *this * quat;
		return *this;
	}

	/**
	* *= operator override
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator*=(const float number) {
		*this = *this * number;
		return *this;
	}

	/**
	* /= operator override
	* @param quat CQuaternion
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator/=(const CQuaternion & quat) {
		*this = *this / quat;
		return *this;
	}

	/**
	* /= operator override
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion & CQuaternion::operator/=(const float number) {
		*this = *this / number;
		return *this;
	}

	/**
	* == operator override
	* @param quat CQuaternion
	* @return The equal result
	*/
	inline const bool CQuaternion::operator==(const CQuaternion & quat) const {
		return std::abs(this->x - quat.x) < EPSILON && std::abs(this->y - quat.y) < EPSILON && std::abs(this->z - quat.z) < EPSILON && std::abs(this->w - quat.w) < EPSILON;
	}

	/**
	* != operator override
	* @param quat CQuaternion
	* @return The !equal result
	*/
	inline const bool CQuaternion::operator!=(const CQuaternion & quat) const {
		return !(*this == quat);
	}

	/**
	* Get the length
	* @return The length
	*/
	inline const float CQuaternion::Length() const {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
	}

	/**
	* Dot product between two CQuaternion
	* @param quat The second CQuaternion
	* @return The dot product
	*/
	inline const float CQuaternion::DotProduct(const CQuaternion & quat) const {
		return (this->x * quat.x) + (this->y * quat.y) + (this->z * quat.z) + (this->w * quat.w);
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	inline CQuaternion::operator float * () {
		return &this->x;
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	inline CQuaternion::operator const float * () const {
		return &this->x;
	}

	/**
	* Get the length of a CQuaternion
	* @param quat A CQuaternion
	* @return The length
	*/
	inline const float CQuaternion::Length(const CQuaternion & quat) {
		return quat.Length();
	}

	/**
	* Dot product between two CQuaternion
	* @param quat1 The first CQuaternion
	* @param quat2 The second CQuaternion
	* @return The dot product
	*/
	inline const float CQuaternion::DotProduct(const CQuaternion & quat1, const CQuaternion & quat2) {
		return quat1.DotProduct(quat2);
	}

	/**
	* Inverse a CQuaternion
	* @param quat A CQuaternion
	*/
	inline void CQuaternion::Inverse(CQuaternion & quat) {
		quat = quat.Inverse();
	}

	/**
	* Normalize a CQuaternion
	* @param quat A CQuaternion
	*/
	inline void CQuaternion::Normalize(CQuaternion & quat) {
		quat = quat.Normalize();
	}

	/**
	* Logarithm of a CQuatenion
	* @param quat The CQuaternion
	*/
	inline void CQuaternion::Log(CQuaternion & quat) {
		quat = quat.Log();
	}

	/**
	* Exponential of a CQuatenion
	* @param quat The CQuaternion
	*/
	inline void CQuaternion::Exp(CQuaternion & quat) {
		quat = quat.Exp();
	}

	/**
	* Power of a CQuatenion
	* @param quat The CQuaternion
	* @param f The power factor
	*/
	inline void CQuaternion::Power(CQuaternion & quat, const float f) {
		quat = quat.Power(f);
	}

	/**
	* Rotate a CQuaternion
	* @param out A CQuaternion
	* @param angleX The Yaw
	* @param angleY The Pitch
	* @param angleZ The Roll
	*/
	inline void CQuaternion::YawPitchRoll(CQuaternion & out, const float angleX, const float angleY, const float angleZ) {
		out = out.YawPitchRoll(angleX, angleY, angleZ).Normalize();
	}

	/**
	* Linear interpolaton of two CQuaternion
	* @param out The Result CQuaternion
	* @param quat1 The first CQuaternion
	* @param quat2 The second CQuaternion
	* @param t The linear factor
	*/
	inline void CQuaternion::Lerp(CQuaternion & out, const CQuaternion & quat1, const CQuaternion & quat2, const float t) {
		out = quat1.Lerp(quat2, t);
	}

	/**
	* Sphercial linear interpolaton of two CQuaternion
	* @param out The Result CQuaternion
	* @param quat1 The first CQuaternion
	* @param quat2 The second CQuaternion
	* @param t The linear factor
	*/
	inline void CQuaternion::Slerp(CQuaternion & out, const CQuaternion & quat1, const CQuaternion & quat2, const float t) {
		out = quat1.Slerp(quat2, t);
	}

	/**
	* * operator override
	* @param quat A CQuaternion
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion operator*(const CQuaternion & quat, const float number) {
		CQuaternion tmp(quat);
		tmp.x *= number;
		tmp.y *= number;
		tmp.z *= number;
		tmp.w *= number;
		return tmp;
	}

	/**
	* * operator override
	* @param quat A CQuaternion
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion operator*(const float number, const CQuaternion & quat) {
		return quat * number;
	}

	/**
	* / operator override
	* @param quat A CQuaternion
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion operator/(const CQuaternion & quat, const float number) {
		CQuaternion tmp(quat);
		tmp.x /= number;
		tmp.y /= number;
		tmp.z /= number;
		tmp.w /= number;
		return tmp;
	}

	/**
	* / operator override
	* @param quat A CQuaternion
	* @param number A number
	* @return The CQuaternion
	*/
	inline const CQuaternion operator/(const float number, const CQuaternion & quat) {
		return number / quat;
	}
}

#endif
