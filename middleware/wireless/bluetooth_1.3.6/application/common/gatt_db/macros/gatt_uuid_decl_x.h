/*
*
* Custom 128-bit UUIDs declaration
*
*/

#ifndef GATT_UUID_DECL_X_H
#define GATT_UUID_DECL_X_H

#define UUID128(name, ...)\
    extern uint8_t name[16];

#include "gatt_uuid128.h"

#undef UUID128

#endif /* GATT_UUID_DECL_X_H */
