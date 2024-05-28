#ifndef INTEGRATOR_FACTORY_HPP
#define INTEGRATOR_FACTORY_HPP

#include "celestial_obj.hpp"
#include "integrator.hpp"
#include "math_obj.hpp"

#include <string>


namespace orbsim {

class IntegratorFactory {

public:
	IntegratorFactory(CelestialObj cel_obj, Vec3 x0, Vec3 v0,
					  double t_start, double t_end, int t_steps);

	Integrator *create(std::string type);

private:
	CelestialObj cel_obj;
	Vec3 x0;
	Vec3 v0;
	double t_start;
	double t_end;
	double t_steps;
};

} // namespace orbsim


#endif	// INTEGRATOR_FACTORY_HPP
