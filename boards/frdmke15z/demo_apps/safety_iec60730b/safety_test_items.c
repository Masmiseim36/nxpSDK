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
*                           STRUCTURE FOR TSI TEST                             *
*                                                                              *
*******************************************************************************/

/*  Self cap TSI input */
fs_tsi_t tsi_safety_test_item_0 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL0),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL0),
  .limits.delta =  TSI_DELTA(TSI_VAL0, TSI_SVAL0),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL0, TSI_SVAL0)*TSI_MIN_DELTA_PERCENT/100,
  .input.rxCH  =  TSI0_CH0, /*TSI_CHx*/
  .input.txCH  =  SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOE_BASE,
  .gpioInput.rxPcr = PORTE_BASE,
  .gpioInput.rxPinNum = 5,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_1 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL1),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL1),
  .limits.delta =  TSI_DELTA(TSI_VAL1, TSI_SVAL1),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL1, TSI_SVAL1)*TSI_MIN_DELTA_PERCENT/100,  
  .input.rxCH  =  TSI0_CH1, /*TSI_CHx*/
  .input.txCH  = SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOE_BASE,
  .gpioInput.rxPcr = PORTE_BASE,
  .gpioInput.rxPinNum = 4,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,  
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
};

fs_tsi_t tsi_safety_test_item_2 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL2),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL2),
  .limits.delta =  TSI_DELTA(TSI_VAL2, TSI_SVAL2),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL2, TSI_SVAL2)*TSI_MIN_DELTA_PERCENT/100,  
  .input.rxCH  =  TSI0_CH9, /*TSI_CHx*/
  .input.txCH  =  SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOB_BASE,
  .gpioInput.rxPcr = PORTB_BASE,
  .gpioInput.rxPinNum = 5,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_3 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL3),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL3),
  .limits.delta =  TSI_DELTA(TSI_VAL3, TSI_SVAL3),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL3, TSI_SVAL3)*TSI_MIN_DELTA_PERCENT/100,    
  .input.rxCH  =  TSI0_CH8, /*TSI_CHx*/
  .input.txCH  =  SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOB_BASE,
  .gpioInput.rxPcr = PORTB_BASE,
  .gpioInput.rxPinNum = 4,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_4 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL4),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL4),
  .limits.delta =  TSI_DELTA(TSI_VAL4, TSI_SVAL4),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL4, TSI_SVAL4)*TSI_MIN_DELTA_PERCENT/100,    
  .input.rxCH  =  TSI0_CH10, /*TSI_CHx*/
  .input.txCH  =  SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 7,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_5 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_VAL5),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_VAL5),
  .limits.delta =  TSI_DELTA(TSI_VAL5, TSI_SVAL5),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_VAL5, TSI_SVAL5)*TSI_MIN_DELTA_PERCENT/100,   
  .input.rxCH  =  TSI0_CH5, /*TSI_CHx*/
  .input.txCH  =  SAFETY_SELFCAP_MODE, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 1,
  .gpioInput.txGpio = SAFETY_SELFCAP_MODE,
  .gpioInput.txPinNum = 0,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 


/* Mutual mode electrodes */
fs_tsi_t tsi_safety_test_item_M0 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_M_VAL0),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_M_VAL0),
  .limits.delta =  TSI_DELTA(TSI_M_VAL0, TSI_M_SVAL0),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_M_VAL0, TSI_M_SVAL0)*TSI_MIN_DELTA_PERCENT/100,   
  .input.rxCH  =  TSI0_CH6, /*TSI_CHx*/
  .input.txCH  =  TSI0_CH3, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 5,
  .gpioInput.txGpio = GPIOE_BASE,
  .gpioInput.txPinNum = 11,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_M1 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_M_VAL1),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_M_VAL1),
  .limits.delta =  TSI_DELTA(TSI_M_VAL1, TSI_M_SVAL1),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_M_VAL1, TSI_M_SVAL1)*TSI_MIN_DELTA_PERCENT/100,   
  .input.rxCH  =  TSI0_CH7, /*TSI_CHx*/
  .input.txCH  =  TSI0_CH3, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 6,
  .gpioInput.txGpio = GPIOE_BASE,
  .gpioInput.txPinNum = 11,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0, 
}; 

