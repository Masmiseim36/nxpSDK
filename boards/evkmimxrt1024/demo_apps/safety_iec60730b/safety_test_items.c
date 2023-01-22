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
    .gpio       = GPIO1_BASE,
    .pinNum     = 14U,
    .pinDir     = PIN_DIRECTION_IN,
    .muxAddr    = MUX_ADDR(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14),
    .padAddr    = PAD_ADDR(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14),
    .pullUpMask = (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0x3) |
                   IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)), /* Mask for enabling pull-up. */
    .pullDownMask =
        (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)), /* Mask for enabling pull-down. */
    .pullResetMask = (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0x3) |
                      IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)) /* Mask for reset pull/keeper settings.
                                                       * Will be negated and used to clear selected bits. */
};

fs_dio_test_imx_t dio_safety_test_item_1 = {
    .gpio       = GPIO1_BASE,
    .pinNum     = 23U,
    .pinDir     = PIN_DIRECTION_IN,
    .muxAddr    = MUX_ADDR(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23),
    .padAddr    = PAD_ADDR(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23),
    .pullUpMask = (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0x3) |
                   IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)), /* Mask for enabling pull-up. */
    .pullDownMask =
        (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)), /* Mask for enabling pull-down. */
    .pullResetMask = (IOMUXC_SW_PAD_CTL_PAD_PKE(0x1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0x3) |
                      IOMUXC_SW_PAD_CTL_PAD_PUE(0x1)) /* Mask for reset pull/keeper settings.
                                                       * Will be negated and used to clear selected bits. */
};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_imx_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};


/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR AIO TEST                             *
 *                                                                              *
 *******************************************************************************/
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_MIN_LIMIT(val)      (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)

fs_aio_test_a2346_t aio_safety_test_item_VL =
{
  .AdcChannel = 0xBU,     
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(0),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(60),
  .state = FS_AIO_INIT
};

fs_aio_test_a2346_t aio_safety_test_item_VH =
{
  .AdcChannel = 0x19U,
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_MAX),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_MAX),
  .state = FS_AIO_INIT
};

fs_aio_test_a2346_t aio_safety_test_item_BG =
{
  .AdcChannel = 0xAU,
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .state = FS_AIO_INIT
};

/* NULL terminated array of pointers to fs_aio_test_a2346_t items for safety AIO test */
fs_aio_test_a2346_t *g_aio_safety_test_items[] = {&aio_safety_test_item_VL,
                                                &aio_safety_test_item_VH,
                                                &aio_safety_test_item_BG,
                                                NULL};
