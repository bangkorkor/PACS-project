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

void mixer::simulate_mixer()
{
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].simulate_phys_mod();
    }
}