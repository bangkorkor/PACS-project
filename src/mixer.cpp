#include "mixer.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

mixer::mixer(size_t number_pm, size_t number_nn)
{
    for (size_t i = 0; i < number_pm; i++)
    {
        screw.push_back(phys_mod(number_nn));
    }
}

// getter
std::vector<phys_mod> mixer::get_screw() { return screw; } // is needed for visualize class

void mixer::print_mixer()
{
    std::cout << "------Mixer:------" << std::endl;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].print_phys_mod();
    }
}

void mixer::set_parameters(double t0, double RPM, double Q, const std::string &type, const std::string &SRtype)
{
    string_parameters["type"] = type;
    string_parameters["SRtype"] = SRtype;
    parameters["t0"] = t0;
    parameters["RPM"] = RPM;
    parameters["Q"] = Q;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_RPM(RPM);
        screw[i].set_Q(Q);
        screw[i].set_type(type);
        screw[i].set_SRtype(SRtype);
    }
}

std::vector<std::string> mixer::get_parameters()
{
    std::vector<std::string> param_list;
    for (const auto &param : parameters)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << param.second;
        param_list.push_back(param.first + "=" + oss.str());
    }
    for (const auto &param : string_parameters)
    {
        param_list.push_back(param.first + "=" + param.second);
    }
    return param_list;
}

void mixer::simulate_mixer()
{
    const int max_iter = 100;
    double temp_convergence_threshold = 0.0001; // Threshold for temperature convergence
    double press_convergence_threshold = 0.001; // Threshold for pressure convergence
    bool is_converged = false;
    std::vector<double> last_temps(screw.size());
    std::vector<double> last_presses(screw.size());

    // Initialize last temperatures and pressures
    for (size_t i = 0; i < screw.size(); i++)
    {
        last_temps[i] = screw[i].get_tOut();
        last_presses[i] = screw[i].get_model().back().get_p();
    }

    // set the initial temperature
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_tIn(parameters["t0"]);
    }

    int iter = 0;
    while (iter < max_iter && !is_converged)
    {

        // iterate going backwards for pressures
        for (int j = static_cast<int>(screw.size()) - 1; j >= 0; j--)
        {
            screw[j].update_p();
        }

        // update temperatures and check convergence
        for (size_t j = 0; j < screw.size(); j++)
        {
            double old_temp = last_temps[j];
            double old_press = last_presses[j];
            screw[j].update_t();

            if (j != screw.size() - 1)
            {
                screw[j + 1].set_tIn(screw[j].get_tOut());
            }

            last_temps[j] = screw[j].get_tOut();
            last_presses[j] = screw[j].get_model().back().get_p();

            // Check convergence for both temperature and pressure
            if (fabs(old_temp - last_temps[j]) < temp_convergence_threshold &&
                fabs(old_press - last_presses[j]) < press_convergence_threshold)
            {
                is_converged = true;
            }
        }
        iter++;
    }
    cout << "Simulation took: " << iter << " iterations" << endl;
}
