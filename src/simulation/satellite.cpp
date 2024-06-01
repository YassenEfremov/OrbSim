#include "satellite.hpp"

#include "integrators/integrator_factory.hpp"
#include "integrators/euler.hpp"
#include "integrators/verlet.hpp"
#include "integrators/rk4.hpp"
#include "celestial_obj.hpp"
#include "diff_eq.hpp"
#include "math_obj.hpp"

#include <cmath>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>


namespace orbsim {

Satellite::Satellite(CartElem cart_elem,
					 std::string integ_name, CelestialObj cel_obj,
					 double t_start, double t_end, int t_steps)
	: cart_elem(cart_elem), integ_name(integ_name), cel_obj(cel_obj),
	  t_start(t_start), t_end(t_end), t_steps(t_steps) {

	std::set valid_integ {"Euler", "Verlet", "RK4"};
	if (valid_integ.find(integ_name.c_str()) != valid_integ.end()) {
		throw std::domain_error("Invalid integrator! Should be one of: Euler, Verlet and RK4");
	}

	calc_kepl();

	IntegratorFactory integ_fact(orbit_de, cel_obj, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps);
	this->integ = integ_fact.create(integ_name);
}

Satellite::Satellite(KeplElem kepl_elem,
					 std::string integ_name, CelestialObj cel_obj,
					 double t_start, double t_end, int t_steps)
	: kepl_elem(kepl_elem), integ_name(integ_name), cel_obj(cel_obj),
	  t_start(t_start), t_end(t_end), t_steps(t_steps) {

	if (kepl_elem.ecc < 0 || kepl_elem.ecc >= 1) {
		throw std::domain_error("Eccentricity must be a number between 0 and 1");
	}
	std::set valid_integ {"Euler", "Verlet", "RK4"};
	if (valid_integ.find(integ_name.c_str()) != valid_integ.end()) {
		throw std::domain_error("Invalid integrator! Should be one of: Euler, Verlet and RK4");
	}

	calc_cart();

	IntegratorFactory integ_fact(orbit_de, cel_obj, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps);
	this->integ = integ_fact.create(integ_name);
}

Satellite::Satellite(const Satellite &other)
	: cart_elem(other.cart_elem), kepl_elem(other.kepl_elem),
	  integ_name(other.integ_name), cel_obj(other.cel_obj),
	  t_start(other.t_start), t_end(other.t_end), t_steps(other.t_steps),
	  integ(other.integ->copy()) {}

Satellite &Satellite::operator=(const Satellite &other) {
	Satellite sat_copy(other);
	std::swap(this->cart_elem, sat_copy.cart_elem);
	std::swap(this->kepl_elem, sat_copy.kepl_elem);
	std::swap(this->integ_name, sat_copy.integ_name);
	std::swap(this->cel_obj, sat_copy.cel_obj);
	std::swap(this->t_start, sat_copy.t_start);
	std::swap(this->t_end, sat_copy.t_end);
	std::swap(this->t_steps, sat_copy.t_steps);
	std::swap(this->integ, sat_copy.integ);

	return *this;
}

Satellite::~Satellite() {
	delete this->integ;
}

CartElem Satellite::get_cart_elem() const { return this->cart_elem; }
KeplElem Satellite::get_kepl_elem() const { return this->kepl_elem; }

double Satellite::get_t_start() const { return this->t_start; }
double Satellite::get_t_end() const { return this->t_end; }
double Satellite::get_t_steps() const { return this->t_steps; }

std::string Satellite::get_integ_name() const { return this->integ_name; }

void Satellite::set_cart_elem(CartElem new_cart_elem) {
	this->cart_elem = new_cart_elem;
	calc_kepl();
	this->integ->set_x0(this->cart_elem.pos);
	this->integ->set_v0(this->cart_elem.vel);
}

void Satellite::set_kepl_elem(KeplElem new_kepl_elem) {
	if (new_kepl_elem.ecc < 0 || new_kepl_elem.ecc >= 1) {
		throw std::domain_error("Eccentricity must be a number between 0 and 1");
	}
	this->kepl_elem = new_kepl_elem;
	calc_cart();
	this->integ->set_x0(this->cart_elem.pos);
	this->integ->set_v0(this->cart_elem.vel);
}

void Satellite::set_t_start(int t_start) {
	if (t_start < 0) {
		throw std::domain_error("Start time must be a positive integer!");
	}
	if (t_start >= t_end) {
		throw std::domain_error("Start time must be smaller than end time!");
	}
	this->t_start = t_start;
	this->integ->set_delta_t(t_start, this->t_end);
}

void Satellite::set_t_end(int t_end) {
	if (t_end <= 0) {
		throw std::domain_error("End time must be a positive integer!");
	}
	if (t_end <= t_start) {
		throw std::domain_error("End time must be larger than start time!");
	}
	this->t_end = t_end;
	this->integ->set_delta_t(this->t_start, t_end);
}

void Satellite::set_t_steps(int t_steps) {
	if (t_steps <= 0) {
		throw std::domain_error("Steps must be a positive integer!");
	}
	this->t_steps = t_steps;
	this->integ->set_steps(t_steps);
}

void Satellite::set_integ(std::string integ_name) {
	std::set valid_integ {"Euler", "Verlet", "RK4"};
	if (valid_integ.find(integ_name.c_str()) != valid_integ.end()) {
		throw std::domain_error("Invalid integrator! Should be one of: Euler, Verlet and RK4");
	}

	this->integ_name = integ_name;

	delete this->integ;
	IntegratorFactory integ_fact(orbit_de, cel_obj, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps);
	this->integ = integ_fact.create(integ_name);
}

SimData Satellite::propagate() {
	this->integ->integrate();
	return SimData{
		this->integ->get_steps(),
		this->integ->get_time_arr(),
		this->integ->get_pos_arr(),
		this->integ->get_vel_arr()
	};
}

void Satellite::calc_kepl() {

	using std::acos;
	using std::clamp;
	using std::atan;
	using std::tan;
	using std::sqrt;
	Vec3 r = this->cart_elem.pos * 1000;	// Convert to meters
	Vec3 v = this->cart_elem.vel * 1000;	// Convert to meters

	// Cartesian state vectors -> Keplerian orbital elements
	// Steps are described here: https://downloads.rene-schwarz.com/download/M002-Cartesian_State_Vectors_to_Keplerian_Orbit_Elements.pdf

	// 1) Prep.

	// a) Orbital momentum vector
	Vec3 h = r.cross(v);

	// b) Ecc. vector and grav. parameter
	double mu = G * cel_obj.mass;
	Vec3 e = (v.cross(h) / mu) - r.norm();

	// c) Vector pointing towards asc. node and true_anom
	Vec3 n{-h.y, h.x, 0};
	this->kepl_elem.true_anom = r.dot(v) >= 0 ?
		acos(clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0))
		:
		2*PI - acos(clamp(e.dot(r) / (e.len() * r.len()), -1.0, 1.0));
	
