#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"

int main()
{
    phys_mod model(40);

    model.simulate_model();

    model.print_model();

    return 0;
};