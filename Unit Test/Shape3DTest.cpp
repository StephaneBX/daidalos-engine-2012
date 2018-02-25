#include "Shape3DTest.hpp"
#include <Scene/3D/CSphere.hpp>
#include <Scene/3D/CAABoundingBox.hpp>
#include <Scene/3D/CEllipse.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(Shape3DTest);

void Shape3DTest::TestCollisionBoundingBoxSphere() {
	
	daidalosengine::CAABoundingBoxF boundingBox(daidalosengine::CVector3F(0,0,0), daidalosengine::CVector3F(10,10,10));
	daidalosengine::CSphereF sphere(daidalosengine::CVector3F(0, 0, 0), 2.0F);
	
	// No collision
	sphere.SetPosition(daidalosengine::CVector3F(0,8.0F,0));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(false, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(0, -2.0F, 0));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(false, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(-8, 0.0F, 0));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(false, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(8, 0.0F, 0));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(false, sphere.IsCollision(&boundingBox));

	// Collision
	boundingBox.SetPosition(daidalosengine::CVector3F(4, 5, 6));
	sphere.SetPosition(daidalosengine::CVector3F(2.5F, 3, 0));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(true, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(-2, 6.5F, 12));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(true, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(4, 5, 6));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(true, sphere.IsCollision(&boundingBox));

	sphere.SetPosition(daidalosengine::CVector3F(-2, -1, 0));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&sphere));
	CPPUNIT_ASSERT_EQUAL(true, sphere.IsCollision(&boundingBox));
}

void Shape3DTest::TestCollisionBoundingBoxEllipse() {

	daidalosengine::CAABoundingBoxF boundingBox(daidalosengine::CVector3F(2, 2, 2), daidalosengine::CVector3F(10, 20, 10));
	daidalosengine::CEllipseF ellipse(daidalosengine::CVector3F(0, 0, 0), daidalosengine::CVector2F(2.0F, 3.0F));

	// No collision
	ellipse.SetPosition(daidalosengine::CVector3F(-4, 13, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(8, 13, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(8, 13, -4));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(8, -9, -4));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(-4, -9, -4));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(-4, -9, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(false, ellipse.IsCollision(&boundingBox));

	// Collision
	ellipse.SetPosition(daidalosengine::CVector3F(-2, 11, 6));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(6, 11, 6));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(6, 11, -2));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(6, -7, -2));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(-2, -7, -2));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));

	ellipse.SetPosition(daidalosengine::CVector3F(-2, -7, 6));
	CPPUNIT_ASSERT_EQUAL(true, boundingBox.IsCollision(&ellipse));
	CPPUNIT_ASSERT_EQUAL(true, ellipse.IsCollision(&boundingBox));
}

void Shape3DTest::TestCollisionBoundingBoxBoundingBox() {
	daidalosengine::CAABoundingBoxF boundingBox1(daidalosengine::CVector3F(-2, -2, -2), daidalosengine::CVector3F(10, 20, 6));
	daidalosengine::CAABoundingBoxF boundingBox2(daidalosengine::CVector3F(0, 0, 0), daidalosengine::CVector3F(10, 10, 10));

	// No collision
	boundingBox2.SetPosition(daidalosengine::CVector3F(-13, 0, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));

	boundingBox2.SetPosition(daidalosengine::CVector3F(10, 6, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));

	boundingBox2.SetPosition(daidalosengine::CVector3F(9, 20, 30));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));

	boundingBox2.SetPosition(daidalosengine::CVector3F(9, -19, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));

	boundingBox2.SetPosition(daidalosengine::CVector3F(9, 6, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));

	boundingBox2.SetPosition(daidalosengine::CVector3F(-13, 13, 8));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox1.IsCollision(&boundingBox2));
	CPPUNIT_ASSERT_EQUAL(false, boundingBox2.IsCollision(&boundingBox1));
}
