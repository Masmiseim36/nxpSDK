/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"
#include "iec60730b.h"
#include "safety_cm7_imxrt.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *) 0)
#endif
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location =  ".safety_ram"
#endif

#define MUX_ADDR(arg1) MUX_ADDR_(arg1) /* wrapper */
#define MUX_ADDR_(arg1, arg2, arg3, arg4, arg5) arg1 /* get 1st value from macro */
#define PAD_ADDR(arg1) PAD_ADDR_(arg1) /* wrapper */
#define PAD_ADDR_(arg1, arg2, arg3, arg4, arg5) arg5 /* get 5th value from macro */

/*******************************************************************************
*                                                                              *
*                           STRUCTURE FOR DIO Initialization and TEST          *
*                                                                              *
*******************************************************************************/
fs_dio_test_rt_t dio_safety_test_item_0 =
{
  .gpio   = GPIO1_BASE,
  .pinNum = 20,
  .pinDir = PIN_DIRECTION_IN,
  .muxAddr = MUX_ADDR(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20),
  .padAddr = PAD_ADDR(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20),
};

fs_dio_test_rt_t dio_safety_test_item_1 =
{
  .gpio   = GPIO1_BASE,
  .pinNum = 21,
  .pinDir = PIN_DIRECTION_IN,
  .muxAddr = MUX_ADDR(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21),
  .padAddr = PAD_ADDR(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21),
};
 
/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_rt_t *dio_safety_test_items[] = { &dio_safety_test_item_0, &dio_safety_test_item_1, NULL };
