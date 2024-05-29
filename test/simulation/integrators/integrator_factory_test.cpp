#include "simulation/integrators/integrator_factory.hpp"
#include "simulation/integrators/integrator.hpp"
#include "simulation/integrators/euler.hpp"
#include "simulation/integrators/verlet.hpp"
#include "simulation/integrators/rk4.hpp"
#include "simulation/celestial_obj.hpp"

#include "gtest/gtest.h"


TEST(IntegratorFactoryTest, Euler) {
	using namespace orbsim;

	IntegratorFactory integ_factory(Earth, Vec3{7000,0,0}, {0,0,0}, 0, 1000, 100);
	Integrator *integ = integ_factory.create("Euler");

	EXPECT_NE(dynamic_cast<Euler *>(integ), nullptr);
	EXPECT_EQ(integ->get_steps(), 100);
	EXPECT_EQ(integ->get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(integ->get_vel_arr()[0], (Vec3{0,0,0}));

	delete integ;
}

TEST(IntegratorFactoryTest, Verlet) {
	using namespace orbsim;

	IntegratorFactory integ_factory(Earth, Vec3{7000,0,0}, {0,0,0}, 0, 1000, 100);
	Integrator *integ = integ_factory.create("Verlet");

	EXPECT_NE(dynamic_cast<Verlet *>(integ), nullptr);
	EXPECT_EQ(integ->get_steps(), 100);
	EXPECT_EQ(integ->get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(integ->get_vel_arr()[0], (Vec3{0,0,0}));

	delete integ;
}

TEST(IntegratorFactoryTest, RK4) {
	using namespace orbsim;

	IntegratorFactory integ_factory(Earth, Vec3{7000,0,0}, {0,0,0}, 0, 1000, 100);
	Integrator *integ = integ_factory.create("RK4");
	
	EXPECT_NE(dynamic_cast<RK4 *>(integ), nullptr);
	EXPECT_EQ(integ->get_steps(), 100);
	EXPECT_EQ(integ->get_pos_arr()[0], (Vec3{7000,0,0}));
	EXPECT_EQ(integ->get_vel_arr()[0], (Vec3{0,0,0}));

	delete integ;
}
