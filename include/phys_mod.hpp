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
            // constructor
            interp_safe();
            interp();

        private:
        }

        // constructor

        void
        set_db();
        void __energy_balance();
        void __add_dtemp();
        void __add_dp();
        void __compute_phi();

        private;

    }

    class full_db
    {
    public:
        // constructor
    private:
    }

    void set_db;

private:
};
