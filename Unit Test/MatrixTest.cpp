#include "MatrixTest.h"
#include <Core/Utility/CMatrix.h>

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);

void MatrixTest::setUp() {

}

void MatrixTest::TestConstructors() {
	// Default constructor
	DE::CMatrix matrix;
	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);

	// Value constructor
	DE::CMatrix matrix2(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
	CPPUNIT_ASSERT(matrix2._11 == 1 && matrix2._12 == 2 && matrix2._13 == 3 && matrix2._14 == 4 &&
		matrix2._21 == 1 && matrix2._22 == 2 && matrix2._23 == 3 && matrix2._24 == 4 &&
		matrix2._31 == 1 && matrix2._32 == 2 && matrix2._33 == 3 && matrix2._34 == 4 &&
		matrix2._41 == 1 && matrix2._42 == 2 && matrix2._43 == 3 && matrix2._44 == 4);

	// Copy constructor
	DE::CMatrix matrix3(matrix2);
	CPPUNIT_ASSERT(matrix3._11 == matrix2._11 && matrix3._12 == matrix2._12 && matrix3._13 == matrix2._13 && matrix3._14 == matrix2._14 &&
		matrix3._21 == matrix2._21 && matrix3._22 == matrix2._22 && matrix3._23 == matrix2._23 && matrix3._24 == matrix2._24 &&
		matrix3._31 == matrix2._31 && matrix3._32 == matrix2._32 && matrix3._33 == matrix2._33 && matrix3._34 == matrix2._34 &&
		matrix3._41 == matrix2._41 && matrix3._42 == matrix2._42 && matrix3._43 == matrix2._43 && matrix3._44 == matrix2._44);

	// Vectors constructor
	DE::CMatrix matrix4(DE::CVector3F(1, 2, 3), DE::CVector3F(0, 0, 0), DE::CVector3F(10, 20, 30));
}

void MatrixTest::TestOperators() {
	DE::CMatrix matrix1(DE::CVector3F(1, 2, 3), DE::CVector3F(DE::DegreeToRadian(30), DE::DegreeToRadian(40), DE::DegreeToRadian(50)), DE::CVector3F(10, 20, 30));
	DE::CMatrix matrix2(DE::CVector3F(4, 5, 6), DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(15)), DE::CVector3F(-10, -15, 20));
	float factor = 3.5f;

	// + operator
	DE::CMatrix matrix3 = matrix1 + matrix2;
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
	DE::CMatrix matrix = DE::CMatrix::RotationMatrix(DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)));
	DE::CMatrix expected = DE::CMatrix(matrix._11, matrix._21, matrix._31, matrix._41,
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
	DE::CMatrix matrix;
	matrix.Identity();
	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);
}

void MatrixTest::TestSetTranslation() {
	DE::CMatrix matrix = DE::CMatrix::TranslationMatrix(DE::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 1 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 1 && matrix._34 == 0 &&
		matrix._41 == 1 && matrix._42 == 2 && matrix._43 == 3 && matrix._44 == 1);
}

void MatrixTest::TestSetRotation() {
	DE::CMatrix matrix = DE::CMatrix::RotationMatrix(DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)));

	CPPUNIT_FAIL("Not implemented");
}

void MatrixTest::TestSetScale() {
	DE::CMatrix matrix = DE::CMatrix::ScaleMatrix(DE::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix._11 == 1 && matrix._12 == 0 && matrix._13 == 0 && matrix._14 == 0 &&
		matrix._21 == 0 && matrix._22 == 2 && matrix._23 == 0 && matrix._24 == 0 &&
		matrix._31 == 0 && matrix._32 == 0 && matrix._33 == 3 && matrix._34 == 0 &&
		matrix._41 == 0 && matrix._42 == 0 && matrix._43 == 0 && matrix._44 == 1);
}

void MatrixTest::TestGetScale() {
	DE::CMatrix matrix = DE::CMatrix::ScaleMatrix(DE::CVector3F(1, 2, 3));

	CPPUNIT_ASSERT(matrix == matrix.GetScale());
}

void MatrixTest::TestGetTranslation() {
	DE::CVector3F translation(10, 20, 30);
	DE::CMatrix matrix = DE::CMatrix::TranslationMatrix(translation);

	CPPUNIT_ASSERT(translation == matrix.GetTranslation());
}

