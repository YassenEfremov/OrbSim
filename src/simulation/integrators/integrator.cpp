#include "integrator.hpp"
#include "diff_eq.hpp"
#include "math_obj.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>


namespace orbsim {

Integrator::Integrator(DESystem<Vec3> de_system, double M, double R0,
					   Vec3 x0, Vec3 v0,
					   double t_i, double t_f, int steps)
	: de_system(de_system), steps(steps), delta_t((t_f - t_i) / (steps - 1)),
	  time_arr(new double[steps]{}),
	  pos_arr(new Vec3[steps]{}), vel_arr(new Vec3[steps]{}) {
	
	if (t_i < 0) {
		throw std::domain_error("Start time must be a positive integer!");
	}
	if (t_f <= 0) {
		throw std::domain_error("End time must be a positive integer!");
	}
	if (t_i >= t_f) {
		throw std::domain_error("Start time must be smaller than end time!");
	}
	if (steps <= 0) {
		throw std::domain_error("Steps must be a positive integer!");
	}

	// this->time_arr = ?
	this->pos_arr[0] = x0;
	this->vel_arr[0] = v0;
	this->M = M;
	this->R0 = R0;

	// Norming constants for dimensionless units
	this->R_dim = this->R0;	// [km]
	this->V_dim = std::sqrt((this->M * G)/(1000*this->R0))/1000;	// [km/sec]
	this->T_dim = R_dim / V_dim;	// [sec]
}

Integrator::Integrator(const Integrator &other)
	: de_system(other.de_system), steps(other.steps), delta_t(other.delta_t),
	  time_arr(new double[other.steps]{}),
	  pos_arr(new Vec3[other.steps]{}), vel_arr(new Vec3[other.steps]{}) {

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
	std::swap(this->de_system, integ_copy->de_system);
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
double Integrator::get_delta_t() const { return this->delta_t; }
double *Integrator::get_time_arr() const { return this->time_arr; }
Vec3 *Integrator::get_pos_arr() const { return this->pos_arr; }
Vec3 *Integrator::get_vel_arr() const { return this->vel_arr; }

void Integrator::set_steps(int steps) {
	if (steps <= 0) {
		throw std::domain_error("Steps must be a positive integer!");
	}
	this->steps = steps;
}

void Integrator::set_delta_t(int t_start, int t_end) {
	if (t_start < 0) {
		throw std::domain_error("Start time must be a positive integer!");
	}
	if (t_start >= t_end) {
		throw std::domain_error("Start time must be smaller than end time!");
	}
	if (t_end <= t_start) {
		throw std::domain_error("End time must be larger than start time!");
	}
	this->delta_t = (t_end - t_start) / (this->steps - 1);
}

void Integrator::set_x0(Vec3 x0) {
	this->pos_arr[0] = x0;
}

void Integrator::set_v0(Vec3 v0) {
	this->vel_arr[0] = v0;
}

void Integrator::save_to_file(const char *filename) const
{
	std::ofstream of(filename);
	for (int i = 0; i < this->steps; i++) {
		of << pos_arr[i].to_str() << " " << vel_arr[i].to_str() << "\n";
	}
}

} // namespace orbsim
