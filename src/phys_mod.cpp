#include "phys_mod.hpp"
#include <cmath>

// constructor
phys_mod::phys_mod(size_t size) : RPM(100), Q(150)
{
    model.reserve(size); // Reserve the necessary space to prevent reallocations
    for (size_t i = 0; i < size; i++)
    {
        model.emplace_back(i); // Construct num_mod objects in-place. emplace_back constructs the num_mod objects directly within the space allocated by the vector.
        if (i > 0)
        {
            model[i].set_prev(&model[i - 1]);
            model[i - 1].set_next(&model[i]);
        }
    }
}

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
    for (size_t i = 0; i < 50; i++)
    {
        update_t();
    }
    for (size_t i = 0; i < 1; i++)
    {
        update_p();
    }
}

// function for updating pressure
void phys_mod::update_p()
{

    // starting from the last element, going backwards
    for (int i = model.size(); i >= 0; i--)
    {
        double new_p = 0;                            // new computed pressure for the given num_mod
        if (i == static_cast<int>(model.size()) - 1) // STOPRING! the last element of the phys_mod is the stopring
        {
            new_p = (20000 * (200 - RPM) + 10000 * Q) * ((450 - model[i].get_t()) / 150);
        }
        else
        {
            new_p = model[i + 1].get_p() - (10000 * RPM + 2000 * (250 - Q)) * ((450 - model[i].get_t()) / 150);
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
    double tIn = 293; // initial temperature
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
