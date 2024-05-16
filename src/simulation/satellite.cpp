#include "satellite.hpp"

#include "math_obj.hpp"

#include <cmath>

#include <algorithm>
#include <iostream>
#include <iomanip>


namespace orbsim {

Satellite::Satellite(Vec3 x0, Vec3 v0) : pos(x0), vel(v0) {

	using std::acos;
	using std::clamp;
	using std::atan;
	using std::tan;
	using std::sqrt;
	Vec3 r = this->pos * 1000;	// Convert to meters
	Vec3 v = this->vel * 1000;	// Convert to meters

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
		acos(clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0))
		:
		2*PI - acos(clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0));
	
	// 2) Inc.

	this->inc = acos(h.z / h.len());

	// 3) Ecc. and ecc. anomaly (second is not currently needed)

	this->ecc = e.len();
	// double E = 2*atan(tan(this->true_anom/2) / sqrt((1 + this->ecc) / (1 - this->ecc)));

	// 4) Longitude/Right asc. of asc. node and arg. of periapsis

	this->ri_asc_node = n.y >= 0 ?
		acos(clamp(n.x / n.len(), -1.0, 1.0))
		:
		2*PI - acos(clamp(n.x / n.len(), -1.0, 1.0));
	
	this->arg_of_per = e.z >= 0 ?
		acos(clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0))
		:
		2*PI - acos(clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0));
	
	// 5) Mean anomaly (Not currently needed)

	// double M = E - this->ecc * std::sin(E);

	// 6) Semi-major axis

	this->sem_maj_ax = 1 / (2/r.len() - (v.len() * v.len())/mu);
	this->sem_maj_ax /= 1000;	// Convert back to kilometers


	std::cout << std::setprecision(15)
		<< "KEPLERIAN:\n"
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
	
	using std::sin;
	using std::cos;
	using std::sqrt;
	using std::atan2;
	double e = this->ecc;
	double a = this->sem_maj_ax * 1000;	// Convert to meters
	double i = this->inc;
	double OM = this->ri_asc_node;
	double w = this->arg_of_per;
	double ni = this->true_anom;

	// Keplerian orbital elements -> Cartesian state vectors
	// Steps are described here: https://downloads.rene-schwarz.com/download/M001-Keplerian_Orbit_Elements_to_Cartesian_State_Vectors.pdf

	// 1) Mean anomaly (SKIP - don't need it)

	// 2) Solve Kepler's equation for the ecc. anom

	double E = 2 * atan2(sqrt(1 - e) * sin(ni/2), sqrt(1 + e) * cos(ni/2));

	// 3) Obtain true anom.	(SKIP - already have it)

	// 4) Get distance to central body

	double r = a * (1 - e * cos(E));

	// 5) Obtain pos and vel vectors in orbital frame

	Vec3 pos_o = r * Vec3{
		cos(ni),
		sin(ni),
		0
	};

	double mu = G * M_Earth;
	Vec3 vel_o = (sqrt(mu * a) / r) * Vec3{
		- sin(E),
		sqrt(1 - e*e) * cos(E),
		0
	};

	// 6) Transform to inertial frame

	auto transform = [=](Vec3 o) {
		return Vec3{
			o.x * (cos(w)*cos(OM) - sin(w)*cos(i)*sin(OM)) - o.y * (sin(w)*cos(OM) + cos(w)*cos(i)*sin(OM)),
			o.x * (cos(w)*sin(OM) + sin(w)*cos(i)*cos(OM)) + o.y * (cos(w)*cos(i)*cos(OM) - sin(w)*sin(OM)),
			o.x * sin(w)*sin(i) + o.y * cos(w)*sin(i)
		};
	};

	this->pos = transform(pos_o) / 1000;
	this->vel = transform(vel_o) / 1000;


	std::cout << std::setprecision(10)
		<< "CARTESIAN:\n"
		<< "pos: " << this->pos.to_str() << "\n"
		<< "vel: " << this->vel.to_str() << "\n";
}

Vec3 Satellite::get_pos() const { return this->pos; }
Vec3 Satellite::get_vel() const { return this->vel; }

} // namespace orbsim
