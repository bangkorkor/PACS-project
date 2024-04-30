#include "phys_mod.hpp"

// -------------interpolator_op class:-----------------
//
// NOT COMPLETE, I DONT UNDERSTAND THIS
//
// TODO: add other interpolation methods in the interp function, see python file
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// constructor
odmodel::phys_mod::num_mod::interpolator_op::interpolator_op(const Eigen::MatrixXd &data)
{
    coord = data.block(0, 0, data.rows(), coordLabels.size()); // Assuming data columns match labels
    for (std::size_t i = 0; i < coordLabels.size(); i++)
    {
        gsl_interp_accel *acc = gsl_interp_accel_alloc();
        gsl_spline *spline = gsl_spline_alloc(gsl_interp_linear, data.rows());
        gsl_spline_init(spline, coord.data() + i * coord.rows(), data.col(i).data(), coord.rows());
        accels.push_back(acc);
        splines.push_back(spline);
    }
}

// destructor
odmodel::phys_mod::num_mod::interpolator_op::~interpolator_op()
{
    for (std::size_t i = 0; i < splines.size(); i++)
    {
        gsl_spline_free(splines[i]);
        gsl_interp_accel_free(accels[i]);
    }
}

// Linear interpolation function
double odmodel::phys_mod::num_mod::interpolator_op::interp(std::size_t varIndex, double point)
{
    if (varIndex < splines.size())
    {
        try
        {
            return gsl_spline_eval(splines[varIndex], point, accels[varIndex]);
        }
        catch (...)
        {
            std::cerr << "Interpolation failed: point out of bounds or other error\n";
            return std::numeric_limits<double>::quiet_NaN();
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

// Nearest neighbor interpolation
double odmodel::phys_mod::num_mod::interpolator_op::interp_safe(int varIndex, double point)
{
    if (varIndex < 0 || varIndex >= coord.rows())
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Find the nearest point
    double minDistance = std::numeric_limits<double>::max();
    double nearestValue = std::numeric_limits<double>::quiet_NaN();
    for (int i = 0; i < coord.rows(); ++i)
    {
        double dist = std::abs(coord(i, varIndex) - point);
        if (dist < minDistance)
        {
            minDistance = dist;
            nearestValue = coord(i, varIndex);
        }
    }

    return nearestValue;
}

// -------------num_mod class:-----------------
