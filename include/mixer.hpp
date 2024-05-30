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

public:
    // constructor
    mixer(size_t number_pm, size_t number_nn);

    // other functions
    void print_mixer();

    // function where the model is simulated
    void simulate_mixer();
};

#endif // MIXER_HPP