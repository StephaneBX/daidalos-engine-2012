#include "Vector3Test.h"
#include <Core/Core.h>
#include <Core/Utility/CVector3.h>

CPPUNIT_TEST_SUITE_REGISTRATION(Vector3Test);

void Vector3Test::TestConstructors() {
	// Default constuctor
	DE::CVector3F test1;
	CPPUNIT_ASSERT(test1.x == 0 && test1.y == 0 && test1.z == 0);

	// Value constructor
	DE::CVector3F test2(10.0f, 20.0f, 30.0f);
	CPPUNIT_ASSERT(test2.x == 10.0f && test2.y == 20.0f && test2.z == 30.0f);

	// Copy constructor
	DE::CVector3F test3(test2);
	CPPUNIT_ASSERT(test3.x == test2.x && test3.y == test2.y && test3.z == test2.z);

	// Array constructor
	float testArray[3] = { 5.0f, 9.0f, -4.0f };
	DE::CVector3F test4(testArray);
	CPPUNIT_ASSERT(test4.x == testArray[0] && test4.y == testArray[1] && test4.z == testArray[2]);
}

void Vector3Test::TestOperators() {
	DE::CVector3F vec1(5.0f, 15.0f, -1.0f);
	DE::CVector3F vec2(8.0f, 2.0f, -10.0f);

	// = operator
	DE::CVector3F test = vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z, EPSILON);

	// Addition between two vectors
	test = vec1 + vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z + vec2.z, EPSILON);

	// Substraction between two vectors
	test = vec1 - vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z - vec2.z, EPSILON);

	// Multiplication between two vectors
	test = vec1 * vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * vec2.z, EPSILON);

	// Division between two vectors
	test = vec1 / vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / vec2.z, EPSILON);

	// Multiplication between a vector and a primitive
	test = vec1 * 2.0f;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * 2.0f, EPSILON);

	// Multiplication between a vector and a primitive
	test = 2.0f * vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * 2.0f, EPSILON);

	// Division between a vector and a primitive
	test = vec1 / 2.0f;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / 2.0f, EPSILON);

	// Division between a primitive and a vector
	test = 2.0f / vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, 2.0f / vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, 2.0f / vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, 2.0f / vec1.z, EPSILON);

	// Negation
	test = -vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, -vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, -vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, -vec1.z, EPSILON);

	// Addition (+=) between two vectors
	test = vec1;
	test += vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z + vec2.z, EPSILON);

	// Substraction (-=) between two vectors
	test = vec1;
	test -= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z - vec2.z, EPSILON);

	// Multiplication (*=) between two vectors
	test = vec1;
	test *= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * vec2.z, EPSILON);

	// Division (/=) between two vectors
	test = vec1;
	test /= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / vec2.z, EPSILON);

	// Multiplication (*=) between a vector and a primitive
	test = vec1;
	test *= 2.0f;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0f, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * 2.0f, EPSILON);

	// Equal
	test = vec1;
	CPPUNIT_ASSERT_EQUAL(test, vec1);

	// Not equal
	test = vec2;
	CPPUNIT_ASSERT(!(test == vec1));

	// Pointer access
	float * test2 = vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test2[0], vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test2[1], vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test2[2], vec1.z, EPSILON);
}

void Vector3Test::TestLength() {
	DE::CVector3F test(10.0f, 10.0f, 10.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::Length(test), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z), EPSILON);

	test = DE::CVector3F(-10.0f, -10.0f, -10.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::Length(test), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z), EPSILON);
}

void Vector3Test::TestDotProduct() {
	DE::CVector3F test1(2.0f, 5.0f, -4.0f);
	DE::CVector3F test2(8.0f, 3.0f, 11.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.DotProduct(test2), test1.x * test2.x + test1.y * test2.y + test1.z * test2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::DotProduct(test1, test2), test1.x * test2.x + test1.y * test2.y + test1.z * test2.z, EPSILON);
}

