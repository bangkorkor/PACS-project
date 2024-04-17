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
    odmodel(std::string num_type, std::vector<double> points, odmodel *prev_mod, std::function<void(odmodel &)> phys_mod,
            int npl = 0, std::string plt = "", bool verbose = false);

    virtual ~odmodel();

    // Getters
    int getNplanets() const { return nplanets; }
    std::string getPlanetType() const { return planet_type; }
    std::vector<double> getPoints() const { return pts; }

private:
    bool verbose;
    std::string type;
    int nplanets;
    std::string planet_type;
    std::vector<double> pts;
    double dx;                               // Change in position between points
    double u;                                // velocity
    double T;                                // Temperature
    double phi;                              // filling factor
    double gradp;                            // pressure gradient
    double tot_gradp;                        // total pressure gradient
    double p;                                // pressure
    double qt;                               // local viscous heating
    double q0;                               // local natural flux
    odmodel *prev;                           // pointer to the previous module
    odmodel *next;                           // pointer to the next module
    std::function<void(odmodel &)> phys_mod; // Function to update physics

    // Function pointers or std::function for updates
    std::function<void()> update_T;
    std::function<void()> update_p;
    std::function<void()> update_phi;
};

#endif // ODMODEL_H
