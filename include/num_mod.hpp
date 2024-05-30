#ifndef NUM_MOD_HPP
#define NUM_MOD_HPP

#pragma once

#include <iostream>

using namespace std;

class num_mod
{
private:
    const int id; // id of the model
    double t;     // temperature
    double p;     // pressure

public:
    // constructor
    num_mod(int id);

    // getters and setters
    int get_id();
    double get_t();
    double get_p();
    void set_t(double t);
    void set_p(double p);

    // other functions
    void print_num_mod();
};

#endif // NUM_MOD_HPP
