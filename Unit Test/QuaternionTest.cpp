#include "QuaternionTest.hpp"
#include <Core/Utility/CQuaternion.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(QuaternionTest);

void QuaternionTest::TestConstructors() {
	// Default contructor
	daidalosengine::CQuaternion quaternion;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, quaternion.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, quaternion.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, quaternion.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1, quaternion.w, EPSILON);


	// Value constructor
	daidalosengine::CQuaternion quaternion2(1, 2, 3, 4);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(1, quaternion2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2, quaternion2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3, quaternion2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4, quaternion2.w, EPSILON);


	// Copy constructor
	daidalosengine::CQuaternion quaternion3 = quaternion2;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion2.x, quaternion3.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion2.y, quaternion3.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion2.z, quaternion3.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion2.w, quaternion3.w, EPSILON);

	// Vector / angle constructor
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion4(vector, daidalosengine::DegreeToRadian(30));

	float cosine = std::cosf(daidalosengine::DegreeToRadian(30) / 2.0F);
	float sinus = std::sinf(daidalosengine::DegreeToRadian(30) / 2.0F);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(vector.x * sinus, quaternion4.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(vector.y * sinus, quaternion4.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(vector.z * sinus, quaternion4.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(cosine, quaternion4.w, EPSILON);

	// Array constructor
	float array1[4] = { 1, 2, 3, 4 };
	daidalosengine::CQuaternion quaternion5(array1);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(array1[0], quaternion5.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(array1[1], quaternion5.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(array1[2], quaternion5.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(array1[3], quaternion5.w, EPSILON);
}

void QuaternionTest::TestOperators() {
	daidalosengine::CVector3F vector1 = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion1(vector1, daidalosengine::DegreeToRadian(30));

	daidalosengine::CVector3F vector2 = daidalosengine::CVector3F(-5, 12, -25).Normalize();
	daidalosengine::CQuaternion quaternion2(vector2, daidalosengine::DegreeToRadian(58));

	daidalosengine::CQuaternion tmp;
	float factor = 2.3F;

	// Addition between two quaternions
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x + quaternion2.x, (quaternion1 + quaternion2).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y + quaternion2.y, (quaternion1 + quaternion2).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z + quaternion2.z, (quaternion1 + quaternion2).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w + quaternion2.w, (quaternion1 + quaternion2).w, EPSILON);

	// Addition (+=) between two quaternions
	tmp = quaternion1;
	tmp += quaternion2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x + quaternion2.x, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y + quaternion2.y, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z + quaternion2.z, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w + quaternion2.w, tmp.w, EPSILON);

	// Substraction between two quaternions
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x - quaternion2.x, (quaternion1 - quaternion2).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y - quaternion2.y, (quaternion1 - quaternion2).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z - quaternion2.z, (quaternion1 - quaternion2).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w - quaternion2.w, (quaternion1 - quaternion2).w, EPSILON);

	// Substraction (-=) between two quaternions
	tmp = quaternion1;
	tmp -= quaternion2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x - quaternion2.x, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y - quaternion2.y, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z - quaternion2.z, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w - quaternion2.w, tmp.w, EPSILON);


	// Multiplication between two quaternions
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.y * quaternion2.z + quaternion1.z * quaternion2.y + quaternion1.w * quaternion2.x + quaternion1.x * quaternion2.w, (quaternion1 * quaternion2).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.z * quaternion2.x + quaternion1.x * quaternion2.z + quaternion1.w * quaternion2.y + quaternion1.y * quaternion2.w, (quaternion1 * quaternion2).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.x * quaternion2.y + quaternion1.y * quaternion2.x + quaternion1.w * quaternion2.z + quaternion1.z * quaternion2.w, (quaternion1 * quaternion2).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w * quaternion2.w - quaternion1.x * quaternion2.x - quaternion1.y * quaternion2.y - quaternion1.z * quaternion2.z, (quaternion1 * quaternion2).w, EPSILON);

	// Multiplication between a quaternion and a number
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x * factor, (quaternion1 * factor).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y * factor, (quaternion1 * factor).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z * factor, (quaternion1 * factor).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w * factor, (quaternion1 * factor).w, EPSILON);

	// Multiplication (*=) between two quaternions
	tmp = quaternion1;
	tmp *= quaternion2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.y * quaternion2.z + quaternion1.z * quaternion2.y + quaternion1.w * quaternion2.x + quaternion1.x * quaternion2.w, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.z * quaternion2.x + quaternion1.x * quaternion2.z + quaternion1.w * quaternion2.y + quaternion1.y * quaternion2.w, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.x * quaternion2.y + quaternion1.y * quaternion2.x + quaternion1.w * quaternion2.z + quaternion1.z * quaternion2.w, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w * quaternion2.w - quaternion1.x * quaternion2.x - quaternion1.y * quaternion2.y - quaternion1.z * quaternion2.z, tmp.w, EPSILON);

	// Multiplication (*=) between a quaternion and a number
	tmp = quaternion1;
	tmp *= factor;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x * factor, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y * factor, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z * factor, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w * factor, tmp.w, EPSILON);

	// Division between two quaternions
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).x, (quaternion1 / quaternion2).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).y, (quaternion1 / quaternion2).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).z, (quaternion1 / quaternion2).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).w, (quaternion1 / quaternion2).w, EPSILON);

	// Division between a quaternion and a number
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x / factor, (quaternion1 / factor).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y / factor, (quaternion1 / factor).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z / factor, (quaternion1 / factor).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w / factor, (quaternion1 / factor).w, EPSILON);

	// Division (/=) between two quaternions
	tmp = quaternion1;
	tmp /= quaternion2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).x, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).y, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).z, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1 * quaternion2.Inverse()).w, tmp.w, EPSILON);

	// Division (/=) between a quaternion and a number
	tmp = quaternion1;
	tmp /= factor;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x / factor, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y / factor, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z / factor, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w / factor, tmp.w, EPSILON);

	// Negation
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.x, (-quaternion1).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.y, (-quaternion1).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.z, (-quaternion1).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion1.w, (-quaternion1).w, EPSILON);

	// Equal
	tmp = quaternion1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x, tmp.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y, tmp.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z, tmp.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w, tmp.w, EPSILON);

	// Not equal
	tmp = quaternion2;
	CPPUNIT_ASSERT(quaternion1.x != tmp.x);
	CPPUNIT_ASSERT(quaternion1.y != tmp.y);
	CPPUNIT_ASSERT(quaternion1.z != tmp.z);
	CPPUNIT_ASSERT(quaternion1.w != tmp.w);

	// Pointer access
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x, quaternion1[0], EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.y, quaternion1[1], EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.z, quaternion1[2], EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.w, quaternion1[3], EPSILON);
}

