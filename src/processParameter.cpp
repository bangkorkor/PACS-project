#include "processParameter.hpp"

std::vector<double> odmodel::processParameter::getCoord()
{
    return {vel, q, tsun, tring};
}

double odmodel::processParameter::calculateCp(double temperature)
{
    return cp(temperature);
}
