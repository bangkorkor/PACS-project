#ifndef ODMODEL_H
#define ODMODEL_H

#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense> // Include Eigen for vector and matrix operations

class ODModel
{
private:
    std::string _screwFilename;
    std::string _dbFilename;
    std::string _srdbFilename;
    std::string _interpType;
    double rpmC;
    int _flowRate;
    int tin;
    int tsun;
    int tring1;
    int tring2;
    int tring3;
    std::vector<std::string> spindlesTypes;
    std::vector<int> spindlesNum;
    std::vector<double> stopringDiam;
    std::map<std::string, Eigen::VectorXd> results; // Store results in a map

public:
    odmodel();               // Constructor declaration
    void generate_results(); // Assumes data to process is internally managed or simulated
    void read_db();
    void read_screw();
};

#endif // ODMODEL_H
