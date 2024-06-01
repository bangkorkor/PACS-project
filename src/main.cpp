#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"
#include "visualize.hpp"
#include "data_collection.hpp"
#include "GetPot"

using namespace std;

int main(int argc, char **argv)
{
    GetPot command_line(argc, argv);

    // reading data
    string config_filename = "data/config.csv";
    data_collection d;
    d.read_csv(config_filename);
    auto data = d.get_data();

    int model_number = command_line("model", 4);
    auto data_line = data[model_number];

    size_t number_of_phys_mods = command_line("npm", stoi(data_line[2]));
    size_t number_of_num_mods = command_line("nm", stoi(data_line[3]));

    mixer m(number_of_phys_mods, number_of_num_mods);

    double t0 = command_line("t0", stod(data_line[4]));
    double RPM = command_line("RPM", stod(data_line[5]));
    double Q = command_line("Q", stod(data_line[6]));

    m.set_parameters(t0, RPM, Q, data_line[0], data_line[1]); // Set parameters

    m.simulate_mixer();
    m.print_mixer();

    vector<string> parameters = m.get_parameters(); // Retrieve formatted parameters from mixer

    visualize v(m, parameters); // Create visualization with parameters
    v.vizualize_screw();        // Generate visualization

    return 0;
}
