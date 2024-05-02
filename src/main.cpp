#include "simulation/integrators/euler.hpp"
#include "simulation/integrators/verlet.hpp"
#include "simulation/integrators/rk4.hpp"
#include "simulation/math_obj.hpp"

#include <iostream>


int main() {

	// Initial conditions

	double M = 5.972e24;	// Earth Mass in [kg]
	double R0 = 6378.137;	// Earth Radius in [km]

	orbsim::Vec3 x0{7000, 0.000001, -0.001608};
	orbsim::Vec3 v0{0.000002, 1.310359, 7.431412};

	double t_i = 0;
	double t_f = 86400;

	// Integrate

	orbsim::Integrator *i = new orbsim::RK4(M, R0, x0, v0, t_i, t_f, t_f/10);

	i->integrate();
	// v.save_to_file("orbit.txt");

	delete i;

	return 0;
}
