#ifndef EULER_HPP
#define EULER_HPP

#include "simulation/integrators/integrator.hpp"
#include "simulation/diff_eq.hpp"
#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Euler integrator
 */
class Euler : public Integrator {

public:
	Euler(DESystem<Vec3> de_system, double M, double R0, Vec3 x0, Vec3 v0,
		  double t_i, double t_f, int steps);

	Euler *copy() const override;

	void integrate() override;
};

} // namespace orbsim


#endif	// EULER_HPP
