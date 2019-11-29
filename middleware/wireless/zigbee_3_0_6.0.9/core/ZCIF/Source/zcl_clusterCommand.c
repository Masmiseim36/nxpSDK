/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       zcl_attribute.c
\brief      Attribute read and write functions
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>

#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       bZCL_CheckManufacturerSpecificCommandFlagMatch
 **
 ** DESCRIPTION:
 **
 **
 ** PARAMETERS:                 Name                    Usage
 ** tsZCL_CommandDefinition  *psCommandDefinition   Attribute data type struct
 ** bool_t                      bManufacturerSpecificAttributeFlag
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC bool_t bZCL_CheckManufacturerSpecificCommandFlagMatch(
                    tsZCL_CommandDefinition  *psCommandDefinition,
                    bool_t                    bManufacturerSpecificCommandFlag)
{

    bool_t bLocalManufacturerSpecificCommandFlag = FALSE;

    if(psCommandDefinition==NULL)
    {
        return(FALSE);
    }
    // get attribute flag and set bool flag
    if(psCommandDefinition->u8CommandFlags & E_ZCL_CF_MS)
    {
        bLocalManufacturerSpecificCommandFlag = TRUE;
    }
    // compare
    if(bLocalManufacturerSpecificCommandFlag == bManufacturerSpecificCommandFlag)
    {
        return(TRUE);
    }
    return(FALSE);
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/



