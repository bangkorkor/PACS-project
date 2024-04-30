#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <Eigen/Dense>
#include <gsl/gsl_spline.h>
#include <limits>

// Include the header where ODModel is declared.
#include "odmodel.hpp"

class odmodel::phys_mod
{
public:
    class num_mod
    {
    public:
        class interpolator_op // NOT COMPLETE, I DONT UNDERSTAND THIS, see comments at phys_mod.cpp for more
        {
        public:
            interpolator_op(const Eigen::MatrixXd &data); // Constructor
            ~interpolator_op();                           // Destructor
            double interp(std::size_t varIndex, double point);
            double interp_safe(int varIndex, double point);

        private:
            Eigen::MatrixXd coord;
            std::vector<gsl_spline *> splines;
            std::vector<gsl_interp_accel *> accels;
            std::vector<std::string> coordLabels = {"TIn", "RPM", "MassFlowRate", "TSun", "TRing"};
        };

        num_mod(std::string num_type, Eigen::VectorXd points, NumMod *prev_mod, bool verbose = false); // Constructor

        void set_db(const Eigen::MatrixXd &df);
        void energy_balance(double t_prev);
        void add_dtemp(double t_prev);
        void add_dp(double p_prev);
        void compute_phi();

    private:
        bool verbose;
        std::string type; // type of numerical module: mixer or stopring
        int nplanets;
        std::string planet_type;
        Eigen::VectorXd pts;                               // points associated to the numerical module
        double dx, u, T, phi, gradp, tot_gradp, p, qt, q0; // see python file for explanation
        odmodel::phys_mod::num_mod *prev;
        odmodel::phys_mod::num_mod *next;
        odmodel::phys_mod::num_mod::interpolator_op *interpolator;
        std::map<std::string, std::function<void(double)>> update_T;
        std::function<void(double)> update_p;
        std::function<void()> update_phi;
    };

private:
    std::string name = "phys_mod";
};
