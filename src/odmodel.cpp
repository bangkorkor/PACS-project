#include "odmodel.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>

// Constructor implementation
odmodel::odmodel() : rpmC(-100.0), _flowRate(150), tin(20), tsun(20), tring1(50), tring2(50), tring3(50)
{
    _screwFilename = "";
    _dbFilename = "";
    _srdbFilename = "";
    _interpType = "";

    std::cout << "ODModel initialized" << std::endl;
}

void odmodel::generate_results()
{
    // For demonstration, let's assume some data is already set
    std::cout << "generate 1D fields" << std::endl;

    // This is a placeholder for where you would handle data from a machine or simulation
    Eigen::VectorXd z = Eigen::VectorXd::LinSpaced(100, 0, 99); // Dummy data generation
    results["z"] = z;                                           // Storing results in a map

    // You would also process and store other metrics like pressure, temperature etc.
    // Here we simply log a message
    std::cout << "Results generated and stored" << std::endl;
}

void odmodel::read_db()
{
    std::ifstream file(_dbFilename);
    if (!file)
    {
        std::cerr << "Failed to open database file: " << _dbFilename << std::endl;
        return;
    }
    std::string line;
    // Assuming first line is header
    getline(file, line); // Skip header

    // Read data lines
    while (getline(file, line))
    {
        std::cout << "Reading line: " << line << std::endl;
        // Here you would process each line
    }

    file.close();
    std::cout << "Database file read successfully." << std::endl;
}

void odmodel::read_screw()
{
    std::ifstream file(_screwFilename);
    if (!file)
    {
        std::cerr << "Failed to open screw configuration file: " << _screwFilename << std::endl;
        return;
    }
    std::string line;
    getline(file, line); // Read header
    std::cout << "Header: " << line << std::endl;

    // Read data lines
    while (getline(file, line))
    {
        std::cout << "Reading screw setup line: " << line << std::endl;
        // Process each screw setup line
    }

    file.close();
    std::cout << "Screw configuration file read successfully." << std::endl;
}
