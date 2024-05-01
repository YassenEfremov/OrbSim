#include "math_obj.hpp"

#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>


namespace orbsim {

Vec3 Vec3::operator+(const Vec3 &rhs) {
	return Vec3{
		this->x + rhs.x,
		this->y + rhs.y,
		this->z + rhs.z,
	};
}

Vec3 Vec3::operator-(const Vec3 &rhs) {
	return Vec3{
		this->x - rhs.x,
		this->y - rhs.y,
		this->z - rhs.z,
	};
}

Vec3 Vec3::operator/(double scalar) {
	return Vec3{
		this->x / scalar,
		this->y / scalar,
		this->z / scalar
	};
}

double Vec3::len() {
	// use that len = sqrt(dot product with itself)
	return std::sqrt(x*x + y*y + z*z);
}

std::string Vec3::to_str() {
	std::ostringstream os;
	os.setf(std::ios::fixed);
	os.precision(8);
	os << "[" << x << "  " << y << "  " << z << "]";
	return os.str();
}

Vec3 operator*(double scalar, const Vec3 &v) {
	return Vec3{
		scalar * v.x,
		scalar * v.y,
		scalar * v.z
	};
}

Vec3 operator*(const Vec3 &v, double scalar) {
	return scalar * v;
}

} // namespace orbsim
