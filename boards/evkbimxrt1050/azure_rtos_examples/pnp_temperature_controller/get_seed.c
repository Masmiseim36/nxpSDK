#include "fsl_trng.h"
#include "fsl_common.h"

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

uint32_t get_seed(void)
{
    uint32_t random_val;
    trng_config_t trngConfig;

    TRNG_GetDefaultConfig(&trngConfig);
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
    TRNG_Init(TRNG, &trngConfig);

    /* delay */
    delay();

    TRNG_GetRandomData(TRNG, (void *)&random_val, sizeof(random_val));

    return random_val;
}
