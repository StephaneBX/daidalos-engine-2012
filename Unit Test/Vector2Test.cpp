#include "Vector2Test.hpp"
#include <Core/Core.hpp>
#include <Core/Utility/CVector2.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(Vector2Test);

void Vector2Test::TestConstructors() {
	// Default constuctor
	daidalosengine::CVector2F test1;
	CPPUNIT_ASSERT(test1.x == 0 && test1.y == 0);

	// Value constructor
	daidalosengine::CVector2F test2(10.0F, 20.0F);
	CPPUNIT_ASSERT(test2.x == 10.0F && test2.y == 20.0F);

	// Copy constructor
	daidalosengine::CVector2F test3(test2);
	CPPUNIT_ASSERT(test3.x == 10.0F && test3.y == 20.0F);

	// Array constructor
	float testArray[2] = { 5.0F, 9.0F };
	daidalosengine::CVector2F test4(testArray);
	CPPUNIT_ASSERT(test4.x == testArray[0] && test4.y == testArray[1]);
}

void Vector2Test::TestOperators() {
	daidalosengine::CVector2F vec1(5.0F, 15.0F);
	daidalosengine::CVector2F vec2(8.0F, 2.0F);

	// = operator
	daidalosengine::CVector2F test = vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y, EPSILON);

	// Addition between two vectors
	test = vec1 + vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);

	// Substraction between two vectors
	test = vec1 - vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);

	// Multiplication between two vectors
	test = vec1 * vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);

	// Division between two vectors
	test = vec1 / vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);

	// Multiplication between a vector and a primitive
	test = vec1 * 2.0F;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0F, EPSILON);

	// Multiplication between a vector and a primitive
	test = 2.0F * vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0F, EPSILON);

	// Division between a vector and a primitive
	test = vec1 / 2.0F;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / 2.0F, EPSILON);

	// Division between a vector and a primitive
	test = 2.0F / vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, 2.0F / vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, 2.0F / vec1.y, EPSILON);

	// Negation
	test = -vec1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, -vec1.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, -vec1.y, EPSILON);

	// Addition (+=) between two vectors
	test = vec1;
	test += vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x + vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y + vec2.y, EPSILON);

	// Substraction (-=) between two vectors
	test = vec1;
	test -= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x - vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y - vec2.y, EPSILON);

	// Multiplication (*=) between two vectors
	test = vec1;
	test *= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * vec2.y, EPSILON);

	// Division (/=) between two vectors
	test = vec1;
	test /= vec2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x / vec2.x, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y / vec2.y, EPSILON);

	// Multiplication (*=) between a vector and a primitive
	test = vec1;
	test *= 2.0F;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.x, vec1.x * 2.0F, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.y, vec1.y * 2.0F, EPSILON);

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
}

void Vector2Test::TestLength() {
	daidalosengine::CVector2F test(10.0F, 10.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector2F::Length(test), sqrtf(test.x * test.x + test.y * test.y), EPSILON);

	test = daidalosengine::CVector2F(-10.0F, -10.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test.Length(), sqrtf(test.x * test.x + test.y * test.y), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector2F::Length(test), sqrtf(test.x * test.x + test.y * test.y), EPSILON);
}

void Vector2Test::TestDotProduct() {
	daidalosengine::CVector2F test1(2.0F, 5.0F);
	daidalosengine::CVector2F test2(8.0F, 3.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.DotProduct(test2), test1.x * test2.x + test1.y * test2.y, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector2F::DotProduct(test1, test2), test1.x * test2.x + test1.y * test2.y, EPSILON);
}

void Vector2Test::TestDistance() {
	daidalosengine::CVector2F test1(2.0F, 5.0F);
	daidalosengine::CVector2F test2(8.0F, 3.0F);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(test1.Distance(test2), (test2 - test1).Length(), EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(daidalosengine::CVector2F::Distance(test1, test2), (test2 - test1).Length(), EPSILON);
}

void Vector2Test::TestNormalize() {
	daidalosengine::CVector2F test(2.0F, 5.0F);
	CPPUNIT_ASSERT_EQUAL(test.Normalize(), test / test.Length());
}

void Vector2Test::TestGetFrom() {
	std::string test1 = "1.5f:10.0F";
	std::string test2 = "efvgregefeqfe";
	std::string test3 = "3;7";
	std::string test4 = "";

	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector2F::GetFrom(test1), daidalosengine::CVector2F(1.5F, 10.0F));
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector2F::GetFrom(test2), daidalosengine::CVector2F());
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector2F::GetFrom(test3, daidalosengine::CVector2F(), ";"), daidalosengine::CVector2F(3.0F, 7.0F));
	CPPUNIT_ASSERT_EQUAL(daidalosengine::CVector2F::GetFrom(test4), daidalosengine::CVector2F());
}
