#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"
#include "visualize.hpp"
#include "data_collection.hpp"

#include "GetPot"

using namespace std;

void request_data()
{
}

int main(int argc, char **argv)
{
    // reading data
    string config_filename = "data/config.csv";
    data_collection d;
    d.read_csv(config_filename);
    auto data = d.get_data();

    auto data_line = data[2];

    GetPot command_line(argc, argv);

    size_t number_of_phys_mods = command_line("np", stoi(data_line[2]));
    size_t number_of_num_mods = command_line("nn", stoi(data_line[3]));
    mixer m(number_of_phys_mods, number_of_num_mods);

    double t0 = command_line("t0", stod(data_line[4]));
    double RPM = command_line("RPM", stod(data_line[5]));
    double Q = command_line("Q", stod(data_line[6]));

    m.set_parameters(t0, RPM, Q);
    m.simulate_mixer();
    m.print_mixer();

    visualize v(m);
    v.vizualize_screw();

    return 0;
};