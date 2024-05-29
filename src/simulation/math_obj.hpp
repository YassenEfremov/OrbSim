#ifndef MATH_OBJ_HPP
#define MATH_OBJ_HPP

#include <string>


namespace orbsim {

const double G = 6.67430e-11;
const double PI = 3.14159265358979323846;

struct Vec3 {
	double x;
	double y;
	double z;

	Vec3 operator+(const Vec3 &rhs) const;
	Vec3 operator-(const Vec3 &rhs) const;
	Vec3 operator-() const;
	Vec3 &operator*=(double scalar);
	Vec3 operator/(double scalar) const;
	Vec3 &operator/=(double scalar);
	bool operator==(const Vec3 &other) const;
	bool operator!=(const Vec3 &other) const;

	double len() const;
	Vec3 norm() const;
	double dot(const Vec3 &other) const;
	Vec3 cross(const Vec3 &other) const;
	std::string to_str() const;
};

Vec3 operator*(double scalar, const Vec3 &v);
Vec3 operator*(const Vec3 &v, double scalar);

struct CartElem {
	// Cartesian state vectors
	Vec3 pos;	// [km]
	Vec3 vel;	// [km/s]
};

struct KeplElem {
	// Keplerian orbital elements
	double ecc;			// [1]
	double sem_maj_ax;	// [km]
	double inc;			// [rad]
	double ri_asc_node;	// [rad]
	double arg_of_per;	// [rad]
	double true_anom;	// [rad]
};

} // namespace orbsim


#endif	// MATH_OBJ_HPP
