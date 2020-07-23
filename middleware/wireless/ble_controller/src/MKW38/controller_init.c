/*! *********************************************************************************
 * \defgroup CONTROLLER
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
*
* Copyright 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "board.h"
#include "ble_general.h"
#include "Flash_Adapter.h"
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_0p5_config.h"
#include "nxp_xcvr_coding_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef gControllerXcvrInitRetryCount_c
#define gControllerXcvrInitRetryCount_c (10U)
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/



#if defined(gControllerPreserveXcvrDacTrimValue_d) && gControllerPreserveXcvrDacTrimValue_d
static void Controller_SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);
static uint32_t Controller_RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);
#endif


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#if defined(gControllerPreserveXcvrDacTrimValue_d) && gControllerPreserveXcvrDacTrimValue_d
static void  Controller_SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    NV_Init();
    FLib_MemCpy(&gHardwareParameters.xcvrCal, xcvrDacTrim, sizeof(xcvr_DcocDacTrim_t));
    NV_WriteHWParameters(&gHardwareParameters);
}

static uint32_t Controller_RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    uint32_t status;

    if (FLib_MemCmpToVal(&gHardwareParameters.xcvrCal, 0xFF, sizeof(xcvr_DcocDacTrim_t)))
    {
        status = 1;
    }
    else
    {
        status = 0;
        FLib_MemCpy(xcvrDacTrim, &gHardwareParameters.xcvrCal, sizeof(xcvr_DcocDacTrim_t));
    }

    return status;
}
#endif


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief   Initializes the XCVR module
*
* \return uint32_t , 0 if successful, error if other values.
*
********************************************************************************** */

uint32_t Controller_RadioInit(void)
{
    xcvrStatus_t status;
    uint32_t count = 0;

#if defined(gControllerPreserveXcvrDacTrimValue_d) && gControllerPreserveXcvrDacTrimValue_d
    xcvr_DcocDacTrim_t  mXcvrDacTrim;
    uint32_t            trim_invalid;
#endif

    const xcvr_config_t *xcvrConfig         = &xcvr_gfsk_bt_0p5_h_0p5_1mbps_full_config;
    const xcvr_coding_config_t *rbmeConfig  = &xcvr_ble_coded_s8_config;

#if defined(gControllerPreserveXcvrDacTrimValue_d) && gControllerPreserveXcvrDacTrimValue_d
    /* check if reserved memory in HW parameters is at least sizeof xcvr_DcocDacTrim_t */
    assert(sizeof(gHardwareParameters.xcvrCal) >= sizeof(xcvr_DcocDacTrim_t) );

    /* Check if XCVR Trim value is valid */
    trim_invalid = Controller_RestoreXcvrDcocDacTrimFromFlash(&mXcvrDacTrim);

    /* Initialize Radio without trim */
    status = XCVR_InitNoDacTrim(&xcvrConfig, &rbmeConfig);
    assert( status == gXcvrSuccess_c );

    if(trim_invalid != 0U)
    {
        /* XCVR Trim value is not valid. Calculate now. */
        do {
            count++;
            status = XCVR_CalculateDcocDacTrims(&mXcvrDacTrim);
        } while ((gXcvrSuccess_c != status) && (count < gControllerXcvrInitRetryCount_c));

        /* store calculated XCVR Trim value */
        (void)Controller_SaveXcvrDcocDacTrimToFlash(&mXcvrDacTrim);
    }
    else
    {
        /* XCVR Trim value is valid. Restore it. */
        status = XCVR_SetDcocDacTrims(&mXcvrDacTrim);
    }
#else
    do {
        count++;
        status = XCVR_Init(&xcvrConfig, &rbmeConfig);
    } while ((gXcvrSuccess_c != status) && (count < gControllerXcvrInitRetryCount_c));
#endif /* defined(gControllerPreserveXcvrDacTrimValue_d) && gControllerPreserveXcvrDacTrimValue_d */

    (void)XCVR_SetXtalTrim( BOARD_GetXtal32MhzTrim(FALSE));

    return (uint32_t)status;
}

/*! *********************************************************************************
* @}
********************************************************************************** */