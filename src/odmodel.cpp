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

void odmodel::RequestData()
{
    std::cout << "REQUEST DATA" << std::endl;

    read_screw();      // read the screw file
    printMixerDescr(); // only for visualization
    std::cout << mixer_descr.size() << std::endl;

    std::vector<phys_mod> ph_m;

    for (size_t count = 0; count < mixer_descr.size(); ++count)
    {
        int numb_of_tring = 0;
        if (count == 0)
        {
            numb_of_tring = tring1;
        }
        else if (count == 1)
        {
            numb_of_tring = tring2;
        }
        else if (count == 2)
        {
            numb_of_tring = tring3;
        }
        else
        {
            std::cerr << "WARNING: a maximum of three modules is allowed for temperature of ring\n";
            std::cerr << "Setting temperature of modules > 3 to temperature of module 3\n";
            tring = tring3;
        }

        auto wc = processParameter(rpm_c, _flowrate, tsun + 273.0, tring + 273.0);

        phys_mod ph(wc, mixer_descr[count], numb_of_tring);

        if (count == 0)
        {
            ph_m.emplace_back("module " + std::to_string(count), wc, mixer_descr[count]);
        }
        else
        {
            std::vector<double> shift; // Implement shift logic if necessary
            ph_m.emplace_back("module " + std::to_string(count), wc, mixer_descr[count], shift);
        }
    }
}