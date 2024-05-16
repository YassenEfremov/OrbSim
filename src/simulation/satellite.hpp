#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include "simulation/math_obj.hpp"


namespace orbsim {

/**
 * @brief Satellite
 */
class Satellite {

public:
	Satellite(Vec3 x0, Vec3 v0);
	Satellite(double ecc, double sem_maj_ax,	// Shape and size
			  double inc, double ri_asc_node,	// Orientation
			  double arg_of_per, double true_anom);

	Vec3 get_pos() const;
	Vec3 get_vel() const;

private:
	// Cartesian state vectors
	Vec3 pos;	// [km]
	Vec3 vel;	// [km]

	// Keplerian orbital elements
	double ecc;			// [1]
	double sem_maj_ax;	// [km]
	double inc;			// [rad]
	double ri_asc_node;	// [rad]
	double arg_of_per;	// [rad]
	double true_anom;	// [rad]
};

} // namespace orbsim


#endif	// SATELLITE_HPP
