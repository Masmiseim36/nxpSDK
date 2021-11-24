/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sln_device_utils.h"
#include "stdint.h"
#include "string.h"

bool hasSpecialCharacters(const char str[])
{
    uint32_t length;

    if (str != NULL)
    {
        length = strlen(str);
        for (int i = 0; i < length; i++)
        {
            if ((!isdigit((int)str[i])) && (!isalpha((int)str[i])) && (str[i] != '-') && (str[i] != '_'))
                return true;
        }
    }
    return false;
}
