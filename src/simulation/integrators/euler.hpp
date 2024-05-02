#ifndef EULER_HPP
#define EULER_HPP

#include "simulation/integrators/integrator.hpp"
#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Euler integrator
 */
class Euler : public Integrator {

public:
	Euler(double M, double R0, Vec3 x0, Vec3 v0,
		  double t_i, double t_f, int steps);

	void integrate() override;
};

} // namespace orbsim


#endif	// EULER_HPP
