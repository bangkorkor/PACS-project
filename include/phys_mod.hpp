#ifndef PHYS_MOD_HPP
#define PHYS_MOD_HPP

#pragma once

#include <iostream>
#include <vector>
#include "num_mod.hpp"

using namespace std;

class phys_mod
{
private:
    vector<num_mod> model; //  vector of num_mods
    const double RPM;      // RPM of the ring in the phys_mod, this is the same for all the num_mods in the given phys_mod
    const double Q;        // flow rate in the phys_mod, this is the same for all the num_mods in the given phys_mod

public:
    // constructor
    phys_mod(size_t size);

    // other functions
    void make_model(size_t n);
    void print_model();
    num_mod &operator[](size_t i); // retuns the num_mod at index i using [] operator

    // function where the model is simulated
    void simulate_model();
    void update_p();
    void update_t();
};

#endif // PHYS_MOD_HPP
