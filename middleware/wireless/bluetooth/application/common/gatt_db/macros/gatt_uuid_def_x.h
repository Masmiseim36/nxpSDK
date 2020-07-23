/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
* 
* Custom 128-bit UUIDs allocation
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef GATT_UUID_DEF_X_H
#define GATT_UUID_DEF_X_H

#define UUID128(name, ...)\
    uint8_t name[16] = { __VA_ARGS__ };

#include "gatt_uuid128.h"

#undef UUID128

#endif /* GATT_UUID_DEF_X_H */
