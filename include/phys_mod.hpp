#pragma once
#include <vector>
#include <functional>
#include <iostream>

// Include the header where ODModel is declared.
#include "odmodel.hpp"

class odmodel::phys_mod
{
public:
    class num_mod
    {
    public:
        class interpolator_op
        {
        public:
            void printtest();

        private:
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
