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
    double RPM;            // RPM of the ring in the phys_mod, this is the same for all the num_mods in the given phys_mod
    double Q;              // flow rate in the phys_mod, this is the same for all the num_mods in the given phys_mod
    double tIn;            // temperature of the incoming fluid

public:
    // constructor
    phys_mod(size_t size);

    // getters and setters
    void set_tIn(double tIn);
    double get_tIn();
    double get_tOut();
    void set_RPM(double RPM);
    double get_RPM();
    void set_Q(double Q);
    double get_Q();
    vector<num_mod> get_model(); // getter for the model, this is used in visualize

    // other functions
    void make_model(size_t n);
    void print_phys_mod();

    // function where the model is simulated
    void simulate_phys_mod();
    void update_p();
    void update_t();
};

#endif // PHYS_MOD_HPP