void QuaternionTest::TestInverse() {
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion(vector, daidalosengine::DegreeToRadian(30));

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion.Normalize().x, quaternion.Inverse().x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion.Normalize().y, quaternion.Inverse().y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-quaternion.Normalize().z, quaternion.Inverse().z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion.Normalize().w, quaternion.Inverse().w, EPSILON);
}

void QuaternionTest::TestNormalize() {
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion(vector, daidalosengine::DegreeToRadian(30));

	float length = std::sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion.x / length, quaternion.Normalize().x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion.y / length, quaternion.Normalize().y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion.z / length, quaternion.Normalize().z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion.w / length, quaternion.Normalize().w, EPSILON);
}

void QuaternionTest::TestLog() {
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion(vector, daidalosengine::DegreeToRadian(30));

	float a = (float)acos(quaternion.w);
	float sina = (float)sin(a);
	daidalosengine::CQuaternion ret;

	ret.w = 0;
	if(sina > EPSILON) {
		ret.x = a*quaternion.x / sina;
		ret.y = a*quaternion.y / sina;
		ret.z = a*quaternion.z / sina;
	} else {
		ret.x = ret.y = ret.z = 0;
	}

	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.x, quaternion.Log().x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.y, quaternion.Log().y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.z, quaternion.Log().z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.w, quaternion.Log().w, EPSILON);
}

void QuaternionTest::TestExp() {
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion(vector, daidalosengine::DegreeToRadian(30));

	float a = sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	float sina = (float)sin(a);
	float cosa = (float)cos(a);
	daidalosengine::CQuaternion ret;

	ret.w = cosa;
	if(a > 0) {
		ret.x = sina * quaternion.x / a;
		ret.y = sina * quaternion.y / a;
		ret.z = sina * quaternion.z / a;
	} else {
		ret.x = ret.y = ret.z = 0;
	}

	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.x, quaternion.Exp().x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.y, quaternion.Exp().y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.z, quaternion.Exp().z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(ret.w, quaternion.Exp().w, EPSILON);
}

