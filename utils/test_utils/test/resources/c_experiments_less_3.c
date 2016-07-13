#include "platform.h"
#include "c_experiments.h"

#define NUM_SEEDS                           (10u)
#define NUM_VALS                            (10u)

int main(void)
{
    INT16_F vals[NUM_SEEDS][NUM_VALS] = {{0}};
    INT16_F new_vals[NUM_SEEDS][NUM_VALS] = {{0}};
    UINT16_F i;
    UINT16_F seed;

    for (seed = 0; seed < 10; seed++)
    {
        pltfm_srand(seed);
        for (i = 0; i < 10; i++)
        {
            vals[seed][i] = pltfm_rand();
        }
    }

    for (seed = 0; seed < 10; seed++)
    {
        pltfm_srand(seed);
        for (i = 0; i < 10; i++)
        {
            new_vals[seed][i] = pltfm_rand();
        }
    }

