/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DEMO_INFO_H_
#define _DEMO_INFO_H_

#include <stdio.h>

#include "demo_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CHECK_STATUS(STATUS, FUNCTION, MESSAGE)                  \
    (STATUS) = (FUNCTION);                                       \
    if ((STATUS) != 0)                                           \
    {                                                            \
        printf(MESSAGE EOL "Error code: %d" EOL, (int)(STATUS)); \
    }                                                            \
    do                                                           \
    {                                                            \
        ;                                                        \
    } while (STATUS)

void DEMO_PrintInfo(void);

#endif /* _DEMO_INFO_H_ */
