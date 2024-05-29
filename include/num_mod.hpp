#ifndef NUM_MOD_HPP
#define NUM_MOD_HPP

#pragma once

#include <iostream>

using namespace std;

class num_mod
{
private:
    const int id;  // id of the model
    double t;      // temperature
    double p;      // pressure
    num_mod *prev; // pointer to previous model
    num_mod *next; // pointer to next model

public:
    // constructor
    num_mod(int id);

    // getters and setters
    int get_id();
    double get_t();
    double get_p();
    void set_t(double t);
    void set_p(double p);
    void set_prev(num_mod *prev);
    void set_next(num_mod *next);

    // other functions
    void print_num_mod();
};

#endif // NUM_MOD_HPP
