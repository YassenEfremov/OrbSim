#include "satellite.hpp"

#include "math_obj.hpp"

#include <cmath>

#include <algorithm>
#include <iostream>
#include <iomanip>


namespace orbsim {

Satellite::Satellite(Vec3 x0, Vec3 v0) : pos(x0), vel(v0) {

	Vec3 r = this->pos;
	Vec3 v = this->vel;

	// Cartesian state vectors -> Keplerian orbital elements
	// Steps are described here: https://downloads.rene-schwarz.com/download/M002-Cartesian_State_Vectors_to_Keplerian_Orbit_Elements.pdf

	// 1) Prep.

	// a) Orbital momentum vector
	Vec3 h = r.cross(v);

	// b) Ecc. vector and grav. parameter
	double mu = G * M_Earth;
	Vec3 e = (v.cross(h) / mu) - r.norm();

	// c) Vector pointing towards asc. node and true_anom
	Vec3 n{-h.y, h.x, 0};
	this->true_anom = r.dot(v) >= 0 ?
		std::acos(std::clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0))
		:
		2*PI - std::acos(std::clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0));
	
	// 2) Inc.

	this->inc = std::acos(h.z / h.len());

	// 3) Ecc. and ecc. anomaly (second is not currently needed)

	this->ecc = e.len();
	// double E = 2*std::atan(std::tan(this->true_anom/2) / std::sqrt((1 + this->ecc) / (1 - this->ecc)));

	// 4) Longitude/Right asc. of asc. node and arg. of periapsis

	this->ri_asc_node = n.y >= 0 ?
		std::acos(std::clamp(n.x / n.len(), -1.0, 1.0))
		:
		2*PI - std::acos(std::clamp(n.x / n.len(), -1.0, 1.0));
	
	this->arg_of_per = e.z >= 0 ?
		std::acos(std::clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0))
		:
		2*PI - std::acos(std::clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0));
	
	// 5) Mean anomaly (Not currently needed)

	// double M = E - this->ecc * std::sin(E);

	// 6) Semi-major axis

	this->sem_maj_ax = 1 / (2/r.len() - (v.len() * v.len())/mu);

	std::cout << std::setprecision(10)
		<< "ecc: " << this->ecc << "\n"
		<< "sem_maj_ax: " << this->sem_maj_ax << "\n"
		<< "inc: " << this->inc << "\n"
		<< "ri_asc_node: " << this->ri_asc_node << "\n"
		<< "arg_of_per: " << this->arg_of_per << "\n"
		<< "true_anom: " << this->true_anom << "\n";

}

Satellite::Satellite(double ecc, double sem_maj_ax,
					 double inc, double ri_asc_node,
					 double arg_of_per, double true_anom)
	: ecc(ecc), sem_maj_ax(sem_maj_ax),
	  inc(inc), ri_asc_node(ri_asc_node),
	  arg_of_per(arg_of_per), true_anom(true_anom) {

	// Keplerian orbital elements -> Cartesian state vectors

	// 1)
}

Vec3 Satellite::get_pos() const { return this->pos; }
Vec3 Satellite::get_vel() const { return this->vel; }

} // namespace orbsim
