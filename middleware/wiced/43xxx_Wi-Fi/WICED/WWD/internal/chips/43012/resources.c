#include "resources.h"
#include "43012.h"
#include "43012_clm.h"

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


