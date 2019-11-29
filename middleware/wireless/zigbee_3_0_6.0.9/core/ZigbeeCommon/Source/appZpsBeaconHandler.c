/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       appZpsBeaconHandler.c
\brief      
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "appZpsBeaconHandler.h"
#include "mac_sap.h"
#ifdef BEACON_ENABLE
#include "dbg.h"
#endif
/************************/
/**** MACROS/DEFINES ****/
/************************/
/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/
/******************************/
/**** FORWARD DECLARATIONS ****/
/******************************/

/*****************/
/**** IMPORTS ****/
/*****************/
extern uint64 zps_u64NwkLibFromPayload(
    uint8 *pu8Buffer);
/*****************/
/**** EXPORTS ****/
/*****************/

#ifdef APP_PROCESS_BEACON
    extern void vAPPBeaconHandler(MAC_MlmeDcfmInd_s* psBeaconIndication);
#endif
tsBeaconFilterType *psBeaconFilter = NULL;
/**************/
/**** DATA ****/
/**************/

/**** LOCAL SCOPE ****/
#if 0
PRIVATE bool_t bDiscovery = 0;
#endif
/**** MODULE SCOPE ****/

/*************************/
/*************************/
/**** PRIVATE METHODS ****/
/*************************/
/*************************/
/************************/
/************************/
/**** MODULE METHODS ****/
/************************/
/************************/

/************************/
/************************/
/**** PUBLIC METHODS ****/
/************************/
/************************/



/****************************************************************************
 *
 * NAME:       ZPS_bAppAddBeaconFilter
 */
/**
 * @param
 *            tsBeaconFilterType *
 * @return
 *
 * @note
 *
 * Function is called by application to enable filtering
 ****************************************************************************/

PUBLIC void ZPS_bAppAddBeaconFilter(tsBeaconFilterType *psAppBeaconStruct)
{
    psBeaconFilter = psAppBeaconStruct;
}


/****************************************************************************
 *
 * NAME:       ZPS_bAppRemoveBeaconFilter
 */
/**
 * @param
 *
 * @return
 *
 * @note
 * Funcation may be called to disable filtering. It is called from the stack
 * and may be called from the application
 *
 * WARNING DO NOT CHANGE FUNCTION NAME,PARAMETER LIST AND RETURN TYPE
 * THIS IS CALLED FROM THE STACK
 ****************************************************************************/

PUBLIC void ZPS_bAppRemoveBeaconFilter(void)
{
    psBeaconFilter = NULL;
}


/****************************************************************************
 *
 * NAME:       ZPS_bAppPassBeaconToHigherLayer
 */
/**
 * @param
 *            MAC_MlmeDcfmInd_s*
 * @return
 *            boolean
 * @note
 *
 * The function is called from the Zigbee pro stack to indicate beacon
 * notification. It provides a mechanism to be selective on the which beacons
 * we are interested in
 * WARNING DO NOT CHANGE FUNCTION NAME,PARAMETER LIST AND RETURN TYPE
 * THIS IS CALLED FROM THE STACK. THE BODY OF THE FUNCTION MAY BE CHANGED.
 ****************************************************************************/