void QuaternionTest::TestPower() {
	daidalosengine::CVector3F vector1 = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion1(vector1, daidalosengine::DegreeToRadian(30));
	float power = 3.27F;

	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1.Log() * power).Exp().x, quaternion1.Power(power).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1.Log() * power).Exp().y, quaternion1.Power(power).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1.Log() * power).Exp().z, quaternion1.Power(power).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((quaternion1.Log() * power).Exp().w, quaternion1.Power(power).w, EPSILON);
}

void QuaternionTest::TestYawPitchRoll() {
	daidalosengine::CVector3F vectorX = daidalosengine::CVector3F(1, 0, 0);
	daidalosengine::CVector3F vectorY = daidalosengine::CVector3F(0, 1, 0);
	daidalosengine::CVector3F vectorZ = daidalosengine::CVector3F(0, 0, 1);

	float angleX = 30;
	float angleY = 45;
	float angleZ = 15;

	daidalosengine::CQuaternion result;
	daidalosengine::CQuaternion::YawPitchRoll(result, angleX, angleY, angleZ);

	CPPUNIT_ASSERT_DOUBLES_EQUAL((daidalosengine::CQuaternion(vectorZ, angleZ) * daidalosengine::CQuaternion(vectorX, angleX) * daidalosengine::CQuaternion(vectorY, angleY)).x, result.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((daidalosengine::CQuaternion(vectorZ, angleZ) * daidalosengine::CQuaternion(vectorX, angleX) * daidalosengine::CQuaternion(vectorY, angleY)).y, result.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((daidalosengine::CQuaternion(vectorZ, angleZ) * daidalosengine::CQuaternion(vectorX, angleX) * daidalosengine::CQuaternion(vectorY, angleY)).z, result.z, EPSILON);
}

void QuaternionTest::TestLerp() {
	daidalosengine::CVector3F vector1 = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion1(vector1, daidalosengine::DegreeToRadian(30));

	daidalosengine::CVector3F vector2 = daidalosengine::CVector3F(-5, 12, -25).Normalize();
	daidalosengine::CQuaternion quaternion2(vector2, daidalosengine::DegreeToRadian(58));

	float factor = 0.72F;

	daidalosengine::CQuaternion result = (quaternion1 * (1 - factor) + quaternion2 * factor).Normalize();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.x, quaternion1.Lerp(quaternion2, factor).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.y, quaternion1.Lerp(quaternion2, factor).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.z, quaternion1.Lerp(quaternion2, factor).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.w, quaternion1.Lerp(quaternion2, factor).w, EPSILON);
}

void QuaternionTest::TestSlerp() {
	daidalosengine::CVector3F vector1 = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion1(vector1, daidalosengine::DegreeToRadian(30));

	daidalosengine::CVector3F vector2 = daidalosengine::CVector3F(-5, 12, -25).Normalize();
	daidalosengine::CQuaternion quaternion2(vector2, daidalosengine::DegreeToRadian(58));

	float factor = 0.72F;
	daidalosengine::CQuaternion quaternion3;
	float dot = quaternion1.DotProduct(quaternion2);

	daidalosengine::CQuaternion result;

	if(dot < 0) {
		dot = -dot;
		quaternion3 = -quaternion2;
	} else quaternion3 = quaternion2;

	if(dot < 0.95F) {
		float angle = acosf(dot);
		result = (quaternion1 * sinf(angle * (1 - factor)) + quaternion3 * sinf(angle * factor)) / sinf(angle);
	} else
		result = quaternion1.Lerp(quaternion2, factor);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.x, quaternion1.Slerp(quaternion2, factor).x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.y, quaternion1.Slerp(quaternion2, factor).y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.z, quaternion1.Slerp(quaternion2, factor).z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(result.w, quaternion1.Slerp(quaternion2, factor).w, EPSILON);
}

void QuaternionTest::TestLength() {
	daidalosengine::CVector3F vector = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion(vector, daidalosengine::DegreeToRadian(30));

	float length = std::sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(length, quaternion.Length(), EPSILON);
}

void QuaternionTest::TestDotProduct() {
	daidalosengine::CVector3F vector1 = daidalosengine::CVector3F(1, 2, 3).Normalize();
	daidalosengine::CQuaternion quaternion1(vector1, daidalosengine::DegreeToRadian(30));

	daidalosengine::CVector3F vector2 = daidalosengine::CVector3F(-5, 12, -25).Normalize();
	daidalosengine::CQuaternion quaternion2(vector2, daidalosengine::DegreeToRadian(58));

	CPPUNIT_ASSERT_DOUBLES_EQUAL(quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y + quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w, quaternion1.DotProduct(quaternion2), EPSILON);
}