	// 2) Inc.

	this->kepl_elem.inc = acos(h.z / h.len());

	// 3) Ecc. and ecc. anomaly (second is not currently needed)

	this->kepl_elem.ecc = e.len();
	// double E = 2*atan(tan(this->true_anom/2) / sqrt((1 + this->ecc) / (1 - this->ecc)));

	// 4) Longitude/Right asc. of asc. node and arg. of periapsis

	this->kepl_elem.ri_asc_node = n.y >= 0 ?
		acos(clamp(n.x / n.len(), -1.0, 1.0))
		:
		2*PI - acos(clamp(n.x / n.len(), -1.0, 1.0));
	
	this->kepl_elem.arg_of_per = e.z >= 0 ?
		acos(clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0))
		:
		2*PI - acos(clamp(n.dot(e) / (n.len() * e.len()), -1.0, 1.0));
	
	// 5) Mean anomaly (Not currently needed)

	// double M = E - this->ecc * std::sin(E);

	// 6) Semi-major axis

	this->kepl_elem.sem_maj_ax = 1 / (2/r.len() - (v.len() * v.len())/mu);
	this->kepl_elem.sem_maj_ax /= 1000;	// Convert back to kilometers


	// std::cout << std::setprecision(15)
	// 	<< "KEPLERIAN:\n"
	// 	<< "ecc: " << this->kepl_elem.ecc << "\n"
	// 	<< "sem_maj_ax: " << this->kepl_elem.sem_maj_ax << "\n"
	// 	<< "inc: " << this->kepl_elem.inc << "\n"
	// 	<< "ri_asc_node: " << this->kepl_elem.ri_asc_node << "\n"
	// 	<< "arg_of_per: " << this->kepl_elem.arg_of_per << "\n"
	// 	<< "true_anom: " << this->kepl_elem.true_anom << "\n";
}

void Satellite::calc_cart() {

	using std::sin;
	using std::cos;
	using std::sqrt;
	using std::atan2;
	double e = this->kepl_elem.ecc;
	double a = this->kepl_elem.sem_maj_ax * 1000;	// Convert to meters
	double i = this->kepl_elem.inc;
	double OM = this->kepl_elem.ri_asc_node;
	double w = this->kepl_elem.arg_of_per;
	double ni = this->kepl_elem.true_anom;

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

	double mu = G * cel_obj.mass;
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

	this->cart_elem.pos = transform(pos_o) / 1000;
	this->cart_elem.vel = transform(vel_o) / 1000;
}

} // namespace orbsim
