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
    string type;           // type of the phys_mod, what type of formula
    string SRtype;         // type of the SR, what type o formula for Stopring

public:
    // constructor
    phys_mod(size_t size);

    // setters
    void set_tIn(double tIn);
    void set_RPM(double RPM);
    void set_Q(double Q);
    void set_type(string type);
    void set_SRtype(string SRtype);

    // getters
    double get_tIn() const;
    double get_tOut();
    double get_RPM() const;
    double get_Q() const;
    string get_type() const;
    string get_SRtype() const;
    vector<num_mod> get_model() const; // getter for the model, this is used in visualize

    double dp_type1(double RPM, double Q, double T) const;
    double dp_type2(double RPM, double Q, double T) const;
    double dp_type3(double RPM, double Q, double T) const;
    double dp_SR(double RPM, double Q, double T) const;
    double dt_type1(double RPM, double Q, double T) const;
    double dt_type3(double RPM, double Q, double T) const;
    double dt_type2(double RPM, double Q, double T) const;

    // other functions
    void make_model(size_t n);
    void print_phys_mod();

    // function where the model is simulated
    void update_p();
    void update_t();
};

#endif // PHYS_MOD_HPP
