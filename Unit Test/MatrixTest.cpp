#include "MatrixTest.hpp"
#include <Core/Utility/CMatrix.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);

void MatrixTest::setUp() {

}

void MatrixTest::TestConstructors() {
	// Default constructor
	daidalosengine::CMatrix matrix;
	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);

	// Value constructor
	daidalosengine::CMatrix matrix2(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
	CPPUNIT_ASSERT(matrix2._11 == 1 && matrix2._12 == 2 && matrix2._13 == 3 && matrix2._14 == 4 &&
		matrix2._21 == 1 && matrix2._22 == 2 && matrix2._23 == 3 && matrix2._24 == 4 &&
		matrix2._31 == 1 && matrix2._32 == 2 && matrix2._33 == 3 && matrix2._34 == 4 &&
		matrix2._41 == 1 && matrix2._42 == 2 && matrix2._43 == 3 && matrix2._44 == 4);

	// Copy constructor
	daidalosengine::CMatrix matrix3(matrix2);
	CPPUNIT_ASSERT(matrix3._11 == matrix2._11 && matrix3._12 == matrix2._12 && matrix3._13 == matrix2._13 && matrix3._14 == matrix2._14 &&
		matrix3._21 == matrix2._21 && matrix3._22 == matrix2._22 && matrix3._23 == matrix2._23 && matrix3._24 == matrix2._24 &&
		matrix3._31 == matrix2._31 && matrix3._32 == matrix2._32 && matrix3._33 == matrix2._33 && matrix3._34 == matrix2._34 &&
		matrix3._41 == matrix2._41 && matrix3._42 == matrix2._42 && matrix3._43 == matrix2._43 && matrix3._44 == matrix2._44);

	// Vectors constructor
	daidalosengine::CMatrix matrix4(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(0, 0, 0), daidalosengine::CVector3F(10, 20, 30));
}

void MatrixTest::TestOperators() {
	daidalosengine::CMatrix matrix1(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(30), daidalosengine::DegreeToRadian(40), daidalosengine::DegreeToRadian(50)), daidalosengine::CVector3F(10, 20, 30));
	daidalosengine::CMatrix matrix2(daidalosengine::CVector3F(4, 5, 6), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(15)), daidalosengine::CVector3F(-10, -15, 20));
	float factor = 3.5F;

	// + operator
	daidalosengine::CMatrix matrix3 = matrix1 + matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 + matrix2._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 + matrix2._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 + matrix2._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 + matrix2._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 + matrix2._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 + matrix2._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 + matrix2._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 + matrix2._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 + matrix2._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 + matrix2._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 + matrix2._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 + matrix2._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 + matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 + matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 + matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 + matrix2._44, matrix3._44, EPSILON);


	// += operator
	matrix3 = matrix1;
	matrix3 += matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 + matrix2._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 + matrix2._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 + matrix2._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 + matrix2._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 + matrix2._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 + matrix2._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 + matrix2._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 + matrix2._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 + matrix2._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 + matrix2._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 + matrix2._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 + matrix2._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 + matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 + matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 + matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 + matrix2._44, matrix3._44, EPSILON);


	// - operator
	matrix3 = matrix1 - matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 - matrix2._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 - matrix2._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 - matrix2._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 - matrix2._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 - matrix2._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 - matrix2._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 - matrix2._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 - matrix2._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 - matrix2._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 - matrix2._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 - matrix2._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 - matrix2._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 - matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 - matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 - matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 - matrix2._44, matrix3._44, EPSILON);

	// -= operator
	matrix3 = matrix1;
	matrix3 -= matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 - matrix2._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 - matrix2._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 - matrix2._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 - matrix2._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 - matrix2._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 - matrix2._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 - matrix2._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 - matrix2._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 - matrix2._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 - matrix2._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 - matrix2._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 - matrix2._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 - matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 - matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 - matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 - matrix2._44, matrix3._44, EPSILON);

	// negative operator
	matrix3 = -matrix1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-matrix1._44, matrix3._44, EPSILON);


	// * operator (matrix)
	matrix3 = matrix1 * matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44, matrix3._44, EPSILON);

	// *= operator (matrix)
	matrix3 = matrix1;
	matrix3 *= matrix2;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44, matrix3._44, EPSILON);

	// * operator (float)
	matrix3 = matrix1 * factor;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * factor, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 * factor, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 * factor, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 * factor, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * factor, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 * factor, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 * factor, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 * factor, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * factor, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 * factor, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 * factor, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 * factor, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * factor, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 * factor, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 * factor, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 * factor, matrix3._44, EPSILON);

	// *= operator (float)
	matrix3 = matrix1;
	matrix3 *= factor;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11 * factor, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12 * factor, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13 * factor, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14 * factor, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21 * factor, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22 * factor, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23 * factor, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24 * factor, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31 * factor, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32 * factor, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33 * factor, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34 * factor, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41 * factor, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42 * factor, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43 * factor, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44 * factor, matrix3._44, EPSILON);

	// = operator
	matrix3 = matrix1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._11, matrix3._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._12, matrix3._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._13, matrix3._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._14, matrix3._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._21, matrix3._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._22, matrix3._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._23, matrix3._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._24, matrix3._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._31, matrix3._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._32, matrix3._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._33, matrix3._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._34, matrix3._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._41, matrix3._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._42, matrix3._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._43, matrix3._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix1._44, matrix3._44, EPSILON);

	// == operator
	matrix3 = matrix1;
	bool test = matrix3._11 == matrix1._11 && matrix3._12 == matrix1._12 && matrix3._13 == matrix1._13 && matrix3._14 == matrix1._14 &&
		matrix3._21 == matrix1._21 && matrix3._22 == matrix1._22 && matrix3._23 == matrix1._23 && matrix3._24 == matrix1._24 &&
		matrix3._31 == matrix1._31 && matrix3._32 == matrix1._32 && matrix3._33 == matrix1._33 && matrix3._34 == matrix1._34 &&
		matrix3._41 == matrix1._41 && matrix3._42 == matrix1._42 && matrix3._43 == matrix1._43 && matrix3._44 == matrix1._44;
	CPPUNIT_ASSERT_EQUAL(matrix3 == matrix1, test);

	// != operator
	CPPUNIT_ASSERT_EQUAL(matrix3 != matrix1, !test);
}

