#ifndef MATH_OBJ_HPP
#define MATH_OBJ_HPP

#include <string>


namespace orbsim {

const double G = 6.67e-11;

const double M_Earth = 5.972e24;	// Earth Mass in [kg]
const double R_Earth = 6378.137;	// Earth Radius in [km]

const double PI = 3.14159265;

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

	double len() const;
	Vec3 norm() const;
	double dot(const Vec3 &other) const;
	Vec3 cross(const Vec3 &other) const;
	std::string to_str() const;
};

Vec3 operator*(double scalar, const Vec3 &v);
Vec3 operator*(const Vec3 &v, double scalar);

} // namespace orbsim


#endif	// MATH_OBJ_HPP
