#include "visualize.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

visualize::visualize(mixer m, const std::vector<std::string> &params) : parameters(params)
{
    std::vector<phys_mod> screw = m.get_screw(); // this is the mixer
    for (size_t i = 0; i < screw.size(); i++)
    {                                                      // iterates over the phys_mods
        std::vector<num_mod> model = screw[i].get_model(); // this gets the num_mods of the phys_mod
        for (size_t j = 0; j < model.size(); j++)
        {                                       // iterates over the num_mods
            t_data.push_back(model[j].get_t()); // this gets the temperature of the num_mod
            p_data.push_back(model[j].get_p()); // this gets the pressure of the num_mod
        }
    }
}

void visualize::vizualize_screw()
{
    std::ofstream file("build/plotting_data.csv"); // opens file

    // step 1: writes csv file
    if (file.is_open())
    {
        file << "t,p\n"; // Header for CSV

        // generate csv file with the data
        for (size_t i = 0; i < t_data.size(); i++)
        {
            file << t_data[i] << "," << p_data[i] << "\n";
        }
        file.close();
        std::cout << "CSV file has been written." << std::endl;
    }
    else
    {
        std::cout << "Unable to open file." << std::endl;
    }

    // Execute the Python script with parameters
    std::string command = "python src/plot.py";
    for (const auto &param : parameters)
    {
        command += " " + param;
    }
    system(command.c_str());
}
