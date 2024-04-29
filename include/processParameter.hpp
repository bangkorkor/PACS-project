#pragma once
#include <vector>
#include <functional>
#include <iostream>

// Include the header where ODModel is declared.
#include "ODModel.hpp"

// Now define the nested class ProcessParameter within the ODModel scope.
class odmodel::processParameter
{
private:
    double vel;                       // Angular velocity in RPM
    double q;                         // Flow rate in Kg/h
    double tsun;                      // Sun temperature in K
    double tring;                     // Ring temperature in K
    double vdot;                      // Computed value
    double rho;                       // Density, constant at 1200
    std::function<double(double)> cp; // Function to calculate cp based on temperature

public:
    // Constructor
    processParameter(double vel, double q, double tsun, double tring)
        : vel(vel), q(q), tsun(tsun), tring(tring), vdot(q / 3600.0 / 1200.0), rho(1200),
          cp([this](double tb)
             { return 1561 + 2.6 * tb; }) {}

    // Methods
    std::vector<double> getCoord();
    double calculateCp(double temperature);
};
