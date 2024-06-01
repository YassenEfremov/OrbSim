#ifndef DIFF_EQ_HPP
#define DIFF_EQ_HPP

#include "simulation/math_obj.hpp"

#include <functional>
#include <vector>

#include <cmath>
#include <cstddef>


namespace orbsim {

/**
 * @brief Generic system of differential equations
 */
template <typename T>
class DESystem {

public:
	DESystem(std::vector<std::function<T(T x)>> equations): equations(equations) {}

	void add_eq(std::function<T(T x)> f, std::size_t order) {
		auto it = this->equations.begin() + order;
		this->equations.insert(it, f);
	}

	std::function<T(T x)> get_rhs(std::size_t order) const {
		return this->equations[order];
	}

private:
	std::vector<std::function<T(T x)>> equations;
};

const DESystem<Vec3> orbit_de({
	[](Vec3 x) { return x; },
	[](Vec3 v) { return v; },
	[](Vec3 x) { return - x / std::pow(x.len(), 3); },
});

} // namespace orbsim

#endif	// DIFF_EQ_HPP
