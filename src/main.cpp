#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"

int main()
{
    size_t number_of_phys_mods = 2;
    size_t number_of_num_mods = 40;
    mixer m(number_of_phys_mods, number_of_num_mods);

    double t_0 = 293;
    double p_0 = 0;
    double RPM = 100;
    double Q = 150;
    m.set_parameters(t_0, p_0, RPM, Q);

    m.simulate_mixer();
    m.print_mixer();
    return 0;
};