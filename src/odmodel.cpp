#include "odmodel.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>

// Constructor implementation
odmodel::odmodel()
{
    std::cout << "odmodel init" << std::endl;
    _screwfilename = "";
    _dbfilename = "";
    _srdbfilename = "";
    _interp_type = "";
    rpm_c = -100.0;
    _flowrate = 150;
    tin = 20;
    tsun = 20;
    tring1 = 50;
    tring2 = 50;
    tring3 = 50;
}

// Setters implementation
void odmodel::setScrewfilename(const std::string &filename) { _screwfilename = filename; }
void odmodel::setDbfilename(const std::string &filename) { _dbfilename = filename; }
void odmodel::setSrdbfilename(const std::string &filename) { _srdbfilename = filename; }
void odmodel::setInterp_type(const std::string &type) { _interp_type = type; }
void odmodel::setRpm_c(double rpm) { rpm_c = rpm; }
void odmodel::setFlowRate(int flowRate) { _flowrate = flowRate; }
void odmodel::setTin(int temperature) { tin = temperature; }
void odmodel::setTsun(int temperature) { tsun = temperature; }
void odmodel::setTring1(int temperature) { tring1 = temperature; }
void odmodel::setTring2(int temperature) { tring2 = temperature; }
void odmodel::setTring3(int temperature) { tring3 = temperature; }
