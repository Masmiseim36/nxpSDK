/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_NTAG_H_
#define APP_NTAG_H_

/*!
\file       app_ntag.h
\brief      Light Bulb application layer for NTAG
*/
     
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <ntag.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define APP_NTAG_ADDRESS          0xFFU  /* I2C Address (0xFF for automatic detection) */

#if (defined OM15045) || (defined OM15053)
#define APP_NTAG_I2C_LOCATION     TRUE   /* DIO4 and DIO5 */
#else
#define APP_NTAG_I2C_LOCATION     FALSE  /* TRUE uses alternate I2C lines (DIO16, 17) instead of (DIO14, 15) */
#endif

#define APP_NTAG_I2C_FREQUENCY_HZ 100000 /* I2C prescale value */
#define APP_NTAG_TICK_MS          5      /* Interval of tick timer in ms */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t APP_bNtagPdmLoad(void);
PUBLIC void APP_vNtagStart(uint8 u8NscType);
PUBLIC void APP_vNtagStop(void);
PUBLIC void APP_cbNtagTimer(void *pvParams);
PUBLIC void APP_cbNtagEvent(            /* Called when an event takes place */
       teNtagEvent  eNtagEvent,         /* Event raised */
       uint32       u32Address,
       uint32       u32Length,
       uint8        *pu8Data);          /* Event data (NULL if no data) */

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_NTAG_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
