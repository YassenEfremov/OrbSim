#include "euler.hpp"
#include "simulation/integrators/integrator.hpp"
#include "simulation/math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Euler::Euler(double M, double R0, Vec3 x0, Vec3 v0,
			 double t_i, double t_f, int steps)
	: Integrator(M, R0, x0, v0, t_i, t_f, steps) {}

void Euler::integrate() {
	for (int i = 0; i < steps - 1; i++) {
		std::cout << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";

		this->pos_arr[i + 1] = this->pos_arr[i] + (this->vel_arr[i]) * delta_t;
		this->vel_arr[i + 1] = this->vel_arr[i] + (- (M * G * this->pos_arr[i]) / (std::pow(this->pos_arr[i].len(), 3))) * delta_t;

		// Convert back to kilometers
		this->pos_arr[i] /= 1000;
		this->vel_arr[i] /= 1000;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] /= 1000;
	this->vel_arr[this->steps - 1] /= 1000;
}

} // namespace orbsim