PUBLIC bool_t ZPS_bAppPassBeaconToHigherLayer(MAC_MlmeDcfmInd_s* psBeaconIndication)
{
    uint8 u8Traverse;
        /* we only want zigbee beacons */
    if((( psBeaconIndication->uParam.sIndBeacon.u8SDUlength > 0) &&
        ( psBeaconIndication->uParam.sIndBeacon.u8SDUlength >= 15) &&
        ( BF_BCN_PL_STACK_PROFILE(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 1 ||
          BF_BCN_PL_STACK_PROFILE(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 2 ) &&
        ( BF_PL_PROTOCOL_VER(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 2) &&
      /*( BF_PL_RESERVED_BITS(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 0) &&*/
        (BF_BCN_PL_PROTOCOL_ID(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 0)))
    {
    #ifdef APP_PROCESS_BEACON
        vAPPBeaconHandler(psBeaconIndication);
    #endif
#ifdef BEACON_ENABLE
        uint32 c = 0;
        extern bool_t bBeaconPrint;
        if(bBeaconPrint)
        {
            DBG_vPrintf(TRUE, "beacon:rx 0x%04x, ", psBeaconIndication->uParam.sIndBeacon.sPANdescriptor.sCoord.u16PanId);
            DBG_vPrintf(TRUE, "AP %x, EP ", BF_GET_ASSOC_PERMIT(psBeaconIndication->uParam.sIndBeacon.sPANdescriptor.u16SuperframeSpec));
            for( c = 3; c < 11; c++)
            {
                DBG_vPrintf ( TRUE, "%02x ", psBeaconIndication->uParam.sIndBeacon.u8SDU [c] );
            }
            DBG_vPrintf ( TRUE , "\n");
        }
#endif
        /*if(bDiscovery == 1)
        {
            vScanDiscoveryOnlyResponse(psBeaconIndication);
            bDiscovery = FALSE;
            return FALSE;
        }*/

        /* Is filter registered */
        if(psBeaconFilter)
        {
            if(psBeaconFilter->u16FilterMap & BF_USED_BITMASK)
            {
                if((psBeaconFilter->u16FilterMap & (BF_BITMAP_BLACKLIST|BF_BITMAP_WHITELIST)) == (BF_BITMAP_BLACKLIST|BF_BITMAP_WHITELIST))
                {
                    return TRUE; /* invalid bitmask pass the upwards*/
                }

                if(psBeaconFilter->u16FilterMap & (BF_BITMAP_BLACKLIST|BF_BITMAP_WHITELIST))
                {
                    for(u8Traverse=0; u8Traverse < psBeaconFilter->u8ListSize; u8Traverse++)
                    {
                        uint64 u64ExtendedPanId = zps_u64NwkLibFromPayload(BF_BCN_PL_EXT_PAN_ID_PTR(psBeaconIndication->uParam.sIndBeacon.u8SDU));

                        if((u64ExtendedPanId == psBeaconFilter->pu64ExtendPanIdList[u8Traverse])
                           && psBeaconFilter->u16FilterMap & BF_BITMAP_BLACKLIST)
                        {
                            return FALSE;
                        }

                        if((u64ExtendedPanId == psBeaconFilter->pu64ExtendPanIdList[u8Traverse])
                            && psBeaconFilter->u16FilterMap & BF_BITMAP_WHITELIST)
                        {
                            break;
                        }
                    }

                    if((psBeaconFilter->u16FilterMap & BF_BITMAP_WHITELIST) &&
                    (u8Traverse == psBeaconFilter->u8ListSize))
                    {
                        return FALSE; /* Not in Whitelist */
                    }
                }

                /*check for any other filters */

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_PERMIT_JOIN))
                {
                    if(BF_GET_ASSOC_PERMIT(psBeaconIndication->uParam.sIndBeacon.sPANdescriptor.u16SuperframeSpec) == 0)
                    {
                        return FALSE; /* Association not permitted */
                    }

                }

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_CAP_ENDDEVICE))
                {
                    if(BF_BCN_PL_ZED_CAPACITY(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 0)
                    {
                        return FALSE;/* no End device capacity  */
                    }
                }

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_CAP_ROUTER))
                {
                    if(BF_BCN_PL_ZR_CAPACITY(psBeaconIndication->uParam.sIndBeacon.u8SDU) == 0)
                    {
                        return FALSE;/* no Router capacity  */
                    }
                }

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_LQI))
                {
                    if(psBeaconIndication->uParam.sIndBeacon.sPANdescriptor.u8LinkQuality < psBeaconFilter->u8Lqi)
                    {
                        return FALSE; /* link quality isn't good enough */
                    }
                }

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_SHORT_PAN))
                {
                    if ( psBeaconIndication->uParam.sIndBeacon.sPANdescriptor.sCoord.u16PanId != psBeaconFilter->u16Panid )
                    {
                        return FALSE; /* Association not permitted */
                    }

                }
                if((psBeaconFilter->u16FilterMap & BF_BITMAP_DEPTH))
                {
                    if(psBeaconFilter->u8Depth == 0xFF)
                    {
                        if((BF_BCN_PL_DEVICE_DEPTH(psBeaconIndication->uParam.sIndBeacon.u8SDU) > 0 ) )
                        {
                            return FALSE; /* Depth is greater than 0 ignore it since we only want to hear from Coordinators */
                        }
                    }
                    
                    if((BF_BCN_PL_DEVICE_DEPTH(psBeaconIndication->uParam.sIndBeacon.u8SDU) >= psBeaconFilter->u8Depth) )
                    {
                        return FALSE; /* depth isn't good enough */
                    }
                }

                if((psBeaconFilter->u16FilterMap & BF_BITMAP_PRIORITY_PARENT))
                {
                    if(( BF_PL_RESERVED_BITS(psBeaconIndication->uParam.sIndBeacon.u8SDU) & 0x2 ) == 0  )
                    {
                        return FALSE; /* No connectivity to TC   */
                    }

                }
                /* passed all the filter tests */
                return TRUE;


            }
            else
            {
                return TRUE; /* invalid filter pass the beacon upwards */
            }
        }
        else /* no filter registered pass the beacon upwards */
        {
            return TRUE;
        }

    }

    return FALSE; /* We get here only if not a zigbee beacon,throw the beacon away*/


}

/*Local functions */
#if 0
/****************************************************************************
 *
 * NAME:       ZPS_bAppDiscoveryReceived
 */
/**
 * @param
 *
 * @return
 *
 * @note
 *
 *
 ****************************************************************************/
PUBLIC void ZPS_bAppDiscoveryReceived(void)
{
    bDiscovery = TRUE;
}
#endif
/* End of file $Id: appZpsBeaconHandler.c 2014-01-08 17:21:12Z nxp29741 $ *******************************************/
