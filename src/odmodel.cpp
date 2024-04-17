// odmodel.cpp
#include "odmodel.hpp"
#include <iostream>

odmodel::odmodel(std::string num_type, std::vector<double> points, odmodel *prev_mod, std::function<void(odmodel &)> phys_mod, int npl, std::string plt, bool verbose)
    : verbose(verbose), type(num_type), nplanets(npl), planet_type(plt), pts(points),
      u(0.0), T(0.0), phi(0.0), gradp(0.0), tot_gradp(0.0), p(0.0), qt(0.0), q0(0.0),
      prev(prev_mod), next(nullptr), phys_mod(phys_mod)
{
    if (points.size() > 1)
    {
        dx = points[1] - points[0]; // Assuming uniform spacing for simplicity
    }
    // Initialize function pointers or std::function here
    // update_T, update_p, update_phi as per logic in your application
}

odmodel::~odmodel()
{
    // Cleanup if necessary
}
