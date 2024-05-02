#include "verlet.hpp"
#include "simulation/math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Verlet::Verlet(double M, double R0, Vec3 x0, Vec3 v0,
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

Verlet::~Verlet() {
	delete[] this->time_arr;
	delete[] this->pos_arr;
	delete[] this->vel_arr;
}

void Verlet::integrate() {
	for (int i = 0; i < steps - 1; i++) {
		std::cout << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";

		Vec3 vel_half = this->vel_arr[i] + (- (M * G * this->pos_arr[i]) / (std::pow(this->pos_arr[i].len(), 3))) * delta_t/2;
		this->pos_arr[i + 1] = this->pos_arr[i] + (vel_half) * delta_t;
		this->vel_arr[i + 1] = vel_half + (- (M * G * this->pos_arr[i + 1]) / (std::pow(this->pos_arr[i + 1].len(), 3))) * delta_t/2;

		// Convert back to kilometers
		this->pos_arr[i] /= 1000;
		this->vel_arr[i] /= 1000;
	}

	// Don't forget the last one
	this->pos_arr[this->steps - 1] /= 1000;
	this->vel_arr[this->steps - 1] /= 1000;
}

double *Verlet::get_time_arr() const { return this->time_arr; }
Vec3 *Verlet::get_pos_arr() const { return this->pos_arr; }
Vec3 *Verlet::get_vel_arr() const { return this->vel_arr; }

void Verlet::save_to_file(const char *filename) const {
	std::ofstream of(filename);
	for (int i = 0; i < this->steps; i++) {
		of << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";
	}
}

} // namespace orbsim