void Vector3Test::TestCrossProduct() {
	DE::CVector3F test1(2.0f, 5.0f, -4.0f);
	DE::CVector3F test2(8.0f, 3.0f, 11.0f);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CrossProduct(test2).x, test1.y * test2.z - test1.z * test2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CrossProduct(test2).y, test1.z * test2.x - test1.x * test2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CrossProduct(test2).z, test1.x * test2.y - test1.y * test2.x, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CrossProduct(test1, test2).x, test1.y * test2.z - test1.z * test2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CrossProduct(test1, test2).y, test1.z * test2.x - test1.x * test2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CrossProduct(test1, test2).z, test1.x * test2.y - test1.y * test2.x, EPSILON);
}

void Vector3Test::TestDistance() {
	DE::CVector3F test1(2.0f, 5.0f, -15.0f);
	DE::CVector3F test2(8.0f, 3.0f, 10.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.Distance(test2), (test2 - test1).Length(), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::Distance(test1, test2), (test2 - test1).Length(), EPSILON);
}

void Vector3Test::TestNormalize() {
	DE::CVector3F test(2.0f, 5.0f, -15.0f);
	CPPUNIT_ASSERT_EQUAL(test.Normalize(), test / test.Length());
}

void Vector3Test::TestGetFrom() {
	std::string test1 = "1.5f:10.0f:-2.0f";
	std::string test2 = "efvgregefeqfe";
	std::string test3 = "3;7;5";
	std::string test4 = "";

	CPPUNIT_ASSERT_EQUAL(DE::CVector3F::GetFrom(test1), DE::CVector3F(1.5f, 10.0f, -2.0f));
	CPPUNIT_ASSERT_EQUAL(DE::CVector3F::GetFrom(test2), DE::CVector3F());
	CPPUNIT_ASSERT_EQUAL(DE::CVector3F::GetFrom(test3, DE::CVector3F(), ";"), DE::CVector3F(3.0f, 7.0f, 5.0f));
	CPPUNIT_ASSERT_EQUAL(DE::CVector3F::GetFrom(test4), DE::CVector3F());
}

void Vector3Test::TestLinearInterpolation() {
	DE::CVector3F test1(2.0f, 5.0f, -15.0f);
	DE::CVector3F test2(8.0f, 3.0f, 10.0f);
	float interpolation = 0.3f;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.LinearInterpolation(test2, interpolation).x, test1.x + interpolation * (test2.x - test1.x), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.LinearInterpolation(test2, interpolation).y, test1.y + interpolation * (test2.y - test1.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.LinearInterpolation(test2, interpolation).z, test1.z + interpolation * (test2.z - test1.z), EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::LinearInterpolation(test1, test2, interpolation).x, test1.x + interpolation * (test2.x - test1.x), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::LinearInterpolation(test1, test2, interpolation).y, test1.y + interpolation * (test2.y - test1.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::LinearInterpolation(test1, test2, interpolation).z, test1.z + interpolation * (test2.z - test1.z), EPSILON);
}

void Vector3Test::TestCosineInterpolation() {
	DE::CVector3F test1(2.0f, 5.0f, -15.0f);
	DE::CVector3F test2(8.0f, 3.0f, 10.0f);
	float interpolation = 0.3f;
	float interpolation2 = (1 - std::cosf(interpolation * PI)) / 2.0f;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CosineInterpolation(test2, interpolation).x, test1.x + interpolation2 * (test2.x - test1.x), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CosineInterpolation(test2, interpolation).y, test1.y + interpolation2 * (test2.y - test1.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.CosineInterpolation(test2, interpolation).z, test1.z + interpolation2 * (test2.z - test1.z), EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CosineInterpolation(test1, test2, interpolation).x, test1.x + interpolation2 * (test2.x - test1.x), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CosineInterpolation(test1, test2, interpolation).y, test1.y + interpolation2 * (test2.y - test1.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(DE::CVector3F::CosineInterpolation(test1, test2, interpolation).z, test1.z + interpolation2 * (test2.z - test1.z), EPSILON);
}
