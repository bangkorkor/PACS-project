#include "mixer.hpp"

mixer::mixer(size_t number_pm, size_t number_nn)
{
    for (size_t i = 0; i < number_pm; i++)
    {
        screw.push_back(phys_mod(number_nn));
    }
}

// getter
vector<phys_mod> mixer::get_screw() { return screw; } // is needed for vizulize class

void mixer::print_mixer()
{
    cout << "------Mixer:------" << endl;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].print_phys_mod();
    }
}

void mixer::set_parameters(double t0, double RPM, double Q, string type, string SRtype)
{
    t_0 = t0;
    p_0 = 0;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_RPM(RPM);
        screw[i].set_Q(Q);
        screw[i].set_type(type);
        screw[i].set_SRtype(SRtype);
    }
}

void mixer::simulate_mixer()
{
    // set the initial temperature, set all screws to the same temperature
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_tIn(t_0);
    }

    size_t iter = 100;
    for (size_t i = 0; i < iter; i++)
    {
        for (size_t i = 0; i < screw.size(); i++)
        {
        screw[i].update_p();
        // update the temperature of the incoming fluid
        if (i < screw.size() - 1)
        {
            screw[i + 1].set_tIn(screw[i].get_tOut());
            }
        }

    for (size_t i = 0; i < screw.size(); i++)
        {
        screw[i].update_t();
        }
    }
    

    
}