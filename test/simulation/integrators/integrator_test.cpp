#include "simulation/integrators/integrator.hpp"
#include "simulation/integrators/euler.hpp"
#include "simulation/integrators/verlet.hpp"
#include "simulation/integrators/rk4.hpp"
#include "simulation/celestial_obj.hpp"
#include "simulation/math_obj.hpp"

#include "gtest/gtest.h"

#include <stdexcept>


template <typename I>
class IntegratorTest : public testing::Test {
public:
	IntegratorTest()
		: integ(orbsim::orbit_de, orbsim::Earth.mass, orbsim::Earth.radius,
				orbsim::Vec3{7000,0,0}, orbsim::Vec3{0,5.1,7.3}, 0, 1000, 100) {}

	I integ;
};

using IntegTypes = testing::Types<orbsim::Euler, orbsim::Verlet, orbsim::RK4>;
TYPED_TEST_SUITE(IntegratorTest, IntegTypes);


TYPED_TEST(IntegratorTest, Constructor) {
	using namespace orbsim;
	
	EXPECT_EQ(this->integ.get_steps(), 100);
	EXPECT_EQ(this->integ.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(this->integ.get_vel_arr()[0], (Vec3{0,5.1,7.3}));
}

TYPED_TEST(IntegratorTest, CopyConstructor) {
	using namespace orbsim;
	
	TypeParam new_integ1(this->integ);

	EXPECT_EQ(new_integ1.get_steps(), 100);
	EXPECT_EQ(new_integ1.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(new_integ1.get_vel_arr()[0], (Vec3{0,5.1,7.3}));


	TypeParam *new_integ2 = new TypeParam(this->integ);
	delete new_integ2;

	EXPECT_EQ(this->integ.get_steps(), 100);
	EXPECT_EQ(this->integ.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(this->integ.get_vel_arr()[0], (Vec3{0,5.1,7.3}));
}

TYPED_TEST(IntegratorTest, Assignment) {
	using namespace orbsim;
	
	TypeParam new_integ1(orbit_de, Earth.mass, Earth.radius, Vec3{7300,0,100}, Vec3{0,5,7}, 100, 2000, 1500);
	new_integ1 = this->integ;

	EXPECT_EQ(new_integ1.get_steps(), 100);
	EXPECT_EQ(new_integ1.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(new_integ1.get_vel_arr()[0], (Vec3{0,5.1,7.3}));


	TypeParam *new_integ2 = new TypeParam(orbit_de, Earth.mass, Earth.radius, Vec3{7300,0,100}, Vec3{0,5,7}, 100, 2000, 1500);
	*new_integ2 = this->integ;
	delete new_integ2;

	EXPECT_EQ(this->integ.get_steps(), 100);
	EXPECT_EQ(this->integ.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(this->integ.get_vel_arr()[0], (Vec3{0,5.1,7.3}));
}

TYPED_TEST(IntegratorTest, VirtualCopyConstructor) {
	using namespace orbsim;

	Integrator *our_integ = &this->integ;
	Integrator *new_integ = our_integ->copy();

	EXPECT_NE(dynamic_cast<TypeParam *>(new_integ), nullptr);

	delete new_integ;

	EXPECT_EQ(this->integ.get_steps(), 100);
	EXPECT_EQ(this->integ.get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(this->integ.get_vel_arr()[0], (Vec3{0,5.1,7.3}));
}

TYPED_TEST(IntegratorTest, StepsSetter) {
	using namespace orbsim;

	this->integ.set_steps(10);

	EXPECT_EQ(this->integ.get_steps(), 10);
	EXPECT_THROW(this->integ.set_steps(-2), std::domain_error);
	EXPECT_THROW(this->integ.set_steps(0), std::domain_error);
}

TYPED_TEST(IntegratorTest, DeltaTSetter) {
	using namespace orbsim;

	this->integ.set_delta_t(10, 200);

	EXPECT_DOUBLE_EQ(this->integ.get_delta_t(), (200 - 10) / this->integ.get_steps());
}

TYPED_TEST(IntegratorTest, InitialPositionSetter) {
	using namespace orbsim;

	this->integ.set_x0(Vec3{7500,0,100});

	EXPECT_EQ(this->integ.get_pos_arr()[0], (Vec3{7500,0,100}));
}

TYPED_TEST(IntegratorTest, InitialVelocitySetter) {
	using namespace orbsim;

	this->integ.set_v0(Vec3{0,-3.9,2.5});

	EXPECT_EQ(this->integ.get_vel_arr()[0], (Vec3{0,-3.9,2.5}));
}
