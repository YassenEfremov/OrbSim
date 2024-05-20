#ifndef RK4_HPP
#define RK4_HPP

#include "simulation/integrators/integrator.hpp"
#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief RK4 (Runge-Kutta 4th order) integrator
 */
class RK4 : public Integrator {

public:
	RK4(double M, double R0, Vec3 x0, Vec3 v0,
		double t_i, double t_f, int steps);

	RK4 *copy() const override;

	void integrate() override;
};

} // namespace orbsim


#endif	// RK4_HPP
