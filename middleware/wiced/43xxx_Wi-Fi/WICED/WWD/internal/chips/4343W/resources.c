#include "resources.h"

#if defined(WICED_MFG_TEST_APP)
#include "4343WA1_mfg.h"
#include "4343WA1_clm_mfg.h"
#else
#include "4343WA1.h"
#include "4343WA1_clm.h"
#endif

const resource_hnd_t wifi_firmware_image = {
    .location = RESOURCE_IN_MEMORY,
    .val.mem.data = &__4343WA1_bin[0],
    .size = sizeof(__4343WA1_bin)
};

const resource_hnd_t wifi_firmware_clm_blob = {
	.location = RESOURCE_IN_MEMORY,
	.val.mem.data = &__4343WA1_clm_blob[0],
	.size = sizeof(__4343WA1_clm_blob)
};


