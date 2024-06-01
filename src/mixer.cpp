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
std::vector<phys_mod> mixer::get_screw() { return screw; } // is needed for vizulize class

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
    // set the initial temperature
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_tIn(parameters["t0"]);
    }

    int iter = 1000;
    for (int i = 0; i < iter; i++)
    {
        // iterate going backwards
        for (int j = static_cast<int>(screw.size()) - 1; j >= 0; j--)
        {
            screw[j].update_p();
        }
        for (size_t j = 0; j < screw.size(); j++)
        {
            screw[j].update_t();
            // update the temperature of the next phys_mod
            if (j != screw.size() - 1)
            {
                screw[j + 1].set_tIn(screw[j].get_tOut());
            }
        }
    }
}