void MatrixTest::TestGetRotation() {
	DE::CMatrix matrix = DE::CMatrix::RotationMatrix(DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)));

	CPPUNIT_ASSERT(matrix == matrix.GetRotation());
}

void MatrixTest::TestTransformVector3() {
	DE::CMatrix matrix(DE::CVector3F(1, 2, 3), DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)), DE::CVector3F(10, 20, 30));
	DE::CVector3F vector(-5, 87, 20);

	DE::CVector3F expected(matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43);

	CPPUNIT_ASSERT(matrix.Transform(vector) == expected);
	CPPUNIT_ASSERT(matrix.Transform(vector, true) == expected.Normalize());
}

void MatrixTest::TestTransformVector4() {
	DE::CMatrix matrix(DE::CVector3F(1, 2, 3), DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)), DE::CVector3F(10, 20, 30));
	DE::CVector4F vector(-5, 87, 20, 8);

	DE::CVector4F expected(matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w,
		matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w,
		matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w,
		matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w);

	CPPUNIT_ASSERT(matrix.Transform(vector) == expected);
	CPPUNIT_ASSERT(matrix.Transform(vector, true) == expected.Normalize());
}

void MatrixTest::TestDeterminant() {
	DE::CMatrix matrix(DE::CVector3F(1, 2, 3), DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)), DE::CVector3F(10, 20, 30));

	float determinant = matrix._11 * (matrix._22 * (matrix._33 * matrix._44 - matrix._43 * matrix._34) - matrix._32 * (matrix._23 * matrix._44 - matrix._43 * matrix._24) + matrix._42 * (matrix._23 * matrix._34 - matrix._33 * matrix._24)) -
		matrix._21 * (matrix._32 * (matrix._43 * matrix._14 - matrix._13 * matrix._44) - matrix._42 * (matrix._33 * matrix._14 - matrix._13 * matrix._34) + matrix._12 * (matrix._33 * matrix._44 - matrix._43 * matrix._34)) +
		matrix._31 * (matrix._42 * (matrix._13 * matrix._24 - matrix._23 * matrix._14) - matrix._12 * (matrix._43 * matrix._24 - matrix._23 * matrix._44) + matrix._22 * (matrix._43 * matrix._14 - matrix._13 * matrix._44)) -
		matrix._41 * (matrix._12 * (matrix._23 * matrix._34 - matrix._33 * matrix._24) - matrix._22 * (matrix._13 * matrix._34 - matrix._33 * matrix._14) + matrix._32 * (matrix._13 * matrix._24 - matrix._23 * matrix._14));

	CPPUNIT_ASSERT_EQUAL(determinant, matrix.Determinant());
}

void MatrixTest::TestTransformArray() {
	const int n = 10;
	DE::CMatrix matrix(DE::CVector3F(1, 2, 3), DE::CVector3F(DE::DegreeToRadian(10), DE::DegreeToRadian(20), DE::DegreeToRadian(30)), DE::CVector3F(10, 20, 30));
	DE::CVector3F vectors[n];
	DE::CVector3F vectors_out[n];
	for(int i = 0; i < n; ++i) {
		vectors[i] = DE::CVector3F(-5 * n, 87 * n, 20 * -n);
	}

	DE::CMatrix::TransformArray(vectors_out, vectors, matrix, n, false);
	for(int i = 0; i < n; ++i) {
		DE::CVector3F expected(matrix._11 * vectors[i].x + matrix._21 * vectors[i].y + matrix._31 * vectors[i].z + matrix._41,
			matrix._12 * vectors[i].x + matrix._22 * vectors[i].y + matrix._32 * vectors[i].z + matrix._42,
			matrix._13 * vectors[i].x + matrix._23 * vectors[i].y + matrix._33 * vectors[i].z + matrix._43);
		CPPUNIT_ASSERT(vectors_out[i] == expected);
	}

	DE::CMatrix::TransformArray(vectors_out, vectors, matrix, n, true);
	for(int i = 0; i < n; ++i) {
		DE::CVector3F expected(matrix._11 * vectors[i].x + matrix._21 * vectors[i].y + matrix._31 * vectors[i].z + matrix._41,
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
