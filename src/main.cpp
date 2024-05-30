#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"

int main()
{
    phys_mod pm(40);
    pm.simulate_phys_mod();
    pm.print_phys_mod();

    return 0;
};