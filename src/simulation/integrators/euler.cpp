#include "euler.hpp"
#include "integrator.hpp"
#include "diff_eq.hpp"
#include "math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Euler::Euler(DESystem<Vec3> de_system, double M, double R0, Vec3 x0, Vec3 v0,
			 double t_i, double t_f, int steps)
	: Integrator(de_system, M, R0, x0, v0, t_i, t_f, steps) {}

Euler *Euler::copy() const { return new Euler(*this); }

void Euler::integrate() {
	// Norm the initial conditions
	// this->time_arr = linspace?
	this->delta_t /= this->T_dim;
	this->pos_arr[0] /= this->R_dim;
	this->vel_arr[0] /= this->V_dim;

	for (int i = 0; i < steps - 1; i++) {
		this->pos_arr[i + 1] = this->pos_arr[i] + this->de_system.get_rhs(1)(this->vel_arr[i]) * delta_t;
		this->vel_arr[i + 1] = this->vel_arr[i] + this->de_system.get_rhs(2)(this->pos_arr[i]) * delta_t;

		// Convert back to kilometers
		this->pos_arr[i] *= this->R_dim;
		this->vel_arr[i] *= this->V_dim;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] *= this->R_dim;
	this->vel_arr[this->steps - 1] *= this->V_dim;

	this->delta_t *= T_dim;
}

} // namespace orbsim
