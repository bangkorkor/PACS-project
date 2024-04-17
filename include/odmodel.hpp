// odmodel.h
#ifndef ODMODEL_H
#define ODMODEL_H

#include <vector>
#include <string>
#include <functional>
#include <memory>

class odmodel
{
public:
    odmodel(const std::string &num_type, const std::vector<double> &points, odmodel *prev_mod, std::function<void()> phys_mod, int npl = 0, const std::string &plt = "", bool verbose = false);
    void set_db(const std::string &df);

private:
    bool verbose;
    std::string type;
    int nplanets;
    std::string planet_type;
    std::vector<double> pts;
    double dx;
    double u;                       // velocity
    double T;                       // Temperature
    double phi;                     // filling factor
    double gradp;                   // pressure gradient
    double tot_gradp;               // total pressure gradient
    double p;                       // pressure
    double qt;                      // local viscous heating
    double q0;                      // local natural flux
    odmodel *prev;                  // Pointer to the previous module
    odmodel *next;                  // Pointer to the next module
    std::function<void()> phys_mod; // Function to update physical model

    std::function<void()> update_T;
    std::function<void()> update_p;
    std::function<void()> update_phi;
};

#endif // ODMODEL_H
