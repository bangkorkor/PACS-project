#include "processParameter.hpp"

// cosntructor
odmodel::processParameter::processParameter(double vel, double q, double tsun, double tring)
    : vel(vel), q(q), tsun(tsun), tring(tring), vdot(q / 3600.0 / 1200.0), rho(1200),
      cp([this](double tb)
         { return 1561 + 2.6 * tb; }) {}

std::vector<double> odmodel::processParameter::getCoord()
{
    return {vel, q, tsun, tring};
}

double odmodel::processParameter::calculateCp(double temperature)
{
    return cp(temperature);
}
