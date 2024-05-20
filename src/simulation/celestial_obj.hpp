#ifndef CELESTIAL_OBJ_HPP
#define CELESTIAL_OBJ_HPP

#include <string>


namespace orbsim {

struct CelestialObj {
	double mass;
	double radius;
};

const CelestialObj Earth{
	.mass = 5.972e24,	// [kg]
	.radius = 6378.137	// [km]
};

// const double M_Sun = 1.989e30;		// Sun Mass in [kg]

} // namespace orbsim


#endif	// CELESTIAL_OBJ_HPP

