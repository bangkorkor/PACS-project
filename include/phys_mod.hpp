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
    double tIn = 293;      // temperature of the incoming fluid

public:
    // constructor
    phys_mod(size_t size);

    // getters and setters
    void set_tIn(double tIn);
    double get_tIn();
    double get_tOut();

    // other functions
    void make_model(size_t n);
    void print_phys_mod();

    // function where the model is simulated
    void simulate_phys_mod();
    void update_p();
    void update_t();
};

#endif // PHYS_MOD_HPP
