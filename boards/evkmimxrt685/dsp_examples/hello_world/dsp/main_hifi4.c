/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <xtensa/config/core.h>

int main(int argc, char **argv)
{
    printf("Hello World running on core %s\n", XCHAL_CORE_ID);

    return 0;
}
