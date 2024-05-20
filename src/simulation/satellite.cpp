#include "satellite.hpp"

#include "integrators/euler.hpp"
#include "integrators/verlet.hpp"
#include "integrators/rk4.hpp"
#include "celestial_obj.hpp"
#include "math_obj.hpp"

#include <cmath>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <utility>


namespace orbsim {

Satellite::Satellite(CartElem cart_elem,
					 std::string integ_name, CelestialObj cel_obj,
					 double t_start, double t_end, double t_steps)
	: cart_elem(cart_elem), integ_name(integ_name), cel_obj(cel_obj),
	  t_start(t_start), t_end(t_end), t_steps(t_steps) {

	calc_kepl();

	std::map<std::string, std::function<orbsim::Integrator *()>> str_integ {
		{"Euler", [=]() { return new orbsim::Euler(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"Verlet", [=]() { return new orbsim::Verlet(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"RK4", [=]() { return new orbsim::RK4(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }}
	};

	this->integ = str_integ[integ_name]();
}

Satellite::Satellite(KeplElem kepl_elem,
					 std::string integ_name, CelestialObj cel_obj,
					 double t_start, double t_end, double t_steps)
	: kepl_elem(kepl_elem), integ_name(integ_name), cel_obj(cel_obj),
	  t_start(t_start), t_end(t_end), t_steps(t_steps) {

	calc_cart();

	std::map<std::string, std::function<orbsim::Integrator *()>> str_integ {
		{"Euler", [=]() { return new orbsim::Euler(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"Verlet", [=]() { return new orbsim::Verlet(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"RK4", [=]() { return new orbsim::RK4(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }}
	};

	this->integ = str_integ[integ_name]();
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
}

void Satellite::set_kepl_elem(KeplElem new_kepl_elem) {
	this->kepl_elem = new_kepl_elem;
	calc_cart();
}

void Satellite::set_t_start(int t_start) {
	this->t_start = t_start;
	this->integ->set_delta_t(t_start, this->t_end);
}

void Satellite::set_t_end(int t_end) {
	this->t_end = t_end;
	this->integ->set_delta_t(this->t_start, t_end);
}

void Satellite::set_t_steps(int t_steps) {
	if (t_steps > 0) {
		this->t_steps = t_steps;
	}
	this->integ->set_steps(t_steps);
}

void Satellite::set_integ(std::string integ_name) {
	this->integ_name = integ_name;

	std::map<std::string, std::function<orbsim::Integrator *()>> str_integ {
		{"Euler", [=]() { return new orbsim::Euler(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"Verlet", [=]() { return new orbsim::Verlet(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }},
		{"RK4", [=]() { return new orbsim::RK4(cel_obj.mass, cel_obj.radius, this->cart_elem.pos, this->cart_elem.vel, t_start, t_end, t_steps); }}
	};

	delete this->integ;
	this->integ = str_integ[integ_name]();
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
