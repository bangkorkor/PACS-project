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

// function for getting num_mod at index i unsing [] operator
num_mod &phys_mod::operator[](size_t i)
{
    return model[i];
}

void phys_mod::print_model()
{
    cout << "Printing model:" << endl;
    for (size_t i = 0; i < model.size(); i++)
    {
        model[i].print_num_mod();
    }
    cout << endl;
}

// function for simulating the model
void phys_mod::simulate_model()
{
    for (size_t i = 0; i < 50; i++)
    {
        update_t();
    }
    for (size_t i = 0; i < 100; i++)
    {
        update_p();
    }
}

// function for updating pressure
void phys_mod::update_p()
{
    double pOut = 1600000; // initial pressure, this is the pressure of the last +1 num_mod
    // starting from the last element, going backwards
    for (int i = model.size(); i >= 0; i--)
    {
        double new_p = 0; // new computed pressure for the given num_mod
        if (i == static_cast<int>(model.size()) - 1)
        {
            new_p = pOut;
        }

        else
        {
            new_p = model[i + 1].get_p() - (10000 * RPM + 2000 * (250 - Q)) * ((450 - model[i].get_t()) / 150);
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
