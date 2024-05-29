#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"

int main()
{
    phys_mod pm;
    pm.print_phys_mod();

    num_mod nm;
    nm.print_num_mod();

    mixer mx;
    mx.print_mixer();

    return 0;
};