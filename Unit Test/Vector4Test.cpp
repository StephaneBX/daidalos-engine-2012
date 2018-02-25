#include "Vector4Test.hpp"
#include <Core/Core.hpp>
#include <Core/Utility/CVector4.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(Vector4Test);

void Vector4Test::TestConstructors() {
	// Default constuctor
	daidalosengine::CVector4F test1;
	CPPUNIT_ASSERT(test1.x == 0 && test1.y == 0 && test1.z == 0 && test1.w == 0);

	// Value constructor
	daidalosengine::CVector4F test2(10.0F, 20.0F, 30.0F, 40.0F);
	CPPUNIT_ASSERT(test2.x == 10.0F && test2.y == 20.0F && test2.z == 30.0F && test2.w == 40.0F);

	// Copy constructor
	daidalosengine::CVector4F test3(test2);
	CPPUNIT_ASSERT(test3.x == test2.x && test3.y == test2.y && test3.z == test2.z && test3.w == test2.w);

	// Array constructor
	float testArray[4] = { 5.0F, 9.0F, -4.0F, 21.0F };
	daidalosengine::CVector4F test4(testArray);
	CPPUNIT_ASSERT(test4.x == testArray[0] && test4.y == testArray[1] && test4.z == testArray[2] && test4.w == testArray[3]);
}

void Vector4Test::TestOperators() {
	daidalosengine::CVector4F vec1(5.0F, 15.0F, -1.0F, 21.0F);
	daidalosengine::CVector4F vec2(8.0F, 2.0F, -10.0F, -21.0F);

	// = operator
	daidalosengine::CVector4F test = vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w, EPSILON);

	// Addition between two vectors
	test = vec1 + vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z + vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w + vec2.w, EPSILON);

	// Substraction between two vectors
	test = vec1 - vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z - vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w - vec2.w, EPSILON);

	// Multiplication between two vectors
	test = vec1 * vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w * vec2.w, EPSILON);

	// Division between two vectors
	test = vec1 / vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w / vec2.w, EPSILON);

	// Multiplication between a vector and a primitive
	test = vec1 * 2.0F;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w * 2.0F, EPSILON);

	// Multiplication between a vector and a primitive
	test = 2.0F * vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w * 2.0F, EPSILON);

	// Division between a vector and a primitive
	test = vec1 / 2.0F;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w / 2.0F, EPSILON);

	// Division between a primitive and a vector
	test = 2.0F / vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, 2.0F / vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, 2.0F / vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, 2.0F / vec1.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, 2.0F / vec1.w, EPSILON);

	// Negation
	test = -vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, -vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, -vec1.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, -vec1.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, -vec1.w, EPSILON);

	// Addition (+=) between two vectors
	test = vec1;
	test += vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z + vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w + vec2.w, EPSILON);

	// Substraction (-=) between two vectors
	test = vec1;
	test -= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z - vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w - vec2.w, EPSILON);

	// Multiplication (*=) between two vectors
	test = vec1;
	test *= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z * vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w * vec2.w, EPSILON);

	// Division (/=) between two vectors
	test = vec1;
	test /= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.z, vec1.z / vec2.z, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.w, vec1.w / vec2.w, EPSILON);

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
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test2[3], vec1.w, EPSILON);
}

void Vector4Test::TestLength() {
	daidalosengine::CVector4F test(10.0F, 10.0F, 10.0F, 10.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z + test.w * test.w), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector4F::Length(test), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z + test.w * test.w), EPSILON);

	test = daidalosengine::CVector4F(-10.0F, -10.0F, -10.0F, -10.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z + test.w * test.w), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector4F::Length(test), sqrtf(test.x * test.x + test.y * test.y + test.z * test.z + test.w * test.w), EPSILON);
}

void Vector4Test::TestNormalize() {
	daidalosengine::CVector4F test(2.0F, 5.0F, -15.0F, 33.0F);
	CPPUNIT_ASSERT_EQUAL(test.Normalize(), test / test.Length());
}

void Vector4Test::TestGetFrom() {
	std::string test1 = "1.5f:10.0F:-2.0F:-15.0F";
	std::string test2 = "efvgregefeqfe";
	std::string test3 = "3;7;5;9";
	std::string test4 = "";

	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector4F::GetFrom(test1), daidalosengine::CVector4F(1.5f, 10.0F, -2.0F, -15.0F));
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector4F::GetFrom(test2), daidalosengine::CVector4F());
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector4F::GetFrom(test3, daidalosengine::CVector4F(), ";"), daidalosengine::CVector4F(3.0F, 7.0F, 5.0F, 9.0F));
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector4F::GetFrom(test4), daidalosengine::CVector4F());
}
