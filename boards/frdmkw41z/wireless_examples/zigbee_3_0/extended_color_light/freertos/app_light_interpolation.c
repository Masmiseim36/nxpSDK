/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_light_interpolation.c
\brief      Light Bulb application: ZCL Linear Interpolation - Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_light_interpolation.h"
#include "DriverBulb_Shim.h"
#include "dbg.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define INTPOINTS    (10)
#define SCALE         (7)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    uint32 u32Current;
    uint32 u32Target;
    int32  i32Delta;
}tsLI_Params;

typedef struct
{
    tsLI_Params sLevel;
    tsLI_Params sRed;
    tsLI_Params sGreen;
    tsLI_Params sBlue;
    tsLI_Params sColTemp;
    uint32      u32PointsAdded;

}tsLI_Vars;

bool_t bSaved = FALSE;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vLI_InitVar(tsLI_Params *psLI_Params, uint32 u32NewTarget);
PRIVATE uint32  u32divu10(uint32 n);

/****************************************************************************/
/*          Exported Variables                                              */
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE tsLI_Vars sLI_Vars = {.sLevel.i32Delta   = 0,
                              .sRed.i32Delta     = 0,
                              .sGreen.i32Delta   = 0,
                              .sBlue.i32Delta    = 0,
                              .sColTemp.i32Delta = 0,
                              .u32PointsAdded  = INTPOINTS};

PRIVATE tsLI_Vars sLI_SavedVars;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 * NAME: vLI_SetCurrentValues
 *
 * DESCRIPTION:
 * Sets the current interpolation values
 ****************************************************************************/
PUBLIC void vLI_SetCurrentValues(uint32 u32Level, uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32ColTemp)
{
    sLI_Vars.sLevel.u32Current   = u32Level   << SCALE;
    sLI_Vars.sRed.u32Current     = u32Red     << SCALE;
    sLI_Vars.sGreen.u32Current   = u32Blue    << SCALE;
    sLI_Vars.sBlue.u32Current    = u32Green   << SCALE;
    sLI_Vars.sColTemp.u32Current = u32ColTemp << SCALE;

}

PUBLIC void vSaveVars(void) {
    if (!bSaved) {
        sLI_SavedVars = sLI_Vars;
        bSaved = TRUE;
    }
}

PUBLIC void vRestoreVars(void) {
    if (bSaved) {
       sLI_Vars = sLI_SavedVars;
        bSaved = FALSE;
    }
}

/****************************************************************************
 * NAME: vLI_Start
 *
 * DESCRIPTION:
 * Starts the linear interpolation process between successive ZCL updates
 ****************************************************************************/
PUBLIC void vLI_Start(uint32 u32Level, uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32ColTemp)
{
    vLI_InitVar(&sLI_Vars.sLevel,    u32Level);
    vLI_InitVar(&sLI_Vars.sRed,      u32Red);
    vLI_InitVar(&sLI_Vars.sGreen,    u32Green);
    vLI_InitVar(&sLI_Vars.sBlue,     u32Blue);
    vLI_InitVar(&sLI_Vars.sColTemp,  u32ColTemp);
    vLI_UpdateDriver();
    sLI_Vars.u32PointsAdded  = 1;
}

PUBLIC void vLI_Stop(void)
{
    sLI_Vars.u32PointsAdded = INTPOINTS;
}

/****************************************************************************
 * NAME: vLI_CreatePoints
 *
 * DESCRIPTION:
 * Provides multiple LI points in time/value pairs between
 * successive  ZCL updates.  This provides smooth 100Hz updates
 * on colour and level transitions
 ****************************************************************************/
PUBLIC void vLI_CreatePoints(void)
{

    if (sLI_Vars.u32PointsAdded < INTPOINTS)
    {
        sLI_Vars.u32PointsAdded++;
        sLI_Vars.sLevel.u32Current   += sLI_Vars.sLevel.i32Delta;
        sLI_Vars.sRed.u32Current     += sLI_Vars.sRed.i32Delta;
        sLI_Vars.sGreen.u32Current   += sLI_Vars.sGreen.i32Delta;
        sLI_Vars.sBlue.u32Current    += sLI_Vars.sBlue.i32Delta;
        sLI_Vars.sColTemp.u32Current += sLI_Vars.sColTemp.i32Delta;
        vLI_UpdateDriver();
    }
}

/****************************************************************************
 * NAME:    vLI_UpdateDriver
 *
 * DESCRIPTION:
 *            passes the LI points between previous and current
 *            ZCL updates to the colour driver for smooth transitions
 ****************************************************************************/
PUBLIC void vLI_UpdateDriver(void)
{
    vBULB_SetColour(sLI_Vars.sRed.u32Current   >> SCALE,
                     sLI_Vars.sGreen.u32Current >> SCALE,
                     sLI_Vars.sBlue.u32Current  >> SCALE);

    vBULB_SetLevel(sLI_Vars.sLevel.u32Current  >> SCALE);

    vBULB_SetColourTemperature(sLI_Vars.sColTemp.u32Current >> SCALE);
}

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 * NAME:    vLI_InitVar
 *
 * DESCRIPTION:
 *             Initialises a single LI variable to the output(s) from
 *            ZCL cluster, converts to big integer and calculates the adjustment
 *            needed on each of the successive LI points
 ****************************************************************************/
PRIVATE void vLI_InitVar(tsLI_Params *psLI_Params, uint32 u32NewTarget)
{
    psLI_Params->u32Target = u32NewTarget << SCALE;

    if (psLI_Params->u32Target < psLI_Params->u32Current)
    {
        psLI_Params->i32Delta = -(u32divu10(psLI_Params->u32Current - psLI_Params->u32Target));
    }
    else
    {
        psLI_Params->i32Delta = u32divu10(psLI_Params->u32Target - psLI_Params->u32Current);;
    }
    psLI_Params->u32Current += psLI_Params->i32Delta;
}

/****************************************************************************
 * NAME:    u32divu10
 *
 * DESCRIPTION:
 *            Fast divide by 10 routine (~8 times faster than gcc "/" operator)
 ****************************************************************************/
PRIVATE uint32  u32divu10(uint32 n)
{
    uint32 q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - q*10;
    return q + ((r + 6) >> 4);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
