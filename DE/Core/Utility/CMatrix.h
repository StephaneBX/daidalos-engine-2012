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

#ifndef __CMATRIX_H
#define __CMATRIX_H

#include <cmath>
#include "../../Core/Utility/CVector3.h"
#include "../../Core/Utility/CVector4.h"
#include "../../Core/Utility/CQuaternion.h"

namespace DE {
	/**
	* CMatrix class
	* Basic math class for matrixes
	*/
	class CMatrix {
	public:
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

	public:
		CMatrix();
		CMatrix(const CMatrix & matrix);
		CMatrix(const CVector3F & scale, const CVector3F & rotation, const CVector3F & translation);
		CMatrix(const float m11, const float m12, const float m13, const float m14,
			const float m21, const float m22, const float m23, const float m24,
			const float m31, const float m32, const float m33, const float m34,
			const float m41, const float m42, const float m43, const float m44);
		const CMatrix operator+(const CMatrix & matrix) const;
		const CMatrix operator-(const CMatrix & matrix) const;
		const CMatrix operator*(const CMatrix & matrix) const;
		const CMatrix operator-() const;
		const CMatrix Inverse() const;
		const CMatrix Transpose() const;
		const CMatrix Identity() const;
		const CMatrix SetTranslation(const float x, const float y, const float z) const;
		const CMatrix SetScale(const float x, const float y, const float z) const;
		const CMatrix SetRotationX(const float radian) const;
		const CMatrix SetRotationY(const float radian) const;
		const CMatrix SetRotationZ(const float radian) const;
		const CMatrix RotationQuaternion(const CQuaternion & quat) const;
		const CMatrix GetScale() const;
		const CMatrix GetRotation() const;
		const CMatrix GetBillboard() const;
		const CVector4F Transform(const CVector4F & vec, const bool normalize = false) const;
		const CVector3F Transform(const CVector3F & vec, const bool normalize = false) const;
		const CVector3F GetTranslation() const;
		const CQuaternion TransformRotation() const;
		const CMatrix & operator=(const CMatrix & matrix);
		const CMatrix & operator+=(const CMatrix & matrix);
		const CMatrix & operator-=(const CMatrix & matrix);
		const CMatrix & operator*=(const CMatrix & matrix);
		const CMatrix & operator*=(const float number);
		const bool operator==(const CMatrix & matrix) const;
		const bool operator!=(const CMatrix & matrix) const;
		const float Determinant() const;
		const float & operator() (const unsigned int row, const unsigned int col) const;
		float & operator() (const unsigned int row, const unsigned int col);
		operator float * ();
		operator const float * () const;
		void LookAt(const CVector3F & eye, const CVector3F & at, const CVector3F & up = CVector3F(0, 1, 0));
		void PerspectiveFOV(const float fovy, const float ratio, const float znear, const float zfar);
		void OrthographicView(const float width, const float height, const float znear, const float zfar);
		void OrthographicView(const float left, const float right, const float bottom, const float top, const float znear, const float zfar);
		static void TransformArray(CVector3F * out, CVector3F * input, const CMatrix & matrix, const int n, const bool normalize);
		static void RotationQuaternion(CMatrix & out, const CQuaternion & quat);
		static void Decompose(CVector3F & outScale, CQuaternion & outRotation, CVector3F & outTranslation, const CMatrix & matrix);
		static const CMatrix TranslationMatrix(const CVector3F & translation);
		static const CMatrix RotationMatrix(const CVector3F & rotation);
		static const CMatrix ScaleMatrix(const CVector3F & scale);
	};

	const CMatrix operator*(const CMatrix & matrix, const float number);
	const CMatrix operator*(const float number, const CMatrix & matrix);
	const CMatrix operator/(const CMatrix & matrix, const float number);
	const CMatrix operator/(const float number, const CMatrix & matrix);

	/**
	* Constructor
	*/
	inline CMatrix::CMatrix() {
		*this = Identity();
	}

	/**
	* Constructor
	* @param matrix A CMatrix
	*/
	inline CMatrix::CMatrix(const CMatrix & matrix) {
		*this = matrix;
	}

