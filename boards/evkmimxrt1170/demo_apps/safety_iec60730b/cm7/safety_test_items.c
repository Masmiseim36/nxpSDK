/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".safety_ram"
#endif

#define MUX_ADDR(arg1)                          MUX_ADDR_(arg1) /* wrapper */
#define MUX_ADDR_(arg1, arg2, arg3, arg4, arg5) arg1            /* get 1st value from macro */
#define PAD_ADDR(arg1)                          PAD_ADDR_(arg1) /* wrapper */
#define PAD_ADDR_(arg1, arg2, arg3, arg4, arg5) arg5            /* get 5th value from macro */

/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR DIO Initialization and TEST          *
 *                                                                              *
 *******************************************************************************/
fs_dio_test_imx_t dio_safety_test_item_0 = {
    .gpio          = GPIO9_BASE,
    .pinNum        = 4U,
    .pinDir        = PIN_DIRECTION_IN,
    .muxAddr       = MUX_ADDR(IOMUXC_GPIO_AD_05_GPIO9_IO04),
    .padAddr       = PAD_ADDR(IOMUXC_GPIO_AD_05_GPIO9_IO04),
    .pullUpMask    = (IOMUXC_SW_PAD_CTL_PAD_PUS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK), /* Mask for enabling pull-up. */
    .pullDownMask  = (IOMUXC_SW_PAD_CTL_PAD_PUE_MASK), /* Mask for enabling pull-down. */
    .pullResetMask = (IOMUXC_SW_PAD_CTL_PAD_PUS_MASK |
                      IOMUXC_SW_PAD_CTL_PAD_PUE_MASK) /* Mask for reset pull/keeper settings.
                                                       * Will be negated and used to clear selected bits. */
};

fs_dio_test_imx_t dio_safety_test_item_1 = {
    .gpio          = GPIO9_BASE,
    .pinNum        = 13U,
    .pinDir        = PIN_DIRECTION_IN,
    .muxAddr       = MUX_ADDR(IOMUXC_GPIO_AD_14_GPIO9_IO13),
    .padAddr       = PAD_ADDR(IOMUXC_GPIO_AD_14_GPIO9_IO13),
    .pullUpMask    = (IOMUXC_SW_PAD_CTL_PAD_PUS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK), /* Mask for enabling pull-up. */
    .pullDownMask  = (IOMUXC_SW_PAD_CTL_PAD_PUE_MASK), /* Mask for enabling pull-down. */
    .pullResetMask = (IOMUXC_SW_PAD_CTL_PAD_PUS_MASK |
                      IOMUXC_SW_PAD_CTL_PAD_PUE_MASK) /* Mask for reset pull/keeper settings.
                                                       * Will be negated and used to clear selected bits. */
};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_imx_t *dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};
