#ifndef MIXER_HPP
#define MIXER_HPP

#pragma once
#include "phys_mod.hpp"
#include <vector>
#include <iostream>

class mixer
{
private:
    vector<phys_mod> screw;
    double t_0 = 293; // initial temperature of the fluid
    double p_0 = 0;   // initial pressure of the fluid

public:
    // constructor
    mixer(size_t number_pm, size_t number_nn);

    // other functions
    void print_mixer();

    // function where the model is simulated
    void simulate_mixer();
};

#endif // MIXER_HPP