	/**
	* Constructor
	* @param scale The scale vector
	* @param rotation The rotation vector in radian
	* @param translation The translation vector
	*/
	inline CMatrix::CMatrix(const CVector3F & scale, const CVector3F & rotation, const CVector3F & translation) {
		CMatrix scaleMatrix;
		CMatrix rotationMatrix;
		CMatrix translationMatrix;
		CQuaternion rot;

		scaleMatrix = scaleMatrix.SetScale(scale.x, scale.y, scale.z);
		CQuaternion::YawPitchRoll(rot, rotation.x, rotation.y, rotation.z);
		rotationMatrix = RotationQuaternion(rot);
		translationMatrix = translationMatrix.SetTranslation(translation.x, translation.y, translation.z);

		*this = scaleMatrix * rotationMatrix * translationMatrix;
	}

	/**
	* Constructor
	* @param m11 Matrix element
	* @param m12 Matrix element
	* @param m13 Matrix element
	* @param m14 Matrix element
	* @param m21 Matrix element
	* @param m22 Matrix element
	* @param m23 Matrix element
	* @param m24 Matrix element
	* @param m31 Matrix element
	* @param m32 Matrix element
	* @param m33 Matrix element
	* @param m34 Matrix element
	* @param m41 Matrix element
	* @param m42 Matrix element
	* @param m43 Matrix element
	* @param m44 Matrix element
	*/
	inline CMatrix::CMatrix(const float m11, const float m12, const float m13, const float m14,
		const float m21, const float m22, const float m23, const float m24,
		const float m31, const float m32, const float m33, const float m34,
		const float m41, const float m42, const float m43, const float m44) {
		this->_11 = m11; this->_12 = m12; this->_13 = m13; this->_14 = m14;
		this->_21 = m21; this->_22 = m22; this->_23 = m23; this->_24 = m24;
		this->_31 = m31; this->_32 = m32; this->_33 = m33; this->_34 = m34;
		this->_41 = m41; this->_42 = m42; this->_43 = m43; this->_44 = m44;
	}

	/**
	* + operator override
	* @param matrix A CMatrix
	* @return A CMatrix
	*/
	inline const CMatrix CMatrix::operator+(const CMatrix & matrix) const {
		return CMatrix(this->_11 + matrix._11, this->_12 + matrix._12, this->_13 + matrix._13, this->_14 + matrix._14,
			this->_21 + matrix._21, this->_22 + matrix._22, this->_23 + matrix._23, this->_24 + matrix._24,
			this->_31 + matrix._31, this->_32 + matrix._32, this->_33 + matrix._33, this->_34 + matrix._34,
			this->_41 + matrix._41, this->_42 + matrix._42, this->_43 + matrix._43, this->_44 + matrix._44);
	}

	/**
	* - operator override
	* @param matrix A CMatrix
	* @return A CMatrix
	*/
	inline const CMatrix CMatrix::operator-(const CMatrix & matrix) const {
		return CMatrix(this->_11 - matrix._11, this->_12 - matrix._12, this->_13 - matrix._13, this->_14 - matrix._14,
			this->_21 - matrix._21, this->_22 - matrix._22, this->_23 - matrix._23, this->_24 - matrix._24,
			this->_31 - matrix._31, this->_32 - matrix._32, this->_33 - matrix._33, this->_34 - matrix._34,
			this->_41 - matrix._41, this->_42 - matrix._42, this->_43 - matrix._43, this->_44 - matrix._44);
	}