fs_tsi_t tsi_safety_test_item_M2 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_M_VAL2),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_M_VAL2),
  .limits.delta =  TSI_DELTA(TSI_M_VAL2, TSI_M_SVAL2),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_M_VAL2, TSI_M_SVAL2)*TSI_MIN_DELTA_PERCENT/100,   
  .input.rxCH  =  TSI0_CH6, /*TSI_CHx*/
  .input.txCH  =  TSI0_CH2, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 5,
  .gpioInput.txGpio = GPIOE_BASE,
  .gpioInput.txPinNum = 10,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0,  
}; 
fs_tsi_t tsi_safety_test_item_M3 = 
{
  .limits.low   =  TSI_CNT_LOW_LIMIT(TSI_M_VAL3),
  .limits.high  =  TSI_CNT_HIGH_LIMIT(TSI_M_VAL3),
  .limits.delta =  TSI_DELTA(TSI_M_VAL3, TSI_M_SVAL3),
  .limits.deltaDev = TSI_MIN_DELTA_PERCENT,
  .limits.deltaPer = TSI_DELTA(TSI_M_VAL3, TSI_M_SVAL3)*TSI_MIN_DELTA_PERCENT/100,   
  .input.rxCH  =  TSI0_CH7, /*TSI_CHx*/
  .input.txCH  =  TSI0_CH2, /*TSI_CHx*/
  .gpioInput.rxGpio = GPIOD_BASE,
  .gpioInput.rxPcr = PORTD_BASE,
  .gpioInput.rxPinNum = 6,
  .gpioInput.txGpio = GPIOE_BASE,
  .gpioInput.txPinNum = 10,
  .stimPolarity = 0,
  .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
  .tsiCounter.counterValue = 0,  
}; 

/* NULL terminated array of pointers to dio_test_t items for safety TSI test */
fs_tsi_t*  tsi_safety_test_items[] = { &tsi_safety_test_item_0, &tsi_safety_test_item_1, &tsi_safety_test_item_2, &tsi_safety_test_item_3, &tsi_safety_test_item_4, &tsi_safety_test_item_5,\
  &tsi_safety_test_item_M0, &tsi_safety_test_item_M1, &tsi_safety_test_item_M2, &tsi_safety_test_item_M3, NULL };

/*******************************************************************************
*                                                                              *
*                           STRUCTURE FOR DIO Initialization and TEST          *
*                                                                              *
*******************************************************************************/

fs_dio_test_t dio_safety_test_item_0 =
{
  .gpio   = GPIOD_BASE,
  .pcr    = PORTD_BASE, /* Base address of PCR register */
  .pinNum = 5,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_1 =
{
  .gpio   = GPIOD_BASE,
  .pcr    = PORTD_BASE, /* Base address of PCR register */
  .pinNum = 6,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_2 =
{
  .gpio   = GPIOE_BASE,
  .pcr    = PORTE_BASE, /* Base address of PCR register */
  .pinNum = 11,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_3 =
{
  .gpio   = GPIOE_BASE,
  .pcr    = PORTE_BASE, /* Base address of PCR register */
  .pinNum = 10,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_4 =
{
  .gpio   = GPIOD_BASE,
  .pcr    = PORTD_BASE, /* Base address of PCR register */
  .pinNum = 1,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_5 =
{ 
  .gpio   = GPIOD_BASE,
  .pcr    = PORTD_BASE, /* Base address of PCR register */
  .pinNum = 7,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};


fs_dio_test_t dio_safety_test_item_6 =
{
  .gpio   = GPIOB_BASE,
  .pcr    = PORTB_BASE, /* Base address of PCR register */
  .pinNum = 4,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_7 =
{
  .gpio   = GPIOB_BASE,
  .pcr    = PORTB_BASE, /* Base address of PCR register */
  .pinNum = 5,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_8 =
{
  .gpio   = GPIOE_BASE,
  .pcr    = PORTE_BASE, /* Base address of PCR register */
  .pinNum = 4,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_9 =
{ 
  .gpio   = GPIOE_BASE,
  .pcr    = PORTE_BASE, /* Base address of PCR register */
  .pinNum = 5,
  .pinDir = PIN_DIRECTION_IN,
  .pinMux = PIN_MUX_GPIO,
};

/* NULL terminated array of pointers to fs_dio_test_t items for safety DIO test */
fs_dio_test_t *dio_safety_test_items[] = { &dio_safety_test_item_0, &dio_safety_test_item_1, &dio_safety_test_item_2, &dio_safety_test_item_3, &dio_safety_test_item_4, &dio_safety_test_item_5, &dio_safety_test_item_6, &dio_safety_test_item_7, &dio_safety_test_item_8, &dio_safety_test_item_9, NULL };