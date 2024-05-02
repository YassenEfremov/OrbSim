#ifndef VERLET_HPP
#define VERLET_HPP

#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Verlet integrator
 */
class Verlet {

public:
	Verlet(double M, double R0, Vec3 x0, Vec3 v0,
		  double t_i, double t_f, int steps);

	~Verlet();

	void integrate();

	double *get_time_arr() const;
	Vec3 *get_pos_arr() const;
	Vec3 *get_vel_arr() const;

	void save_to_file(const char *filename) const;

private:
	double M;
	double R0;
	int steps;
	double delta_t;
	double *time_arr;
	Vec3 *pos_arr;
	Vec3 *vel_arr;

};

} // namespace orbsim


#endif	// VERLET_HPP