void MatrixTest::TestInverse() {
	CPPUNIT_FAIL("Not implemented");
}

void MatrixTest::TestTranspose() {
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::RotationMatrix(daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)));
	daidalosengine::CMatrix expected = daidalosengine::CMatrix(matrix._11, matrix._21, matrix._31, matrix._41,
		matrix._12, matrix._22, matrix._32, matrix._42,
		matrix._13, matrix._23, matrix._33, matrix._43,
		matrix._14, matrix._24, matrix._34, matrix._44);

	matrix = matrix.Transpose();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._11, expected._11, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._12, expected._12, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._13, expected._13, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._14, expected._14, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._21, expected._21, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._22, expected._22, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._23, expected._23, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._24, expected._24, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._31, expected._31, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._32, expected._32, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._33, expected._33, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._34, expected._34, EPSILON);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._41, expected._41, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._42, expected._42, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._43, expected._43, EPSILON);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(matrix._44, expected._44, EPSILON);
}

void MatrixTest::TestIdentity() {
	daidalosengine::CMatrix matrix;
	matrix.Identity();
	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);
}

void MatrixTest::TestSetTranslation() {
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::TranslationMatrix(daidalosengine::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 1 && matrix._42 == 2 && matrix._43 == 3 && matrix._44 == 1);
}

void MatrixTest::TestSetRotation() {
	//daidalosengine::CMatrix matrix = daidalosengine::CMatrix::RotationMatrix(daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)));

	CPPUNIT_FAIL("Not implemented");
}

void MatrixTest::TestSetScale() {
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::ScaleMatrix(daidalosengine::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 2 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 3 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);
}

void MatrixTest::TestGetScale() {
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::ScaleMatrix(daidalosengine::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix == matrix.GetScale());
}

void MatrixTest::TestGetTranslation() {
	daidalosengine::CVector3F translation(10, 20, 30);
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::TranslationMatrix(translation);

	CPPUNIT_ASSERT(translation == matrix.GetTranslation());
}

void MatrixTest::TestGetRotation() {
	daidalosengine::CMatrix matrix = daidalosengine::CMatrix::RotationMatrix(daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)));

	CPPUNIT_ASSERT(matrix == matrix.GetRotation());
}

