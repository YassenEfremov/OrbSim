#ifndef VERLET_HPP
#define VERLET_HPP

#include "simulation/integrators/integrator.hpp"
#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Verlet integrator
 */
class Verlet : public Integrator {

public:
	Verlet(double M, double R0, Vec3 x0, Vec3 v0,
		   double t_i, double t_f, int steps);

	Verlet *copy() const override;

	void integrate() override;
};

} // namespace orbsim


#endif	// VERLET_HPP
