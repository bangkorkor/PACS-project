#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "mixer.hpp"

class visualize
{
private:
    std::vector<double> t_data;          // data with the temperature of each num_mod. The index is the position of the num_mod in the mixer
    std::vector<double> p_data;          // data with the pressure of each num_mod
    std::vector<std::string> parameters; // stores parameters for visualization

public:
    // constructor takes in the screw to visualize and parameters
    visualize(mixer m, const std::vector<std::string> &params);

    void vizualize_screw();
};

#endif // VISUALIZE_HPP
