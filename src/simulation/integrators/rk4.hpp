#ifndef RK4_HPP
#define RK4_HPP

#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief RK4 (Runge-Kutta 4th order) integrator
 */
class RK4 {

public:
	RK4(double M, double R0, Vec3 x0, Vec3 v0,
		  double t_i, double t_f, int steps);

	~RK4();

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


#endif	// RK4_HPP
