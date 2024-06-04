#include <omp.h>

void mixer::simulate_mixer()
{
    // Set the initial temperature sequentially
    for (size_t i = 0; i < screw.size(); i++)
    {
        screw[i].set_tIn(parameters["t0"]);
    }

    int iter = 2000;
// Parallelize the outer loop
#pragma omp parallel for
    for (int i = 0; i < iter; i++)
    {
// Parallel region starts here
#pragma omp parallel sections
        {
#pragma omp section
            {
                // Iterate going backwards and update pressures
                for (int j = static_cast<int>(screw.size()) - 1; j >= 0; j--)
                {
                    screw[j].update_p();
                }
            }
#pragma omp section
            {
                // Update temperatures
                for (size_t j = 0; j < screw.size(); j++)
                {
                    screw[j].update_t();
                    if (j != screw.size() - 1)
                    {
                        screw[j + 1].set_tIn(screw[j].get_tOut());
                    }
                }
            }
        } // End of parallel sections
    }
}


