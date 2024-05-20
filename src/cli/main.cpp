#include "simulation/satellite.hpp"

#include <string>
#include <iostream>


int main(int argc, char *argv[]) {

	// Example of how to use orbsim without a GUI

    orbsim::Satellite sat;
    orbsim::SimData sim_data = sat.propagate();

	std::string output;
	for (int i = 0; i < sim_data.steps - 1; i++) {
		output += sim_data.pos_arr[i].to_str() + " " + sim_data.vel_arr[i].to_str() + "\n";
	}

	std::cout << output << "\n";

    return 0;
}
