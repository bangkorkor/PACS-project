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

// setters
void phys_mod::set_tIn(double tIn) { this->tIn = tIn; }
void phys_mod::set_RPM(double RPM) { this->RPM = RPM; }
void phys_mod::set_Q(double Q) { this->Q = Q; }
void phys_mod::set_type(string type) { this->type = type; }
void phys_mod::set_SRtype(string SRtype) { this->SRtype = SRtype; }

// getters
double phys_mod::get_tIn() const { return tIn; }
double phys_mod::get_tOut() { return model[model.size() - 1].get_t(); }
double phys_mod::get_RPM() const { return RPM; }
double phys_mod::get_Q() const { return Q; }
string phys_mod::get_type() const { return type; }
string phys_mod::get_SRtype() const { return SRtype; }
vector<num_mod> phys_mod::get_model() const { return model; }

// print functions for debugging and testing
void phys_mod::print_phys_mod()
{
    cout << "Printing model:" << endl;
    for (size_t i = 0; i < model.size(); i++)
    {
        model[i].print_num_mod();
    }
    cout << endl;
}

// functions for calculating used in update_p and update_t
double phys_mod::dt_type1(double RPM, double Q, double T) const
{
    return 4 * RPM / Q * (T / 500);
}

double phys_mod::dt_type2(double RPM, double Q, double T) const
{
    return 5 * RPM / Q * exp((300 - T) / 20) + T / 700;
}

double phys_mod::dt_type3(double RPM, double Q, double T) const
{
    return 5 * RPM / Q * exp((300 - T) / 20);
}

double phys_mod::dp_type1(double RPM, double Q, double T) const
{
    return (10000 * RPM + 2000 * (250 - Q)) * ((470 - T) / 150);
}

double phys_mod::dp_type2(double RPM, double Q, double T) const
{
    return (1000 * RPM + 800 * (250 - Q)) * ((450 - T) / 150);
}

double phys_mod::dp_type3(double RPM, double Q, double T) const
{
    return (10000 * RPM + 2000 * (250 - Q)) * ((450 - T) / 150);
}

double phys_mod::dp_SR(double RPM, double Q, double T) const
{
    return (30000 * (200 - RPM) + 10000 * Q) * ((500 - T) / 150);
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

            new_p = dp_SR(RPM, Q, model[i].get_t());
        }
        else
        {
            if (type == "Type1")
            {
                new_p = model[i + 1].get_p() - dp_type1(RPM, Q, model[i].get_t());
            }
            else if (type == "Type2")
            {
                new_p = model[i + 1].get_p() - dp_type2(RPM, Q, model[i].get_t());
            }
            else if (type == "Type3")
            {
                new_p = model[i + 1].get_p() - dp_type3(RPM, Q, model[i].get_t());
            }
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

        if (i == 0) // sets tIn for the fist num_mod in the phys_mod
        {
            if (type == "Type1")
            {
                new_t = tIn + dt_type1(RPM, Q, model[i].get_t());
            }
            else if (type == "Type2")
            {
                new_t = tIn + dt_type3(RPM, Q, model[i].get_t());
            }
            else if (type == "Type3")
            {
                new_t = tIn + dt_type3(RPM, Q, model[i].get_t());
            }
            model[i].set_t(new_t);
        }
        else
        {
            if (type == "Type1")
            {
                new_t = model[i - 1].get_t() + dt_type1(RPM, Q, model[i].get_t());
            }
            else if (type == "Type2")
            {
                new_t = model[i - 1].get_t() + dt_type2(RPM, Q, model[i].get_t());
            }
            else if (type == "Type3")
            {
                new_t = model[i - 1].get_t() + dt_type3(RPM, Q, model[i].get_t());
            }
        }
        // temperature change in Stopring
        if (i == model.size() - 1)
        {
            if (SRtype == "Special")
            {
                new_t = model[i - 1].get_t() + 0.01 * model[i].get_t();
            }
            // if SRtype is Standard, do nothing
        }
        model[i].set_t(new_t);
    }
}