	/**
	* * operator override
	* @param matrix A CMatrix
	* @return A CMatrix
	*/
	inline const CMatrix CMatrix::operator*(const CMatrix & matrix) const {
		return CMatrix(this->_11 * matrix._11 + this->_12 * matrix._21 + this->_13 * matrix._31 + this->_14 * matrix._41,
			this->_11 * matrix._12 + this->_12 * matrix._22 + this->_13 * matrix._32 + this->_14 * matrix._42,
			this->_11 * matrix._13 + this->_12 * matrix._23 + this->_13 * matrix._33 + this->_14 * matrix._43,
			this->_11 * matrix._14 + this->_12 * matrix._24 + this->_13 * matrix._34 + this->_14 * matrix._44,

			this->_21 * matrix._11 + this->_22 * matrix._21 + this->_23 * matrix._31 + this->_24 * matrix._41,
			this->_21 * matrix._12 + this->_22 * matrix._22 + this->_23 * matrix._32 + this->_24 * matrix._42,
			this->_21 * matrix._13 + this->_22 * matrix._23 + this->_23 * matrix._33 + this->_24 * matrix._43,
			this->_21 * matrix._14 + this->_22 * matrix._24 + this->_23 * matrix._34 + this->_24 * matrix._44,

			this->_31 * matrix._11 + this->_32 * matrix._21 + this->_33 * matrix._31 + this->_34 * matrix._41,
			this->_31 * matrix._12 + this->_32 * matrix._22 + this->_33 * matrix._32 + this->_34 * matrix._42,
			this->_31 * matrix._13 + this->_32 * matrix._23 + this->_33 * matrix._33 + this->_34 * matrix._43,
			this->_31 * matrix._14 + this->_32 * matrix._24 + this->_33 * matrix._34 + this->_34 * matrix._44,

			this->_41 * matrix._11 + this->_42 * matrix._21 + this->_43 * matrix._31 + this->_44 * matrix._41,
			this->_41 * matrix._12 + this->_42 * matrix._22 + this->_43 * matrix._32 + this->_44 * matrix._42,
			this->_41 * matrix._13 + this->_42 * matrix._23 + this->_43 * matrix._33 + this->_44 * matrix._43,
			this->_41 * matrix._14 + this->_42 * matrix._24 + this->_43 * matrix._34 + this->_44 * matrix._44);

	}

	/**
	* - operator override
	* @return A CMatrix
	*/
	inline const CMatrix CMatrix::operator-() const {
		return CMatrix(-this->_11, -this->_12, -this->_13, -this->_14,
			-this->_21, -this->_22, -this->_23, -this->_24,
			-this->_31, -this->_32, -this->_33, -this->_34,
			-this->_41, -this->_42, -this->_43, -this->_44);
	}

	/**
	* Inverse a matrix
	* @return The inversed matrix
	*/
	inline const CMatrix CMatrix::Inverse() const {
		CMatrix ret;

		float a0 = this->_11 * this->_22 - this->_12 * this->_21;
		float a1 = this->_11 * this->_23 - this->_13 * this->_21;
		float a2 = this->_11 * this->_24 - this->_14 * this->_21;
		float a3 = this->_12 * this->_23 - this->_13 * this->_22;
		float a4 = this->_12 * this->_24 - this->_14 * this->_22;
		float a5 = this->_13 * this->_24 - this->_14 * this->_23;
		float b0 = this->_31 * this->_42 - this->_32 * this->_41;
		float b1 = this->_31 * this->_43 - this->_33 * this->_41;
		float b2 = this->_31 * this->_44 - this->_34 * this->_41;
		float b3 = this->_32 * this->_43 - this->_33 * this->_42;
		float b4 = this->_32 * this->_44 - this->_34 * this->_42;
		float b5 = this->_33 * this->_44 - this->_34 * this->_43;

		float determinant = a0  *b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

		if(std::fabs(determinant) > EPSILON) {
			ret._11 = this->_22 * b5 - this->_23 * b4 + this->_24 * b3;
			ret._21 = -this->_21 * b5 + this->_23 * b2 - this->_24 * b1;
			ret._31 = this->_21 * b4 - this->_22 * b2 + this->_11 * b0;
			ret._41 = -this->_21 * b3 + this->_22 * b1 - this->_23 * b0;
			ret._12 = -this->_12 * b5 + this->_13 * b4 - this->_14 * b3;
			ret._22 = this->_11 * b5 - this->_13 * b2 + this->_14 * b1;
			ret._32 = -this->_11 * b4 + this->_12 * b2 - this->_14 * b0;
			ret._42 = this->_11 * b3 - this->_12 * b1 + this->_13 * b0;
			ret._13 = this->_42 * a5 - this->_43 * a4 + this->_44 * a3;
			ret._23 = -this->_41 * a5 + this->_43 * a2 - this->_44 * a1;
			ret._33 = this->_41 * a4 - this->_42 * a2 + this->_44 * a0;
			ret._43 = -this->_41 * a3 + this->_42 * a1 - this->_43 * a0;
			ret._14 = -this->_32 * a5 + this->_33 * a4 - this->_34 * a3;
			ret._24 = this->_31 * a5 - this->_33 * a2 + this->_34 * a1;
			ret._34 = -this->_31 * a4 + this->_32 * a2 - this->_34 * a0;
			ret._44 = this->_31 * a3 - this->_32 * a1 + this->_33 * a0;

			float invDet = 1 / determinant;
			ret *= invDet;
		}

		return ret;
	}

