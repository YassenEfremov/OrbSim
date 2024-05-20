#include "integrator.hpp"
#include "math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>


namespace orbsim {

Integrator::Integrator(double M, double R0, Vec3 x0, Vec3 v0,
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

Integrator::Integrator(const Integrator &other)
	: time_arr(new double[other.steps]{}),
	  pos_arr(new Vec3[other.steps]{}),
	  vel_arr(new Vec3[other.steps]{}) {
	
	this->steps = other.steps;
	this->delta_t = other.delta_t;
	this->M = other.M;
	this->R0 = other.R0;
	for (int i = 0; i < other.steps; i++) {
		this->time_arr[i] = other.time_arr[i];
		this->pos_arr[i] = other.pos_arr[i];
		this->vel_arr[i] = other.vel_arr[i];
	}
}

Integrator &Integrator::operator=(const Integrator &other) {
	Integrator *integ_copy = other.copy();
	std::swap(this->M, integ_copy->M);
	std::swap(this->R0, integ_copy->R0);
	std::swap(this->steps, integ_copy->steps);
	std::swap(this->delta_t, integ_copy->delta_t);
	std::swap(this->time_arr, integ_copy->time_arr);
	std::swap(this->pos_arr, integ_copy->pos_arr);
	std::swap(this->vel_arr, integ_copy->vel_arr);

	return *this;
}

Integrator::~Integrator() {
	delete[] this->time_arr;
	delete[] this->pos_arr;
	delete[] this->vel_arr;
}

int Integrator::get_steps() const { return this->steps; }
double *Integrator::get_time_arr() const { return this->time_arr; }
Vec3 *Integrator::get_pos_arr() const { return this->pos_arr; }
Vec3 *Integrator::get_vel_arr() const { return this->vel_arr; }

void Integrator::set_delta_t(int t_start, int t_end) {
	this->delta_t = (t_end - t_start) / (this->steps - 1);
}

void Integrator::set_steps(int steps) {
	this->steps = steps;
}

void Integrator::save_to_file(const char *filename) const
{
	std::ofstream of(filename);
	for (int i = 0; i < this->steps; i++) {
		of << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";
	}
}

} // namespace orbsim
