/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  COLOUR_CONTROL_INTERNAL_H_INCLUDED
#define  COLOUR_CONTROL_INTERNAL_H_INCLUDED

/*!
\file       ColourControl_internal.h
\brief      The internal API for the Colour Control Cluster
*/

    
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "ColourControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ColourControlCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveToHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveToHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandStepHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_StepHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveToSaturationCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveToSaturationCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveSaturationCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveSaturationCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandStepSaturationCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_StepSaturationCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveToHueAndSaturationCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveToHueAndSaturationCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveToColourCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveToColourCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveColourCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveColourCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandStepColourCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_StepColourCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandMoveToColourTemperatureCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveToColourTemperatureCommandPayload *psPayload);

#if (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_ENHANCE_HUE_SUPPORTED)
PUBLIC teZCL_Status eCLD_ColourControlCommandEnhancedMoveToHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_EnhancedMoveToHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandEnhancedMoveHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_EnhancedMoveHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandEnhancedStepHueCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_EnhancedStepHueCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ColourControlCommandEnhancedMoveToHueAndSaturationCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_EnhancedMoveToHueAndSaturationCommandPayload *psPayload);
#endif //#if (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_ENHANCE_HUE_SUPPORTED)

#if (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_COLOUR_LOOP_SUPPORTED)
PUBLIC teZCL_Status eCLD_ColourControlCommandColourLoopSetCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_ColourLoopSetCommandPayload *psPayload);
#endif

#if ((CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_HUE_SATURATION_SUPPORTED)   ||\
     (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_ENHANCE_HUE_SUPPORTED)      ||\
     (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_XY_SUPPORTED)               ||\
     (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_COLOUR_TEMPERATURE_SUPPORTED))
    PUBLIC teZCL_Status eCLD_ColourControlCommandStopMoveStepCommandReceive(
                        ZPS_tsAfEvent               *pZPSevent,
                        uint8                       *pu8TransactionSequenceNumber,
                        tsCLD_ColourControl_StopMoveStepCommandPayload *psPayload);
#endif

#if (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_COLOUR_TEMPERATURE_SUPPORTED)
PUBLIC teZCL_Status eCLD_ColourControlCommandMoveColourTemperatureCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_MoveColourTemperatureCommandPayload *psPayload);
PUBLIC teZCL_Status eCLD_ColourControlCommandStepColourTemperatureCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ColourControl_StepColourTemperatureCommandPayload *psPayload);
#endif //#if (CLD_COLOURCONTROL_COLOUR_CAPABILITIES & COLOUR_CAPABILITY_COLOUR_TEMPERATURE_SUPPORTED)

PUBLIC teZCL_Status eCLD_ColourControlUpdate(uint8 u8SourceEndPointId);


PUBLIC teZCL_Status eCLD_ColourControl_HSV2xyY(
        uint8                       u8SourceEndPointId,
        uint16                      u16Hue,
        uint8                       u8Saturation,
        uint8                       u8Value,
        uint16                      *pu16x,
        uint16                      *pu16y,
        uint8                       *pu8Y);

PUBLIC teZCL_Status eCLD_ColourControl_xyY2HSV(
        tsCLD_ColourControlCustomDataStructure  *psCustomDataStructPtr,
        uint16                      u16x,
        uint16                      u16y,
        uint8                       u8Y,
        uint16                      *pu16Hue,
        uint8                       *pu8Saturation,
        uint8                       *pu8Value);

PUBLIC void vCLD_ColourControl_CCT2xyY(
        uint16                      u16ColourTemperatureMired,
        uint16                      *pu16x,
        uint16                      *pu16y,
        uint8                       *pu8Y);

PUBLIC void vCLD_ColourControl_xyY2CCT(
        uint16                      u16x,
        uint16                      u16y,
        uint8                       u8Y,
        uint16                      *pu16ColourTemperature);

PUBLIC teZCL_Status eCLD_ColourControlCalculateConversionMatrices(
        tsCLD_ColourControlCustomDataStructure  *psCustomDataStructure,
        float                                   fRedX,
        float                                   fRedY,
        float                                   fGreenX,
        float                                   fGreenY,
        float                                   fBlueX,
        float                                   fBlueY,
        float                                   fWhiteX,
        float                                   fWhiteY);
PUBLIC bool_t CS_bTransitionIsValid(uint16 u16X1,  uint16 u16Y1,
                                    uint16 u16X2,  uint16 u16Y2,
                                    uint16 *pu16X, uint16 *pu16Y);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* COLOUR_CONTROL_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
