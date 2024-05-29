#include "math_obj.hpp"

#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>


namespace orbsim {

Vec3 Vec3::operator+(const Vec3 &rhs) const {
	return Vec3{
		this->x + rhs.x,
		this->y + rhs.y,
		this->z + rhs.z,
	};
}

Vec3 Vec3::operator-(const Vec3 &rhs) const {
	return Vec3{
		this->x - rhs.x,
		this->y - rhs.y,
		this->z - rhs.z,
	};
}

Vec3 Vec3::operator-() const {
	return Vec3{
		- this->x,
		- this->y,
		- this->z
	};
}

Vec3 &Vec3::operator*=(double scalar) {
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

Vec3 Vec3::operator/(double scalar) const {
	return Vec3{
		this->x / scalar,
		this->y / scalar,
		this->z / scalar
	};
}

Vec3 &Vec3::operator/=(double scalar) {
	return *this *= 1/scalar;
}

bool Vec3::operator==(const Vec3 &other) const {
	auto compd = [](double a, double b) -> bool {
		const double epsilon = 1e-8;
		return std::fabs(a - b) < epsilon;
	};
	
	return compd(this->x, other.x) &&
		   compd(this->y, other.y) &&
		   compd(this->z, other.z);
}

bool Vec3::operator!=(const Vec3 &other) const {
	return !(*this == other);
}


double Vec3::len() const {
	// use that len = sqrt(dot product with itself)
	return std::sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::norm() const {
	return *this / this->len();
}

double Vec3::dot(const Vec3 &other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
	return Vec3{
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x,
	};
}

std::string Vec3::to_str() const {
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
