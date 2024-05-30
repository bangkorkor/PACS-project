#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

#pragma once

#include <iostream>
#include <map>
#include "mixer.hpp"

class visualize
{
private:
    vector<double> t_data; // data with the temperature of each num_mod. The index is the possision of the num_mod in the mixer
    vector<double> p_data; // data with the pressure of each num_mod

public:
    // constructor takes in the screw to visualize
    visualize(mixer m);

    void vizualize_screw();
};

#endif // VISUALIZE_HPP
