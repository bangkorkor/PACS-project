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

void phys_mod::set_type(string type) { this->type = type; }

string phys_mod::get_type() { return type; }

void phys_mod::set_SRtype(string SRtype) { this->SRtype = SRtype; }

string phys_mod::get_SRtype() { return SRtype; }

vector<num_mod> phys_mod::get_model() { return model; }

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

// functions for calculating used in update_p and update_t
double phys_mod::calculate_temperature_henrik(double RPM, double Q, double T)
{
    return 5 * RPM / Q * exp((300 - T) / 20);
}

double phys_mod::calculate_temperature_bharat(double RPM, double Q, double T)
{
    return 5 * RPM / Q * (exp((300 - T) / 20) + 0.01 * T);
}

double phys_mod::calculate_temperature_ask(double RPM, double Q, double T)
{
    return 5 * RPM / Q * exp((300 - T) / 25);
}

double phys_mod::calculate_pressure_henrik(double RPM, double Q, double T)
{
    return (10000 * RPM + 2000 * (250 - Q)) * ((450 - T) / 150);
}

double phys_mod::calculate_pressure_bharat(double RPM, double Q, double T)
{
    return (8000 * RPM + 1500 * (300 - Q) + 500 * pow(Q, 2)) * ((500 - pow(T, 2)) / 2);
}

double phys_mod::calculate_pressure_ask(double RPM, double Q, double T)
{
    return (10000 * std::log(RPM + 1) + 2500 * std::sin(300 - Q) + 1000 * std::exp(Q / 100) + 500 * std::sinh(Q / 50)) *
           ((std::cos(M_PI * T / 450) / 150) + std::cosh(T / 100) + std::tanh(T / 100));
}

double phys_mod::calculate_SR_pressure_standard(double RPM, double Q, double T)
{
    return (20000 * (200 - RPM) + 10000 * Q) * ((450 - T) / 150);
}

double phys_mod::calculate_SR_pressure_extreme(double RPM, double Q, double T)
{
    return (20000 * (200 - RPM) + 10000 * Q) * ((450 - T) / 69);
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
            if (SRtype == "Standard")
            {
                new_p = calculate_SR_pressure_standard(RPM, Q, model[i].get_t());
            }
            else if (SRtype == "Extreme")
            {
                new_p = calculate_SR_pressure_extreme(RPM, Q, model[i].get_t());
            }
        }
        else
        {
            if (type == "Henrik")
            {
                new_p = model[i + 1].get_p() - calculate_pressure_henrik(RPM, Q, model[i].get_t());
            }
            else if (type == "Bharat")
            {
                new_p = model[i + 1].get_p() - calculate_pressure_bharat(RPM, Q, model[i].get_t());
            }
            else if (type == "Ask")
            {
                new_p = model[i + 1].get_p() - calculate_pressure_ask(RPM, Q, model[i].get_t());
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
            if (type == "Henrik")
            {
                new_t = tIn + calculate_temperature_henrik(RPM, Q, model[i].get_t());
            }
            else if (type == "Bharat")
            {
                new_t = tIn + calculate_temperature_bharat(RPM, Q, model[i].get_t());
            }
            else if (type == "Ask")
            {
                new_t = tIn + calculate_temperature_ask(RPM, Q, model[i].get_t());
            }
            model[i].set_t(new_t);
        }
        else
        {
            if (type == "Henrik")
            {
                new_t = model[i - 1].get_t() + calculate_temperature_henrik(RPM, Q, model[i].get_t());
            }
            else if (type == "Bharat")
            {
                new_t = model[i - 1].get_t() + calculate_temperature_bharat(RPM, Q, model[i].get_t());
            }
            else if (type == "Ask")
            {
                new_t = model[i - 1].get_t() + calculate_temperature_ask(RPM, Q, model[i].get_t());
            }
        }
        // temperature change in Stopring
        if (i == model.size() - 1)
        {
            if (SRtype == "Extreme")
            {
                new_t = model[i - 1].get_t() + 1000 / model[i].get_t();
            }
            // if SRtype is Standard, do nothing
        }
        model[i].set_t(new_t);
    }
}
