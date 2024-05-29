#include "simulation/math_obj.hpp"

#include "gtest/gtest.h"

#include <cmath>


TEST(Vec3Test, Addition) {
	using orbsim::Vec3;

	Vec3 v1{1.5, 2.5, 3.1};
	Vec3 v2{4.4, 5.5, 6.9};
	Vec3 v3 = v1 + v2;

	EXPECT_DOUBLE_EQ(v3.x, 1.5 + 4.4);
	EXPECT_DOUBLE_EQ(v3.y, 2.5 + 5.5);
	EXPECT_DOUBLE_EQ(v3.z, 3.1 + 6.9);
}

TEST(Vec3Test, Subtraction) {
	using orbsim::Vec3;

	Vec3 v1{10.5, 20.1, 30.7};
	Vec3 v2{5.3, 10.1, 15.4};
	Vec3 v3 = v1 - v2;

	EXPECT_DOUBLE_EQ(v3.x, 10.5 - 5.3);
	EXPECT_DOUBLE_EQ(v3.y, 20.1 - 10.1);
	EXPECT_DOUBLE_EQ(v3.z, 30.7 - 15.4);
}

TEST(Vec3Test, Negation) {
	using orbsim::Vec3;

	Vec3 v1{5.5, 2.3, 3.1};
	Vec3 v2 = - v1;

	EXPECT_DOUBLE_EQ(v2.x, - 5.5);
	EXPECT_DOUBLE_EQ(v2.y, - 2.3);
	EXPECT_DOUBLE_EQ(v2.z, - 3.1);
}

TEST(Vec3Test, Multiplication) {
	using orbsim::Vec3;

	Vec3 v1{2.5, 7.5, 10.1};
	Vec3 v2 = 2 * v1;
	Vec3 v3 = v1 * 2;

	EXPECT_DOUBLE_EQ(v2.x, v3.x);
	EXPECT_DOUBLE_EQ(v2.y, v3.y);
	EXPECT_DOUBLE_EQ(v2.z, v3.z);
	EXPECT_DOUBLE_EQ(v2.x, 2 * 2.5);
	EXPECT_DOUBLE_EQ(v2.y, 2 * 7.5);
	EXPECT_DOUBLE_EQ(v2.z, 2 * 10.1);
}

TEST(Vec3Test, MultiplicationAndAssignment) {
	using orbsim::Vec3;

	Vec3 v1{2.5, 7.5, 10.1};
	v1 *= 2;

	EXPECT_DOUBLE_EQ(v1.x, 2 * 2.5);
	EXPECT_DOUBLE_EQ(v1.y, 2 * 7.5);
	EXPECT_DOUBLE_EQ(v1.z, 2 * 10.1);
}

TEST(Vec3Test, Division) {
	using orbsim::Vec3;

	Vec3 v1{5.2, 15.1, 10.5};
	Vec3 v2 = v1 / 5;

	EXPECT_DOUBLE_EQ(v2.x, 5.2 / 5);
	EXPECT_DOUBLE_EQ(v2.y, 15.1 / 5);
	EXPECT_DOUBLE_EQ(v2.z, 10.5 / 5);
}

TEST(Vec3Test, DivisionAndAssignment) {
	using orbsim::Vec3;

	Vec3 v1{5.2, 15.1, 10.5};
	v1 /= 5;

	EXPECT_DOUBLE_EQ(v1.x, 5.2 / 5);
	EXPECT_DOUBLE_EQ(v1.y, 15.1 / 5);
	EXPECT_DOUBLE_EQ(v1.z, 10.5 / 5);
}

TEST(Vec3Test, Comparison) {
	using orbsim::Vec3;

	Vec3 v1{5.2, 15.1, 10.5};
	Vec3 v2{5.3, -1.1, 10.0};

	EXPECT_TRUE(v1 == v1);
	EXPECT_TRUE(v1 != v2);
	EXPECT_FALSE(v1 == v2);
	EXPECT_FALSE(v2 != v2);
}

TEST(Vec3Test, Length) {
	using orbsim::Vec3;

	Vec3 v1{1, 1, 1};

	EXPECT_DOUBLE_EQ(v1.len(), std::sqrt(3));
}

TEST(Vec3Test, Norm) {
	using orbsim::Vec3;

	Vec3 v1{2, 2, 2};
	Vec3 v2 = v1.norm();

	EXPECT_DOUBLE_EQ(v2.len(), 1);
	EXPECT_DOUBLE_EQ(v2.x, 1 / std::sqrt(3));
	EXPECT_DOUBLE_EQ(v2.y, 1 / std::sqrt(3));
	EXPECT_DOUBLE_EQ(v2.z, 1 / std::sqrt(3));
}

TEST(Vec3Test, DotProduct) {
	using orbsim::Vec3;

	Vec3 v1{1.1, 2.6, 0.0};
	Vec3 v2{4.3, 5.1, 6.5};
	Vec3 v3{0.0, 0.0, 6.5};

	EXPECT_DOUBLE_EQ(v1.dot(v2), 1.1*4.3 + 2.6*5.1 + 0.0*6.5);
	EXPECT_DOUBLE_EQ(v1.dot(v3), 0.0);
	EXPECT_DOUBLE_EQ(v3.dot(v3), 6.5 * 6.5);
}

TEST(Vec3Test, CrossProduct) {
	using orbsim::Vec3;

	Vec3 v1{1.1, 2.6, 3.4};
	Vec3 v2{4.3, 5.1, 6.5};
	Vec3 v3 = v1.cross(v2);

	// Formula for cross product with coordinates:
	// x = this->y * other.z - this->z * other.y,
	// y = this->z * other.x - this->x * other.z,
	// z = this->x * other.y - this->y * other.x,
	EXPECT_DOUBLE_EQ(v3.x, 2.6*6.5 - 3.4*5.1);
	EXPECT_DOUBLE_EQ(v3.y, 3.4*4.3 - 1.1*6.5);
	EXPECT_DOUBLE_EQ(v3.z, 1.1*5.1 - 2.6*4.3);
}

TEST(Vec3Test, ToString) {
	using orbsim::Vec3;

	Vec3 v1{1.75830921, 2.18274001, 3.11208863};

	EXPECT_STREQ(v1.to_str().c_str(), "[1.75830921  2.18274001  3.11208863]");
}
