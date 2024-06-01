#include "rk4.hpp"
#include "integrator.hpp"
#include "diff_eq.hpp"
#include "math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

RK4::RK4(DESystem<Vec3> de_system, double M, double R0, Vec3 x0, Vec3 v0,
		 double t_i, double t_f, int steps)
	: Integrator(de_system, M, R0, x0, v0, t_i, t_f, steps) {}

RK4 *RK4::copy() const { return new RK4(*this); }

void RK4::integrate() {
	// Norm the initial conditions
	// this->time_arr = linspace?
	this->delta_t /= this->T_dim;
	this->pos_arr[0] /= this->R_dim;
	this->vel_arr[0] /= this->V_dim;

	struct {
		Vec3 pos;
		Vec3 vel;
	} rk_slopes[4];

	Vec3 pos_temp;
	Vec3 vel_temp;

	for (int i = 0; i < steps - 1; i++) {
		rk_slopes[0].pos = this->de_system.get_rhs(1)(this->vel_arr[i]);
		rk_slopes[0].vel = this->de_system.get_rhs(2)(this->pos_arr[i]);

		pos_temp = this->pos_arr[i] + rk_slopes[0].pos * (delta_t/2);
		vel_temp = this->vel_arr[i] + rk_slopes[0].vel * (delta_t/2);

		rk_slopes[1].pos = this->de_system.get_rhs(1)(vel_temp);
		rk_slopes[1].vel = this->de_system.get_rhs(2)(pos_temp);

		pos_temp = this->pos_arr[i] + rk_slopes[1].pos * (delta_t/2);
		vel_temp = this->vel_arr[i] + rk_slopes[1].vel * (delta_t/2);

		rk_slopes[2].pos = this->de_system.get_rhs(1)(vel_temp);
		rk_slopes[2].vel = this->de_system.get_rhs(2)(pos_temp);

		pos_temp = this->pos_arr[i] + rk_slopes[2].pos * delta_t;
		vel_temp = this->vel_arr[i] + rk_slopes[2].vel * delta_t;

		rk_slopes[3].pos = this->de_system.get_rhs(1)(vel_temp);
		rk_slopes[3].vel = this->de_system.get_rhs(2)(pos_temp);

		// Final next step estimation
		this->pos_arr[i + 1] = pos_arr[i] + (rk_slopes[0].pos + 2*rk_slopes[1].pos + 2*rk_slopes[2].pos + rk_slopes[3].pos)/6 * delta_t;
		this->vel_arr[i + 1] = vel_arr[i] + (rk_slopes[0].vel + 2*rk_slopes[1].vel + 2*rk_slopes[2].vel + rk_slopes[3].vel)/6 * delta_t;

		// Convert back to kilometers
		this->pos_arr[i] *= R_dim;
		this->vel_arr[i] *= V_dim;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] *= R_dim;
	this->vel_arr[this->steps - 1] *= V_dim;

	this->delta_t *= this->T_dim;
}

} // namespace orbsim
