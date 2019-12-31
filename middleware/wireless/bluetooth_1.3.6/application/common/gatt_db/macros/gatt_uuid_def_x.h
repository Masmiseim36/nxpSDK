/*
*
* Custom 128-bit UUIDs allocation
*
*/

#ifndef GATT_UUID_DEF_X_H
#define GATT_UUID_DEF_X_H

#define UUID128(name, ...)\
    uint8_t name[16] = { __VA_ARGS__ };

#include "gatt_uuid128.h"

#undef UUID128

#endif /* GATT_UUID_DEF_X_H */
