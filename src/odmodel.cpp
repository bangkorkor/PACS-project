#include "odmodel.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

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

// reads the machine.csv file
void odmodel::read_screw()
{
    std::ifstream file(getScrewfilename().c_str());
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << getScrewfilename() << std::endl;
        return;
    }

    std::string line;
    getline(file, line); // Read the header line
    boost::char_separator<char> sep(";");
    boost::tokenizer<boost::char_separator<char>> tokens(line, sep);
    std::vector<std::string> headers(tokens.begin(), tokens.end());

    std::vector<std::map<std::string, std::string>> phi_mod_descr;

    while (getline(file, line))
    {
        boost::tokenizer<boost::char_separator<char>> row(line, sep);
        std::vector<std::string> data(row.begin(), row.end());
        std::map<std::string, std::string> loc_el;

        auto it = headers.begin();
        for (const auto &field : data)
        {
            loc_el[*it++] = field;
        }

        phi_mod_descr.push_back(loc_el);

        if (loc_el["type"] == "SR")
        {
            mixer_descr.push_back(phi_mod_descr);
            phi_mod_descr.clear();
        }
    }
    file.close();
}

// ONLY FOR VISUALIZATION, prints the mixer description
void odmodel::printMixerDescr()
{
    for (const auto &descr : mixer_descr)
    {
        for (const auto &map : descr)
        {
            for (const auto &pair : map)
            {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }
            std::cout << "------" << std::endl;
        }
        std::cout << "=============" << std::endl;
    }
}