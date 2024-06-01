#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "simulation/diff_eq.hpp"
#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Generic integrator
 */
class Integrator {

public:
	Integrator(DESystem<Vec3> de_system, double M, double R0, Vec3 x0, Vec3 v0,
			   double t_i, double t_f, int steps);
	Integrator(const Integrator &other);
	Integrator &operator=(const Integrator &other);

	virtual Integrator *copy() const = 0;

	~Integrator();

	virtual void integrate() = 0;

	int get_steps() const;
	double get_delta_t() const;
	double *get_time_arr() const;
	Vec3 *get_pos_arr() const;
	Vec3 *get_vel_arr() const;

	void set_steps(int steps);
	void set_delta_t(int t_start, int t_end);
	void set_x0(Vec3 x0);
	void set_v0(Vec3 v0);

	void save_to_file(const char *filename) const;

protected:
	double M;	// [kg]
	double R0;	// [km]
	DESystem<Vec3> de_system;
	int steps;
	double delta_t;
	double *time_arr;
	Vec3 *pos_arr;	// [km]
	Vec3 *vel_arr;	// [km/s]

	double R_dim;
	double V_dim;
	double T_dim;
};

} // namespace orbsim


#endif	// INTEGRATOR_HPP
