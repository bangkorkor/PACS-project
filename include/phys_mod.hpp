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
        void set_db();
        void __energy_balance();
        void __add_dtemp();
        void __add_dp();
        void __compute_phi();

    private:
    };

    // Constructor
    phys_mod();

    void printSomething()
    {
        std::cout << "Printing something" << std::endl;
    }

private:
    std::string name = "phys_mod";
};
