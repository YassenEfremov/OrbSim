#include "rk4.hpp"
#include "simulation/integrators/integrator.hpp"
#include "simulation/math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

RK4::RK4(double M, double R0, Vec3 x0, Vec3 v0,
			   double t_i, double t_f, int steps)
	: Integrator(M, R0, x0, v0, t_i, t_f, steps) {}

void RK4::integrate() {

	struct {
		Vec3 pos;
		Vec3 vel;
	} rk_slopes[4];

	Vec3 pos_temp;
	Vec3 vel_temp;

	for (int i = 0; i < steps - 1; i++) {
		rk_slopes[0].pos = vel_arr[i];
		rk_slopes[0].vel = - (M * G * this->pos_arr[i]) / (std::pow(this->pos_arr[i].len(), 3));

		pos_temp = this->pos_arr[i] + rk_slopes[0].pos * (delta_t/2);
		vel_temp = this->vel_arr[i] + rk_slopes[0].vel * (delta_t/2);

		rk_slopes[1].pos = vel_temp;
		rk_slopes[1].vel = - (M * G * pos_temp) / (std::pow(pos_temp.len(), 3));

		pos_temp = this->pos_arr[i] + rk_slopes[1].pos * (delta_t/2);
		vel_temp = this->vel_arr[i] + rk_slopes[1].vel * (delta_t/2);

		rk_slopes[2].pos = vel_temp;
		rk_slopes[2].vel = - (M * G * pos_temp) / (std::pow(pos_temp.len(), 3));

		pos_temp = this->pos_arr[i] + rk_slopes[2].pos * delta_t;
		vel_temp = this->vel_arr[i] + rk_slopes[2].vel * delta_t;

		rk_slopes[3].pos = vel_temp;
		rk_slopes[3].vel = - (M * G * pos_temp) / (std::pow(pos_temp.len(), 3));

		// Final next step estimation
		this->pos_arr[i + 1] = pos_arr[i] + (rk_slopes[0].pos + 2*rk_slopes[1].pos + 2*rk_slopes[2].pos + rk_slopes[3].pos)/6 * delta_t;
		this->vel_arr[i + 1] = vel_arr[i] + (rk_slopes[0].vel + 2*rk_slopes[1].vel + 2*rk_slopes[2].vel + rk_slopes[3].vel)/6 * delta_t;

		// Convert back to kilometers
		this->pos_arr[i] /= 1000;
		this->vel_arr[i] /= 1000;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] /= 1000;
	this->vel_arr[this->steps - 1] /= 1000;
}

} // namespace orbsim
