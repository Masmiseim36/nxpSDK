/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/

#include "safety_config.h"
#include "iec60730b.h"
#include "safety_cm4_cm7_imxrt.h"

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
dio_test_rt_t dio_safety_test_item_0 =
{
  .gpio   = GPIO1_BASE,
  .pinNum = 20,
  .pinDir = PIN_DIRECTION_IN,
  .muxAddr = MUX_ADDR(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20),
  .padAddr = PAD_ADDR(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20),
};

dio_test_rt_t dio_safety_test_item_1 =
{
  .gpio   = GPIO1_BASE,
  .pinNum = 21,
  .pinDir = PIN_DIRECTION_IN,
  .muxAddr = MUX_ADDR(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21),
  .padAddr = PAD_ADDR(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21),
};
 
/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
dio_test_rt_t *dio_safety_test_items[] = { &dio_safety_test_item_0, &dio_safety_test_item_1, NULL };