	/**
	* Transpose a matrix
	* @return The transposed matrix
	*/
	inline const CMatrix CMatrix::Transpose() const {
		return CMatrix(this->_11, this->_21, this->_31, this->_41,
			this->_12, this->_22, this->_32, this->_42,
			this->_13, this->_23, this->_33, this->_43,
			this->_14, this->_24, this->_34, this->_44);
	}

	/**
	* Transform the matrix into an identity matrix
	*/
	inline const CMatrix CMatrix::Identity() const {
		return CMatrix(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	/**
	* Transform the matrix into a translation matrix
	*/
	inline const CMatrix CMatrix::SetTranslation(const float x, const float y, const float z) const {
		return CMatrix(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1);
	}

	/**
	* Transform the matrix into a scale matrix
	*/
	inline const CMatrix CMatrix::SetScale(const float x, const float y, const float z) const {
		return CMatrix(x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	/**
	* Transform the matrix into a x rotation matrix
	*/
	inline const CMatrix CMatrix::SetRotationX(const float radian) const {
		float cos = std::cos(radian);
		float sin = std::sin(radian);

		return CMatrix(1, 0, 0, 0,
			0, cos, sin, 0,
			0, -sin, cos, 0,
			0, 0, 0, 1);
	}

	/**
	* Transform the matrix into a y rotation matrix
	*/
	inline const CMatrix CMatrix::SetRotationY(const float radian) const {
		float cos = std::cos(radian);
		float sin = std::sin(radian);

		return CMatrix(cos, 0, -sin, 0,
			0, 1, 0, 0,
			sin, 0, cos, 0,
			0, 0, 0, 1);
	}

	/**
	* Transform the matrix into a y rotation matrix
	*/
	inline const CMatrix CMatrix::SetRotationZ(const float radian) const {
		float cos = std::cos(radian);
		float sin = std::sin(radian);

		return CMatrix(cos, sin, 0, 0,
			-sin, cos, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1);
	}

	/**
	* Create a rotation matrix with a quaternion
	* @param quat The CQuaternion
	* @return A rotation matrix
	*/
	inline const CMatrix CMatrix::RotationQuaternion(const CQuaternion & quat) const {
		return CMatrix(1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z),
			2.0f * (quat.x * quat.y + quat.z * quat.w),
			2.0f * (quat.x * quat.z - quat.y * quat.w),
			0,

			2.0f * (quat.x * quat.y - quat.z * quat.w),
			1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z),
			2.0f * (quat.y * quat.z + quat.x * quat.w),
			0,

			2.0f * (quat.x * quat.z + quat.y * quat.w),
			2.0f * (quat.y * quat.z - quat.x * quat.w),
			1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y),
			0,

			0,
			0,
			0,
			1);
	}

	/**
	* Get a scale matrix
	* @return A scale matrix
	*/
	inline const CMatrix CMatrix::GetScale() const {
		return CMatrix(this->_11, 0, 0, 0,
			0, this->_22, 0, 0,
			0, 0, this->_33, 0,
			0, 0, 0, 1);
	}

	/**
	* Get a rotation matrix
	* @return A rotation matrix
	*/
	inline const CMatrix CMatrix::GetRotation() const {
		CMatrix tmp = *this;
		tmp._41 = 0;
		tmp._42 = 0;
		tmp._43 = 0;

		return tmp;
	}

	inline const CMatrix CMatrix::GetBillboard() const {
		CMatrix billboard = Identity();
		CMatrix transpose = Transpose();

		billboard._11 = transpose._11;
		billboard._12 = transpose._12;
		billboard._13 = transpose._13;
		billboard._21 = transpose._21;
		billboard._22 = transpose._22;
		billboard._23 = transpose._23;
		billboard._31 = transpose._31;
		billboard._32 = transpose._32;
		billboard._33 = transpose._33;

		return billboard;
	}

	/**
	* Tranform a vector with the matrix
	* @param vec The vector to transform
	* @param normalize Normalize the result
	* @return The transformed vector
	*/
	inline const CVector4F CMatrix::Transform(const CVector4F & vec, const bool normalize) const {
		CVector4F result = CVector4F(vec.x * this->_11 + vec.y * this->_21 + vec.z * this->_31 + vec.w * this->_41,
			vec.x * this->_12 + vec.y * this->_22 + vec.z * this->_32 + vec.w * this->_42,
			vec.x * this->_13 + vec.y * this->_23 + vec.z * this->_33 + vec.w * this->_43,
			vec.x * this->_14 + vec.y * this->_24 + vec.z * this->_34 + vec.w * this->_44);
		if(normalize)
			result = result.Normalize();

		return result;
	}

	/**
	* Tranform a vector with the matrix
	* @param vec The vector to transform
	* @param normalize Normalize the result
	* @return The transformed vector
	*/
	inline const CVector3F CMatrix::Transform(const CVector3F & vec, const bool normalize) const {
		CVector3F result = CVector3F(vec.x * this->_11 + vec.y * this->_21 + vec.z * this->_31 + this->_41,
			vec.x * this->_12 + vec.y * this->_22 + vec.z * this->_32 + this->_42,
			vec.x * this->_13 + vec.y * this->_23 + vec.z * this->_33 + this->_43);

		if(normalize)
			result = result.Normalize();

		return result;
	}

	/**
	* Get the translation
	* @return The translation vector
	*/
	inline const CVector3F CMatrix::GetTranslation() const {
		return CVector3F(this->_41, this->_42, this->_43);
	}

	/**
	* Transform the matrix into a rotation quaternion
	* @return A rotation quaternion
	*/
	inline const CQuaternion CMatrix::TransformRotation() const {
		CQuaternion tmp;
		const float trace = this->_11 + this->_22 + this->_33;

		if(trace > EPSILON) {
			float scale = 0.5f / sqrtf(trace + 1.0f);
			tmp.w = 0.25f / scale;
			tmp.x = (this->_23 - this->_32) * scale;
			tmp.y = (this->_31 - this->_13) * scale;
			tmp.z = (this->_12 - this->_21) * scale;
		} else {
			if(this->_11 > this->_22 && this->_11 > this->_33) {
				const float scale = 2.0f * sqrtf(1.0f + this->_11 - this->_22 - this->_33);
				tmp.w = (this->_23 - this->_32) / scale;
				tmp.x = 0.25f * scale;
				tmp.y = (this->_21 + this->_12) / scale;
				tmp.z = (this->_31 + this->_13) / scale;
			} else {
				if(this->_22 > this->_33) {
					const float scale = 2.0f * sqrtf(1.0f + this->_22 - this->_11 - this->_33);
					tmp.w = (this->_31 - this->_13) / scale;
					tmp.x = (this->_21 + this->_12) / scale;
					tmp.y = 0.25f * scale;
					tmp.z = (this->_32 + this->_23) / scale;
				} else {
					const float scale = 2.0f * sqrtf(1.0f + this->_33 - this->_11 - this->_22);
					tmp.w = (this->_12 - this->_21) / scale;
					tmp.x = (this->_31 + this->_13) / scale;
					tmp.y = (this->_32 + this->_23) / scale;
					tmp.z = 0.25f * scale;
				}
			}
		}

		return tmp.Normalize();
	}

	/**
	* = operator override
	* @param matrix A matrix
	* @return The matrix
	*/
	inline const CMatrix & CMatrix::operator=(const CMatrix & matrix) {
		this->_11 = matrix._11; this->_12 = matrix._12; this->_13 = matrix._13; this->_14 = matrix._14;
		this->_21 = matrix._21; this->_22 = matrix._22; this->_23 = matrix._23; this->_24 = matrix._24;
		this->_31 = matrix._31; this->_32 = matrix._32; this->_33 = matrix._33; this->_34 = matrix._34;
		this->_41 = matrix._41; this->_42 = matrix._42; this->_43 = matrix._43; this->_44 = matrix._44;
		return *this;
	}

	/**
	* += operator override
	* @param matrix A matrix
	* @return The matrix
	*/
	inline const CMatrix & CMatrix::operator+=(const CMatrix & matrix) {
		this->_11 += matrix._11; this->_12 += matrix._12; this->_13 += matrix._13; this->_14 += matrix._14;
		this->_21 += matrix._21; this->_22 += matrix._22; this->_23 += matrix._23; this->_24 += matrix._24;
		this->_31 += matrix._31; this->_32 += matrix._32; this->_33 += matrix._33; this->_34 += matrix._34;
		this->_41 += matrix._41; this->_42 += matrix._42; this->_43 += matrix._43; this->_44 += matrix._44;
		return *this;
	}

	/**
	* -= operator override
	* @param matrix A matrix
	* @return The matrix
	*/
	inline const CMatrix & CMatrix::operator-=(const CMatrix & matrix) {
		this->_11 -= matrix._11; this->_12 -= matrix._12; this->_13 -= matrix._13; this->_14 -= matrix._14;
		this->_21 -= matrix._21; this->_22 -= matrix._22; this->_23 -= matrix._23; this->_24 -= matrix._24;
		this->_31 -= matrix._31; this->_32 -= matrix._32; this->_33 -= matrix._33; this->_34 -= matrix._34;
		this->_41 -= matrix._41; this->_42 -= matrix._42; this->_43 -= matrix._43; this->_44 -= matrix._44;
		return *this;
	}

	/**
	* *= operator override
	* @param matrix A matrix
	* @return The matrix
	*/
	inline const CMatrix & CMatrix::operator*=(const CMatrix & matrix) {
		*this = *this * matrix;
		return *this;
	}

	/**
	* *= operator override
	* @param number A float
	* @return The matrix
	*/
	inline const CMatrix & CMatrix::operator*=(const float number) {
		*this = *this * number;
		return *this;
	}

	/**
	* == operator override
	* @param matrix A matrix
	* @return The equal operation result
	*/
	inline const bool CMatrix::operator==(const CMatrix & matrix) const {
		return (std::abs(this->_11 - matrix._11) < EPSILON && std::abs(this->_12 - matrix._12) < EPSILON && std::abs(this->_13 - matrix._13) < EPSILON && std::abs(this->_14 - matrix._14) < EPSILON &&
			std::abs(this->_21 - matrix._21) < EPSILON && std::abs(this->_22 - matrix._22) < EPSILON && std::abs(this->_23 - matrix._23) < EPSILON && std::abs(this->_24 - matrix._24) < EPSILON &&
			std::abs(this->_31 - matrix._31) < EPSILON && std::abs(this->_32 - matrix._32) < EPSILON && std::abs(this->_33 - matrix._33) < EPSILON && std::abs(this->_34 - matrix._34) < EPSILON &&
			std::abs(this->_41 - matrix._41) < EPSILON && std::abs(this->_42 - matrix._42) < EPSILON && std::abs(this->_43 - matrix._43) < EPSILON && std::abs(this->_44 - matrix._44) < EPSILON);
	}

	/**
	* != operator override
	* @param matrix A matrix
	* @return The equal operation result
	*/
	inline const bool CMatrix::operator!=(const CMatrix & matrix) const {
		return !(*this == matrix);
	}

	/**
	* Get the determinant
	* @return The determinant
	*/
	inline const float CMatrix::Determinant() const {
		float a0 = this->_11 * this->_22 - this->_12 * this->_21;
		float a1 = this->_11 * this->_23 - this->_13 * this->_21;
		float a2 = this->_11 * this->_24 - this->_14 * this->_21;
		float a3 = this->_12 * this->_23 - this->_13 * this->_22;
		float a4 = this->_12 * this->_24 - this->_14 * this->_22;
		float a5 = this->_13 * this->_24 - this->_14 * this->_23;
		float b0 = this->_31 * this->_42 - this->_32 * this->_41;
		float b1 = this->_31 * this->_43 - this->_33 * this->_41;
		float b2 = this->_31 * this->_44 - this->_34 * this->_41;
		float b3 = this->_32 * this->_43 - this->_33 * this->_42;
		float b4 = this->_32 * this->_44 - this->_34 * this->_42;
		float b5 = this->_33 * this->_44 - this->_34 * this->_43;

		return a0  *b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	}

	/**
	* () operator override
	* @param row The row
	* @param col The column
	* @return The value
	*/
	inline const float & CMatrix::operator() (const unsigned int row, const unsigned int col) const {
		return operator()(row, col);
	}

	/**
	* () operator override
	* @param row The row
	* @param col The column
	* @return The value
	*/
	inline float & CMatrix::operator() (const unsigned int row, const unsigned int col) {
		return operator float*()[row, col];
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	inline CMatrix::operator float * () {
		return &this->_11;
	}

	/**
	* float * cast operator override
	* @return A float pointer
	*/
	inline CMatrix::operator const float * () const {
		return &this->_11;
	}

	/**
	* Create a view matrix
	* @param eye The eye coordinates
	* @param at The looking coordinates
	* @param up The up vector
	*/
	inline void CMatrix::LookAt(const CVector3F & eye, const CVector3F & at, const CVector3F & up) {
		CVector3F zAxis = (at - eye).Normalize();
		CVector3F xAxis = up.CrossProduct(zAxis).Normalize();
		CVector3F yAxis = zAxis.CrossProduct(xAxis).Normalize();

		this->_11 = xAxis.x;								this->_12 = yAxis.x;								this->_13 = zAxis.x;								this->_14 = 0;
		this->_21 = xAxis.y;								this->_22 = yAxis.y;								this->_23 = zAxis.y;								this->_24 = 0;
		this->_31 = xAxis.z;								this->_32 = yAxis.z;								this->_33 = zAxis.z;								this->_34 = 0;
		this->_41 = -xAxis.DotProduct(eye);					this->_42 = -yAxis.DotProduct(eye);					this->_43 = -zAxis.DotProduct(eye);					this->_44 = 1.0f;
	}

	/**
	* Create a perspective matrix
	* @param fovy
	* @param ratio
	* @param znear
	* @param zfar
	*/
	inline void CMatrix::PerspectiveFOV(const float fovy, const float ratio, const float znear, const float zfar) {
		float yScale = std::cosf(fovy / 2.0f) / std::sinf(fovy / 2.0f);
		float xScale = yScale / ratio;
		float coeff = zfar / (zfar - znear);

		this->_11 = xScale; this->_12 = 0.0f;   this->_13 = 0.0f;			this->_14 = 0.0f;
		this->_21 = 0.0f;   this->_22 = yScale; this->_23 = 0.0f;			this->_24 = 0.0f;
		this->_31 = 0.0f;   this->_32 = 0.0f;   this->_33 = coeff;			this->_34 = 1.0f;
		this->_41 = 0.0f;   this->_42 = 0.0f;   this->_43 = znear * -coeff; this->_44 = 0.0f;
	}

	/**
	* Create an orthographic view
	* @param width The camera space width
	* @param height The camera space height
	* @param znear The near plane
	* @param zfar The far plane
	*/
	inline void CMatrix::OrthographicView(const float width, const float height, const float znear, const float zfar) {
		this->_11 = 2 / width;	this->_12 = 0.0f;		this->_13 = 0.0f;						this->_14 = 0.0f;
		this->_21 = 0.0f;		this->_22 = 2 / height;	this->_23 = 0.0f;						this->_24 = 0.0f;
		this->_31 = 0.0f;		this->_32 = 0.0f;		this->_33 = 1 / (zfar - znear);			this->_34 = 0.0f;
		this->_41 = 0.0f;		this->_42 = 0.0f;		this->_43 = znear / (znear - zfar);	this->_44 = 1.0f;
	}

	/**
	* Create an orthographic view
	* @param left The left corner
	* @param right The right corner
	* @param bottom The bottom value
	* @param top The top value
	* @param znear The near plane
	* @param zfar The far plane
	*/
	inline void CMatrix::OrthographicView(const float left, const float right, const float bottom, const float top, const float znear, const float zfar) {
		this->_11 = 2 / (right - left);					this->_12 = 0.0f;								this->_13 = 0.0f;								this->_14 = 0.0f;
		this->_21 = 0.0f;								this->_22 = 2 / (top - bottom);					this->_23 = 0.0f;								this->_24 = 0.0f;
		this->_31 = 0.0f;								this->_32 = 0.0f;								this->_33 = 1 / (zfar - znear);					this->_34 = 0.0f;
		this->_41 = (left + right) / (left - right);	this->_42 = (top + bottom) / (bottom - top);	this->_43 = znear / (znear - zfar);				this->_44 = 1.0f;
	}

	/**
	* Tranform a vector array with the matrix
	* @param out The output vector array
	* @param input The intput vector array
	* @param matrix The matrix
	* @param n The array size
	* @param normalize If the data needs to be normalized
	*/
	inline void CMatrix::TransformArray(CVector3F * out, CVector3F * input, const CMatrix & matrix, const int n, const bool normalize) {
		for(int i = 0; i < n; ++i) {
			if(normalize)
				out[i] = matrix.Transform(input[i]).Normalize();
			else
				out[i] = matrix.Transform(input[i]);
		}
	}

	/**
	* Create a rotation matrix with a quaternion
	* @param out The CMatrix
	* @param quat The CQuaternion
	*/
	inline void CMatrix::RotationQuaternion(CMatrix & out, const CQuaternion & quat) {
		out = out.RotationQuaternion(quat);
	}

	/**
	* Decompose a matrix intro a scaling vector, a translation vector and a rotation quaternion
	* @param outScale The scaling vector
	* @param outRotation The rotation quaternion
	* @param outTranslation The translation vector
	* @param matrix The matrix
	*/
	inline void CMatrix::Decompose(CVector3F & outScale, CQuaternion & outRotation, CVector3F & outTranslation, const CMatrix & matrix) {
		// Scale
		outScale.x = sqrtf(matrix._11 * matrix._11 + matrix._21 * matrix._21 + matrix._31 * matrix._31);
		outScale.y = sqrtf(matrix._12 * matrix._12 + matrix._22 * matrix._22 + matrix._32 * matrix._32);
		outScale.z = sqrtf(matrix._13 * matrix._13 + matrix._23 * matrix._23 + matrix._33 * matrix._33);

		// Translation
		outTranslation.x = matrix._41;
		outTranslation.y = matrix._42;
		outTranslation.z = matrix._43;

		// Rotation
		outRotation = matrix.TransformRotation();
	}

	/**
	* Create a translation matrix
	* @param translation A translation vector
	* @return A translation matrix
	*/
	inline const CMatrix CMatrix::TranslationMatrix(const CVector3F & translation) {
		CMatrix matrix;
		return matrix.SetTranslation(translation.x, translation.y, translation.z);
	}

	/**
	* Create a rotation matrix
	* @param rotation A rotation vector in radian
	* @return A rotation matrix
	*/
	inline const CMatrix CMatrix::RotationMatrix(const CVector3F & rotation) {
		CMatrix matrix;
		CQuaternion quaternion;

		CQuaternion::YawPitchRoll(quaternion, rotation.x, rotation.y, rotation.z);
		CMatrix::RotationQuaternion(matrix, quaternion);
		return matrix;
	}

	/**
	* Create a scale matrix
	* @param scale A scale vector
	* @return A scale matrix
	*/
	inline const CMatrix CMatrix::ScaleMatrix(const CVector3F & scale) {
		CMatrix matrix;
		return matrix.SetScale(scale.x, scale.y, scale.z);
	}

	/**
	* * operator override
	* @param matrix A CMatrix
	* @param number A float
	* @return A CMatrix
	*/
	inline const CMatrix operator*(const CMatrix & matrix, const float number) {
		return CMatrix(matrix._11 * number, matrix._12 * number, matrix._13 * number, matrix._14 * number,
			matrix._21 * number, matrix._22 * number, matrix._23 * number, matrix._24 * number,
			matrix._31 * number, matrix._32 * number, matrix._33 * number, matrix._34 * number,
			matrix._41 * number, matrix._42 * number, matrix._43 * number, matrix._44 * number);
	}

	/**
	* * operator override
	* @param matrix A CMatrix
	* @param number A float
	* @return A CMatrix
	*/
	inline const CMatrix operator*(const float number, const CMatrix & matrix) {
		return matrix * number;
	}

	/**
	* / operator override
	* @param matrix A CMatrix
	* @param number A float
	* @return A CMatrix
	*/
	inline const CMatrix operator/(const CMatrix & matrix, const float number) {
		return CMatrix(matrix._11 / number, matrix._12 / number, matrix._13 / number, matrix._14 / number,
			matrix._21 / number, matrix._22 / number, matrix._23 / number, matrix._24 / number,
			matrix._31 / number, matrix._32 / number, matrix._33 / number, matrix._34 / number,
			matrix._41 / number, matrix._42 / number, matrix._43 / number, matrix._44 / number);
	}

	/**
	* / operator override
	* @param matrix A CMatrix
	* @param number A float
	* @return A CMatrix
	*/
	inline const CMatrix operator/(const float number, const CMatrix & matrix) {
		return matrix / number;
	}
}

#endif
