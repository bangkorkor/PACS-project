#ifndef ODMODEL_H
#define ODMODEL_H

#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <Eigen/Dense> // Include Eigen for vector and matrix operations

class odmodel
{
private:
    std::string _screwfilename; // Screw file name
    std::string _dbfilename;    // Database file name
    std::string _srdbfilename;  // Stop ring database file name
    std::string _interp_type;   // Interpolation type
    double rpm_c;               // RRM
    int _flowrate;              // Flow rate
    int tin;                    // inflow temperature
    int tsun;                   // temperature of central shaft
    int tring1;                 // temperature of barrel 1
    int tring2;                 // temperature of barrel 2
    int tring3;                 // temperature of barrel 3

    std::vector<std::vector<std::map<std::string, std::string>>> mixer_descr; // Mixer description

    std::vector<std::string> spindles_types;         // Spindle types
    std::vector<int> spindles_num;                   // Number of spindles
    std::vector<int> stopring_diam;                  // Stop ring diameter
    std::unordered_map<std::string, double> results; //

public:
    class processParameter;

    odmodel(); // Constructor declaration

    // setters:
    void setScrewfilename(const std::string &filename);
    void setDbfilename(const std::string &filename);
    void setSrdbfilename(const std::string &filename);
    void setInterp_type(const std::string &type);
    void setRpm_c(double rpm);
    void setFlowRate(int flowRate);
    void setTin(int temperature);
    void setTsun(int temperature);
    void setTring1(int temperature);
    void setTring2(int temperature);
    void setTring3(int temperature);

    // getters:
    std::string getScrewfilename() const { return _screwfilename; }
    std::string getDbfilename() const { return _dbfilename; }
    std::string getSrdbfilename() const { return _srdbfilename; }
    std::string getInterp_type() const { return _interp_type; }
    double getRpm_c() const { return rpm_c; }
    int getFlowRate() const { return _flowrate; }
    int getTin() const { return tin; }
    int getTsun() const { return tsun; }
    int getTring1() const { return tring1; }
    int getTring2() const { return tring2; }
    int getTring3() const { return tring3; }

    void read_screw();
    void printMixerDescr();
};

#endif // ODMODEL_H
