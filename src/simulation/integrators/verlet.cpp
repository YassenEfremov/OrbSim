#include "verlet.hpp"
#include "integrator.hpp"
#include "math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Verlet::Verlet(double M, double R0, Vec3 x0, Vec3 v0,
			   double t_i, double t_f, int steps)
	: Integrator(M, R0, x0, v0, t_i, t_f, steps) {}

Verlet *Verlet::copy() const { return new Verlet(*this); }

void Verlet::integrate() {
	for (int i = 0; i < steps - 1; i++) {
		Vec3 vel_half = this->vel_arr[i] + (- (M * G * this->pos_arr[i]) / (std::pow(this->pos_arr[i].len(), 3))) * (delta_t/2);
		this->pos_arr[i + 1] = this->pos_arr[i] + (vel_half) * delta_t;
		this->vel_arr[i + 1] = vel_half + (- (M * G * this->pos_arr[i + 1]) / (std::pow(this->pos_arr[i + 1].len(), 3))) * (delta_t/2);

		// Convert back to kilometers
		this->pos_arr[i] /= 1000;
		this->vel_arr[i] /= 1000;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] /= 1000;
	this->vel_arr[this->steps - 1] /= 1000;
}

} // namespace orbsim
