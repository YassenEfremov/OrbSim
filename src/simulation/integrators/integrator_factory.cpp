#include "integrator_factory.hpp"

#include "integrator.hpp"
#include "euler.hpp"
#include "verlet.hpp"
#include "rk4.hpp"
#include "math_obj.hpp"
#include "diff_eq.hpp"

#include <string>
#include <stdexcept>


namespace orbsim {

IntegratorFactory::IntegratorFactory(DESystem<Vec3> de_system, CelestialObj cel_obj,
									 Vec3 x0, Vec3 v0,
									 double t_start, double t_end, int t_steps)
	: de_system(de_system), cel_obj(cel_obj), x0(x0), v0(v0),
	  t_start(t_start), t_end(t_end), t_steps(t_steps) {}

Integrator *orbsim::IntegratorFactory::create(std::string type) {
	if (type == "Euler") {
		return new Euler(de_system, cel_obj.mass, cel_obj.radius, x0, v0, t_start, t_end, t_steps);
	} else if (type == "Verlet") {
		return new Verlet(de_system, cel_obj.mass, cel_obj.radius, x0, v0, t_start, t_end, t_steps);
	} else if (type == "RK4") {
		return new RK4(de_system, cel_obj.mass, cel_obj.radius, x0, v0, t_start, t_end, t_steps);
	} else {
		throw std::domain_error("Invalid integrator! Should be one of: Euler, Verlet and RK4");
	}
}

} // namespace orbsim
