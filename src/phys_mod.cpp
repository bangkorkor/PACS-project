#include "phys_mod.hpp"
#include <cmath>

// constructor
phys_mod::phys_mod(size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        model.push_back(num_mod(i));
    }
}

// getters and setters
void phys_mod::set_tIn(double tIn) { this->tIn = tIn; }

double phys_mod::get_tIn() { return tIn; }

double phys_mod::get_tOut() { return model[model.size() - 1].get_t(); }

void phys_mod::set_RPM(double RPM) { this->RPM = RPM; }

double phys_mod::get_RPM() { return RPM; }

void phys_mod::set_Q(double Q) { this->Q = Q; }

double phys_mod::get_Q() { return Q; }

// Other functions
void phys_mod::print_phys_mod()
{
    cout << "Printing model:" << endl;
    for (size_t i = 0; i < model.size(); i++)
    {
        model[i].print_num_mod();
    }
    cout << endl;
}

// function for simulating the model
void phys_mod::simulate_phys_mod()
{
    size_t iterations = 10;
    for (size_t i = 0; i < iterations; i++)
    {
        update_p();
        update_t();
    }
}

// function for updating pressure
void phys_mod::update_p()
{

    // starting from the last element, going backwards
    for (int i = model.size() - 1; i >= 0; i--)
    {
        double new_p = 0;                            // new computed pressure for the given num_mod
        if (i == static_cast<int>(model.size()) - 1) // STOPRING! the last element of the phys_mod is the stopring
        {
            new_p = (20000 * (200 - RPM) + 10000 * Q) * ((450 - model[i].get_t()) / 150);
        }
        else
        {
            new_p = model[i + 1].get_p() - (800 * RPM + 500 * (250 - Q)) * ((450 - model[i].get_t()) / 150);
        }
        if (new_p < 0)
        {
            new_p = 0;
        }
        model[i].set_p(new_p);
    }
}

// function for updating temperature
void phys_mod::update_t()
{

    // starting from the first element, going forwards
    for (size_t i = 0; i < model.size(); i++)
    {
        double new_t = 0; // new computed temperature for the given num_mod
        if (i == 0)
        {
            new_t = tIn + 5 * RPM / Q * exp((300 - model[i].get_t()) / 20);
            model[i].set_t(new_t);
        }
        else
        {
            new_t = model[i - 1].get_t() + 5 * RPM / Q * exp((300 - model[i].get_t()) / 20);
            model[i].set_t(new_t);
        }
    }
}
