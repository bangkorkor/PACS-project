#include "num_mod.hpp"

// constructor
num_mod::num_mod(int id) // inititaly sets temperature to 273 and pressure to 0
    : id(id)
{
}

// getters and setters
int num_mod::get_id() { return id; }

double num_mod::get_t() { return t; }

double num_mod::get_p() { return p; }

void num_mod::set_t(double t) { this->t = t; }

void num_mod::set_p(double p) { this->p = p; }

// other functions

void num_mod::print_num_mod()
{
    cout << "num_mod " << id << ", t: " << t << ", p: " << p << endl;
}
