#include "mixer.hpp"

mixer::mixer(size_t number_pm, size_t number_nn)
{
    for (size_t i = 0; i < number_pm; i++)
    {
        screw.push_back(phys_mod(number_nn));
    }
}

void mixer::print_mixer()
{
    cout << "------Mixer:------" << endl;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].print_phys_mod();
    }
}

void mixer::set_parameters(double t0, double p0, double RPM, double Q)
{
    t_0 = t0;
    p_0 = p0;
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_RPM(RPM);
        screw[i].set_Q(Q);
    }
}

void mixer::simulate_mixer()
{
    for (size_t i = 0; i < screw.size(); i++)
    {
        if (i == 0)
        {
            screw[i].set_tIn(t_0);
        }
        else
        {
            screw[i].set_tIn(screw[i - 1].get_tOut());
        }
        screw[i].simulate_phys_mod();
    }
}