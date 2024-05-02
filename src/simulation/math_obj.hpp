#ifndef MATH_OBJ_HPP
#define MATH_OBJ_HPP

#include <string>


namespace orbsim {

const double G = 6.67e-11;

struct Vec3 {
	double x;
	double y;
	double z;

	Vec3 operator+(const Vec3 &rhs);
	Vec3 operator-(const Vec3 &rhs);
	Vec3 operator-();
	Vec3 &operator*=(double scalar);
	Vec3 operator/(double scalar);
	Vec3 &operator/=(double scalar);

	double len();
	std::string to_str();
};

Vec3 operator*(double scalar, const Vec3 &v);
Vec3 operator*(const Vec3 &v, double scalar);

} // namespace orbsim


#endif	// MATH_OBJ_HPP
