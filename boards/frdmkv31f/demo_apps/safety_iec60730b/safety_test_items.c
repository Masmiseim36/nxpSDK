/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

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

/*******************************************************************************
*                                                                              *
*                           STRUCTURE FOR DIO Initialization and TEST          *
*                                                                              *
*******************************************************************************/

fs_dio_test_t dio_safety_test_item_0 =
{
  .gpio   = GPIOC_BASE,
  .pcr    = PORTC_BASE,   /* Base address of PCR register */
  .pinNum = 5,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_1 =
{
  .gpio   = GPIOB_BASE,
  .pcr    = PORTB_BASE,   /* Base address of PCR register */
  .pinNum = 18,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

/* NULL terminated array of pointers to fs_dio_test_t items for safety DIO test */
fs_dio_test_t *dio_safety_test_items[] = { &dio_safety_test_item_0, &dio_safety_test_item_1, NULL };
