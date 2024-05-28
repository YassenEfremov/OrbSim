#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include "simulation/integrators/integrator.hpp"
#include "simulation/celestial_obj.hpp"
#include "simulation/math_obj.hpp"

#include <string>
// #include <vector>


namespace orbsim {

struct SimData {
	int steps;
	double *time_arr;
	Vec3 *pos_arr;	// [km]
	Vec3 *vel_arr;	// [km]

	// maybe better?
	// std::vector<Vec3> pos_arr;	// [km]
	// std::vector<Vec3> vel_arr;	// [km]
};

/**
 * @brief Satellite
 */
class Satellite {

public:
	Satellite(CartElem cart_elem = {Vec3{7000, 0.000001, -0.001608},
									Vec3{0.000002, 1.310359, 7.431412}},
			  std::string integ_name = "RK4", CelestialObj cel_obj = Earth,
			  double t_start = 0, double t_end = 86400, int t_steps = 8640);
	Satellite(KeplElem kepl_elem,
			  std::string integ_name, CelestialObj cel_obj,
			  double t_start, double t_end, int t_steps);
	Satellite(const Satellite &other);
	Satellite &operator=(const Satellite &other);

	~Satellite();

	CartElem get_cart_elem() const;
	KeplElem get_kepl_elem() const;

	double get_t_start() const;
	double get_t_end() const;
	double get_t_steps() const;
	std::string get_integ_name() const;

	void set_cart_elem(CartElem new_cart_elem);
	void set_kepl_elem(KeplElem new_kepl_elem);

	void set_t_start(int t_start);
	void set_t_end(int t_end);
	void set_t_steps(int t_steps);
	void set_integ(std::string integ_name);

	SimData propagate();

private:
	void calc_kepl();
	void calc_cart();

	CartElem cart_elem;
	KeplElem kepl_elem;

	std::string integ_name;
	CelestialObj cel_obj;
	double t_start;
	double t_end;
	double t_steps;

	Integrator *integ;
};

} // namespace orbsim


#endif	// SATELLITE_HPP