void MatrixTest::TestTransformVector3() {
	daidalosengine::CMatrix matrix(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)), daidalosengine::CVector3F(10, 20, 30));
	daidalosengine::CVector3F vector(-5, 87, 20);

	daidalosengine::CVector3F expected(matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43);

	CPPUNIT_ASSERT(matrix.Transform(vector) == expected);
	CPPUNIT_ASSERT(matrix.Transform(vector, true) == expected.Normalize());
}

void MatrixTest::TestTransformVector4() {
	daidalosengine::CMatrix matrix(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)), daidalosengine::CVector3F(10, 20, 30));
	daidalosengine::CVector4F vector(-5, 87, 20, 8);

	daidalosengine::CVector4F expected(matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w,
		matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w);

	CPPUNIT_ASSERT(matrix.Transform(vector) == expected);
	CPPUNIT_ASSERT(matrix.Transform(vector, true) == expected.Normalize());
}

void MatrixTest::TestDeterminant() {
	daidalosengine::CMatrix matrix(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)), daidalosengine::CVector3F(10, 20, 30));

	float determinant = matrix._11 * (matrix._22 * (matrix._33 * matrix._44 - matrix._43 * matrix._34) - matrix._32 * (matrix._23 * matrix._44 - matrix._43 * matrix._24) + matrix._42 * (matrix._23 * matrix._34 - matrix._33 * matrix._24)) -
		matrix._21 * (matrix._32 * (matrix._43 * matrix._14 - matrix._13 * matrix._44) - matrix._42 * (matrix._33 * matrix._14 - matrix._13 * matrix._34) + matrix._12 * (matrix._33 * matrix._44 - matrix._43 * matrix._34)) +
		matrix._31 * (matrix._42 * (matrix._13 * matrix._24 - matrix._23 * matrix._14) - matrix._12 * (matrix._43 * matrix._24 - matrix._23 * matrix._44) + matrix._22 * (matrix._43 * matrix._14 - matrix._13 * matrix._44)) -
		matrix._41 * (matrix._12 * (matrix._23 * matrix._34 - matrix._33 * matrix._24) - matrix._22 * (matrix._13 * matrix._34 - matrix._33 * matrix._14) + matrix._32 * (matrix._13 * matrix._24 - matrix._23 * matrix._14));

	CPPUNIT_ASSERT_EQUAL(determinant, matrix.Determinant());
}

void MatrixTest::TestTransformArray() {
	const int n = 10;
	daidalosengine::CMatrix matrix(daidalosengine::CVector3F(1, 2, 3), daidalosengine::CVector3F(daidalosengine::DegreeToRadian(10), daidalosengine::DegreeToRadian(20), daidalosengine::DegreeToRadian(30)), daidalosengine::CVector3F(10, 20, 30));
	daidalosengine::CVector3F vectors[n];
	daidalosengine::CVector3F vectors_out[n];
	for(int i = 0; i < n; ++i) {
		vectors[i] = daidalosengine::CVector3F(-5 * n, 87 * n, 20 * -n);
	}

	daidalosengine::CMatrix::TransformArray(vectors_out, vectors, matrix, n, false);
	for(int i = 0; i < n; ++i) {
		daidalosengine::CVector3F expected(matrix._11 * vectors[i].x + matrix._21 * vectors[i].y + matrix._31 * vectors[i].z + matrix._41,
			matrix._12 * vectors[i].x + matrix._22 * vectors[i].y + matrix._32 * vectors[i].z + matrix._42,
			matrix._13 * vectors[i].x + matrix._23 * vectors[i].y + matrix._33 * vectors[i].z + matrix._43);
		CPPUNIT_ASSERT(vectors_out[i] == expected);
	}

	daidalosengine::CMatrix::TransformArray(vectors_out, vectors, matrix, n, true);
	for(int i = 0; i < n; ++i) {
		daidalosengine::CVector3F expected(matrix._11 * vectors[i].x + matrix._21 * vectors[i].y + matrix._31 * vectors[i].z + matrix._41,
			matrix._12 * vectors[i].x + matrix._22 * vectors[i].y + matrix._32 * vectors[i].z + matrix._42,
			matrix._13 * vectors[i].x + matrix._23 * vectors[i].y + matrix._33 * vectors[i].z + matrix._43);
		expected = expected.Normalize();
		CPPUNIT_ASSERT(vectors_out[i] == expected);
	}
}

void MatrixTest::TestRotationQuaternion() {
	CPPUNIT_FAIL("Not implemented");
}

void MatrixTest::TestDecompose() {
	CPPUNIT_FAIL("Not implemented");
}
