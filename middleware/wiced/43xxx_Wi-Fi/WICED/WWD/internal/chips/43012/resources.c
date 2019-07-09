#include "resources.h"

#if defined(WICED_MFG_TEST_APP)
#include "43012_mfg.h"
#include "43012_mfg_clm.h"
#else
#include "43012.h"
#include "43012_clm.h"
#endif

const resource_hnd_t wifi_firmware_image = {
    .location = RESOURCE_IN_MEMORY,
    .val.mem.data = &__43012_bin[0],
    .size = sizeof(__43012_bin)
};

const resource_hnd_t wifi_firmware_clm_blob = {
    .location = RESOURCE_IN_MEMORY,
    .val.mem.data = &__43012_clm_blob[0],
    .size = sizeof(__43012_clm_blob)
};


