#include "odmodel.hpp"
#include <iostream>
#include <functional>

int main()
{
    // Example data for initialization
    std::string num_type = "mixer";
    std::vector<double> points = {0.0, 1.0, 2.0, 3.0}; // Sample points
    odmodel *prev_mod = nullptr;                       // No previous module in this example
    std::function<void(odmodel &)> phys_mod = [](odmodel &mod)
    {
        // Placeholder for a physical model update function
        std::cout << "Updating physical model" << std::endl;
    };

    // Initialize odmodel object
    odmodel model(num_type, points, prev_mod, phys_mod);

    // Accessing and printing values
    std::cout << "Type of numerical module: " << num_type << std::endl;
    std::cout << "First point: " << points[0] << std::endl;
    std::cout << "Second point: " << points[1] << std::endl;
    std::cout << "Number of planets: " << model.getNplanets() << std::endl; // Assuming a getter exists
    std::cout << "Planet type:" << model.getPlanetType() << std::endl;      // Assuming a getter exists

    return 0;
}
