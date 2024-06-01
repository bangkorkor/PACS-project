#ifndef MIXER_HPP
#define MIXER_HPP

#pragma once
#include "phys_mod.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <map>

class mixer
{
private:
    std::vector<phys_mod> screw;
    double t_0; // initial temperature of the fluid
    double p_0; // initial pressure of the fluid
    std::map<std::string, double> parameters;
    std::map<std::string, std::string> string_parameters;

public:
    // constructor
    mixer(size_t number_pm, size_t number_nn);

    // getters and setters
    std::vector<phys_mod> get_screw();

    // other functions
    void print_mixer();
    std::vector<std::string> get_parameters();

    // function where the model is simulated
    void set_parameters(double t0, double RPM, double Q, const std::string &type, const std::string &SRtype);
    void simulate_mixer();
};

#endif // MIXER_HPP
