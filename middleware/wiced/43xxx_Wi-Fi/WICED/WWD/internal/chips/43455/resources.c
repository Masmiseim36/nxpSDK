#include <chips/43455/resources.h>

#if defined(WICED_MFG_TEST_APP)
#include <chips/43455/43455_mfg.h>
#include <chips/43455/43455_mfg_clm.h>
#else
#include <chips/43455/43455.h>
#include <chips/43455/43455_clm.h>
#endif

const resource_hnd_t wifi_firmware_image = {
    .location = RESOURCE_IN_MEMORY,
    .val.mem.data = &__43455_bin[0],
    .size = sizeof(__43455_bin)
};

const resource_hnd_t wifi_firmware_clm_blob = {
    .location = RESOURCE_IN_MEMORY,
    .val.mem.data = &__43455_clm_blob[0],
    .size = sizeof(__43455_clm_blob)
};


