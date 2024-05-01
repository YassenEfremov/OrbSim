#include "euler.hpp"
#include "simulation/math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Euler::Euler(double M, double R0, Vec3 x0, Vec3 v0,
			 double t_i, double t_f, int steps)
	: time_arr(new double[steps]{}), pos_arr(new Vec3[steps]{}),
									 vel_arr(new Vec3[steps]{}) {
	
	this->steps = steps;
	this->delta_t = (t_f - t_i) / (steps - 1);
	this->M = M;
	this->R0 = R0;
	// Convert to meters
	this->pos_arr[0] = x0 * 1000;
	this->vel_arr[0] = v0 * 1000;

}

Euler::~Euler() {
	delete[] this->time_arr;
	delete[] this->pos_arr;
	delete[] this->vel_arr;
}

void Euler::integrate() {
	for (int i = 0; i < steps - 1; i++) {
		std::cout << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";
		this->pos_arr[i + 1] = this->pos_arr[i] + (this->vel_arr[i]) * delta_t;
		this->vel_arr[i + 1] = this->vel_arr[i] - (M * G * this->pos_arr[i]) / (std::pow(this->pos_arr[i].len(), 3)) * delta_t;
	}
}

double *Euler::get_time_arr() const { return this->time_arr; }
Vec3 *Euler::get_pos_arr() const { return this->pos_arr; }
Vec3 *Euler::get_vel_arr() const { return this->vel_arr; }

void Euler::save_to_file(const char *filename) const {
	std::ofstream of(filename);
	for (int i = 0; i < this->steps; i++) {
		of << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";
	}
}

} // namespace orbsim
