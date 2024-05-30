#include <iostream>
#include "phys_mod.hpp"
#include "num_mod.hpp"
#include "mixer.hpp"

int main()
{
    mixer m(2, 40);
    m.simulate_mixer();
    m.print_mixer();
    return 0;
};