#ifndef INTEGRATOR_FACTORY_HPP
#define INTEGRATOR_FACTORY_HPP

#include "simulation/celestial_obj.hpp"
#include "simulation/integrators/integrator.hpp"
#include "simulation/diff_eq.hpp"
#include "simulation/math_obj.hpp"

#include <string>


namespace orbsim {

class IntegratorFactory {

public:
	IntegratorFactory(DESystem<Vec3> de_system, CelestialObj cel_obj,
					  Vec3 x0, Vec3 v0,
					  double t_start, double t_end, int t_steps);

	Integrator *create(std::string type);

private:
	DESystem<Vec3> de_system;
	CelestialObj cel_obj;
	Vec3 x0;
	Vec3 v0;
	double t_start;
	double t_end;
	double t_steps;
};

} // namespace orbsim


#endif	// INTEGRATOR_FACTORY_HPP
