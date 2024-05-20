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
//
//
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// constructor

// odmodel::phys_mod::num_mod::num_mod(std::string num_type, Eigen::VectorXd points, odmodel::phys_mod::num_mod *prev_mod, bool verbose)
//     : verbose(verbose), type(num_type), pts(points), prev(prev_mod), next(nullptr),
//       u(0.0), T(0.0), phi(0.0), gradp(0.0), tot_gradp(0.0), p(0.0), qt(0.0), q0(0.0)
// {
//     dx = pts(1) - pts(0); // Assuming points are consecutive
//     update_T = {
//         {"mix", std::bind(&odmodel::phys_mod::num_mod::energy_balance, this, std::placeholders::_1)},
//         {"stop", std::bind(&odmodel::phys_mod::num_mod::add_dtemp, this, std::placeholders::_1)}};
//     update_p = std::bind(&odmodel::phys_mod::num_mod::add_dp, this, std::placeholders::_1);
//     update_phi = std::bind(&odmodel::phys_mod::num_mod::compute_phi, this);
// }

// void odmodel::phys_mod::num_mod::set_db(const Eigen::MatrixXd &df)
// {
//     Eigen::MatrixXd filtered;

//     if (nplanets > 0)
//     {
//         std::vector<int> validIndices;

//         // Assuming column 0 is NPlanets and column 1 is PlanetsType
//         for (int i = 0; i < df.rows(); i++)
//         {
//             if (df(i, 0) == nplanets && df(i, 1) == std::stoi(planet_type))
//             {
//                 validIndices.push_back(i);
//             }
//         }

//         if (validIndices.empty())
//         {
//             throw std::runtime_error("Combination of " + std::to_string(nplanets) + " planets of type " + planet_type + " not found");
//         }

//         // Create a new matrix for filtered data
//         filtered.resize(validIndices.size(), df.cols());
//         for (size_t i = 0; i < validIndices.size(); ++i)
//         {
//             filtered.row(i) = df.row(validIndices[i]);
//         }
//     }
//     else
//     {
//         // No filtering needed, use all data
//         filtered = df;
//     }

//     interpolator = new interpolator_op(filtered);
// }

// void odmodel::phys_mod::num_mod::energy_balance(double t_prev)
// {
//     if (verbose)
//         std::cout << "Compute energy balance ";

//     std::vector<double> interp_point = {t_prev};
//     auto coords = phys_mod.wc.getCoord();
//     interp_point.insert(interp_point.end(), coords.begin(), coords.end());

//     double vh_dt = dx * interpolator->interp("VHspec", interp_point);
//     double vdot = phys_mod.wc.props["vdot"];
//     double cp = phys_mod.wc.props["cp"];
//     double rho = phys_mod.wc.props["rho"];
//     double mdot = rho * vdot;

//     double t_barr = phys_mod.wc.props["TRing"];
//     double t_shaft = phys_mod.wc.props["TSun"];
//     double a_barr = 6.0 * 1.56e-3;
//     double a_shaft = 6.0 * 7e-4;
//     double alpha_barr = 100.0;
//     double alpha_shaft = 100.0;

//     double tavg = T; // Average temperature
//     double qh_barr = alpha_barr * a_barr * (t_barr - tavg);
//     double qh_shaft = alpha_shaft * a_shaft * (t_shaft - tavg);
//     double dt_eb = (vh_dt + qh_barr + qh_shaft) / (mdot * cp * 6.0);

//     T = t_prev + dt_eb * phi;

//     if (verbose)
//         std::cout << dt_eb << " " << dt_eb * phi << " " << t_prev << " " << T << std::endl;
// }

// void odmodel::phys_mod::num_mod::add_dtemp(double t_prev)
// {
//     if (verbose)
//         std::cout << "Add delta Temperature ";

//     std::vector<double> interp_point = {t_prev};
//     auto coords = phys_mod.wc.getCoord();
//     interp_point.insert(interp_point.end(), coords.begin(), coords.end());

//     double dt = interpolator->interp("deltaTBulk", interp_point);
//     T = t_prev + dt;

//     if (verbose)
//         std::cout << dt << " " << t_prev << " " << T << std::endl;
// }

// void odmodel::phys_mod::num_mod::add_dp(double p_prev)
// {
//     if (verbose)
//         std::cout << "Add delta Pressure ";

//     std::vector<double> interp_point = {T};
//     auto coords = phys_mod.wc.getCoord();
//     interp_point.insert(interp_point.end(), coords.begin(), coords.end());

//     double dp;
//     if (type == "stop")
//     {
//         dp = interpolator->interp("deltaP", interp_point);
//     }
//     else
//     {
//         dp = dx * interpolator->interp("gradP", interp_point);
//     }

//     double new_p = p_prev - dp;

//     if (new_p > 0)
//     {
//         p = new_p;
//     }
//     else
//     {
//         p = 0; // Ensuring pressure doesn't drop below zero
//     }

//     gradp = dp;
//     if (verbose)
//         std::cout << dp << " " << p_prev << " " << new_p << " " << p << std::endl;
// }

// void odmodel::phys_mod::num_mod::compute_phi()
// {
//     if (verbose)
//         std::cout << "Compute filling factor ";

//     std::vector<double> interp_point = {T};
//     auto coords = phys_mod.wc.getCoord();
//     interp_point.insert(interp_point.end(), coords.begin(), coords.end());

//     if (type == "stop")
//     {
//         if (verbose)
//             std::cout << "stopring ";
//         phi = 1; // Assuming a fixed value for 'stop' type
//         q0 = 0;
//     }
//     else
//     {
//         double q0 = interpolator->interp("Q0", interp_point);
//         this->q0 = q0;
//         phi = gradp / tot_gradp + (1 - gradp / tot_gradp) * phys_mod.wc.props["vdot"] / q0;
//     }

//     if (verbose)
//         std::cout << phi << std::endl;
// }
