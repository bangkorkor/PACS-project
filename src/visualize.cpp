#include "visualize.hpp"

#include <iostream>
#include <fstream>
#include <cmath> // For math functions

visualize::visualize(mixer m)
{
    vector<phys_mod> screw = m.get_screw();   // this is the mixer
    for (size_t i = 0; i < screw.size(); i++) // iterates over the phys_mods
    {
        vector<num_mod> model = screw[i].get_model(); // this gets the num_mods of the phys_mod
        for (size_t j = 0; j < model.size(); j++)     // iterates over the num_mods
        {
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

        // genereate csv file with the data
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

    // Step 2: Execute the Python script
    system("python src/plot.py"); // Ensure that the python command is configured correctly in your environment
};
