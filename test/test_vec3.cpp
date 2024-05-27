#include "simulation/math_obj.hpp"

#include "gtest/gtest.h"

#include <cmath>


TEST(Vec3Test, Addition) {

	orbsim::Vec3 v1{1.5, 2.5, 3.1};
	orbsim::Vec3 v2{4.4, 5.5, 6.9};
	orbsim::Vec3 v3 = v1 + v2;

	EXPECT_DOUBLE_EQ(v3.x, v1.x + v2.x);
	EXPECT_DOUBLE_EQ(v3.y, v1.y + v2.y);
	EXPECT_DOUBLE_EQ(v3.z, v1.z + v2.z);
}

TEST(Vec3Test, Subtraction) {

	orbsim::Vec3 v1{10.5, 20.1, 30.7};
	orbsim::Vec3 v2{5.3, 10.1, 15.4};
	orbsim::Vec3 v3 = v1 - v2;

	EXPECT_DOUBLE_EQ(v3.x, v1.x - v2.x);
	EXPECT_DOUBLE_EQ(v3.y, v1.y - v2.y);
	EXPECT_DOUBLE_EQ(v3.z, v1.z - v2.z);
}

TEST(Vec3Test, Negation) {

	orbsim::Vec3 v1{5.5, 2.3, 3.1};
	orbsim::Vec3 v2 = - v1;

	EXPECT_DOUBLE_EQ(v2.x, - v1.x);
	EXPECT_DOUBLE_EQ(v2.y, - v1.y);
	EXPECT_DOUBLE_EQ(v2.z, - v1.z);
}

TEST(Vec3Test, Multiplication) {

	orbsim::Vec3 v1{2.5, 7.5, 10.1};
	orbsim::Vec3 v2 = 2 * v1;
	orbsim::Vec3 v3 = v1 * 2;

	EXPECT_DOUBLE_EQ(v2.x, 2 * v1.x);
	EXPECT_DOUBLE_EQ(v2.y, 2 * v1.y);
	EXPECT_DOUBLE_EQ(v2.z, 2 * v1.z);

	EXPECT_DOUBLE_EQ(v3.x, v1.x * 2);
	EXPECT_DOUBLE_EQ(v3.y, v1.y * 2);
	EXPECT_DOUBLE_EQ(v3.z, v1.z * 2);
}

TEST(Vec3Test, MultiplicationAndAssignment) {

	orbsim::Vec3 v1{2.5, 7.5, 10.1};
	v1 *= 2;

	EXPECT_DOUBLE_EQ(v1.x, 2 * 2.5);
	EXPECT_DOUBLE_EQ(v1.y, 2 * 7.5);
	EXPECT_DOUBLE_EQ(v1.z, 2 * 10.1);
}

TEST(Vec3Test, Division) {

	orbsim::Vec3 v1{5.2, 15.1, 10.5};
	orbsim::Vec3 v2 = v1 / 5;

	EXPECT_DOUBLE_EQ(v2.x, v1.x / 5);
	EXPECT_DOUBLE_EQ(v2.y, v1.y / 5);
	EXPECT_DOUBLE_EQ(v2.z, v1.z / 5);
}

TEST(Vec3Test, DivisionAndAssignment) {

	orbsim::Vec3 v1{5.2, 15.1, 10.5};
	v1 /= 5;

	EXPECT_DOUBLE_EQ(v1.x, 5.2 / 5);
	EXPECT_DOUBLE_EQ(v1.y, 15.1 / 5);
	EXPECT_DOUBLE_EQ(v1.z, 10.5 / 5);
}

TEST(Vec3Test, Length) {

	orbsim::Vec3 v1{1, 1, 1};

	EXPECT_DOUBLE_EQ(v1.len(), std::sqrt(3));
}

TEST(Vec3Test, Norm) {

	orbsim::Vec3 v1{2, 2, 2};
	orbsim::Vec3 v1_normed = v1.norm();

	EXPECT_DOUBLE_EQ(v1_normed.len(), 1);
	EXPECT_DOUBLE_EQ(v1_normed.x, 1 / std::sqrt(3));
	EXPECT_DOUBLE_EQ(v1_normed.y, 1 / std::sqrt(3));
	EXPECT_DOUBLE_EQ(v1_normed.z, 1 / std::sqrt(3));
}

TEST(Vec3Test, DotProduct) {

	orbsim::Vec3 v1{1.1, 2.6, 3.4};
	orbsim::Vec3 v2{4.3, 5.1, 6.5};

	EXPECT_DOUBLE_EQ(v1.dot(v2), v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

TEST(Vec3Test, CrossProduct) {

	orbsim::Vec3 v1{1.1, 2.6, 3.4};
	orbsim::Vec3 v2{4.3, 5.1, 6.5};
	orbsim::Vec3 v3 = v1.cross(v2);

	// this->y * other.z - this->z * other.y,
	// this->z * other.x - this->x * other.z,
	// this->x * other.y - this->y * other.x,
	EXPECT_DOUBLE_EQ(v3.x, v1.y*v2.z - v1.z*v2.y);
	EXPECT_DOUBLE_EQ(v3.y, v1.z*v2.x - v1.x*v2.z);
	EXPECT_DOUBLE_EQ(v3.z, v1.x*v2.y - v1.y*v2.x);
}

TEST(Vec3Test, ToString) {

	orbsim::Vec3 v1{1.75830921, 2.18274001, 3.11208863};

	EXPECT_STREQ(v1.to_str().c_str(), "[1.75830921  2.18274001  3.11208863]");
}
