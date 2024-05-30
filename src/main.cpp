#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"
#include "GetPot"

int main(int argc, char **argv) {
    GetPot command_line(argc, argv);

    size_t number_of_phys_mods = command_line("np", 2);
    size_t number_of_num_mods = command_line("nn", 40);
    mixer m(number_of_phys_mods, number_of_num_mods);

    double t0 = command_line("t0",293);
    double p0 = command_line("p0",0);
    double RPM = command_line("RPM",100);
    double Q = command_line("Q",150);
    m.set_parameters(t0, p0, RPM, Q);

    m.simulate_mixer();
    m.print_mixer();
    return 0;
};