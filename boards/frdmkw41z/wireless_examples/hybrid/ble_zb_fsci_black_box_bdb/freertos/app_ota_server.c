/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "dbg.h"
#include "PDM.h"
#include "PDM_IDs.h"
#include "app_pdum_cfg.h"
#include "OTA.h"
#include "app_ota_server.h"
#include "app_common.h"
//#include "zps_gen.h"  
#include "app_zcl_cfg.h"

#ifdef CLD_OTA
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_APP_OTA
    #define TRACE_APP_OTA               TRUE
#else
    #define TRACE_APP_OTA               FALSE
#endif

#define OTA_CLIENT_EP                      1
#define OTA_STARTUP_DELAY_IN_SEC           5
#define OTA_FIND_SERVER                    1
#define OTA_FIND_SERVER_WAIT               2
#define OTA_FIND_SERVER_ADDRESS            3
#define OTA_FIND_SERVER_ADDRESS_WAIT       4
#define OTA_QUERYIMAGE                     5
#define OTA_DL_PROGRESS                    6

//#define APP_IEEE_ADDR_RESPONSE            0x8001
//#define APP_MATCH_DESCRIPTOR_RESPONSE     0x8006

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vInitAndDisplayKeys(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/* MAC address */
PUBLIC    uint8    au8MacAddress[]  __attribute__ ((section (".ro_mac_address"))) =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/* Pre-configured Link Key */
PUBLIC    uint8    s_au8LnkKeyArray[16] __attribute__ ((section (".ro_se_lnkKey"))) =
{
    0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,
    0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39
};

volatile PRIVATE uint8    au8MacAddressVolatile [ 8 ];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vAppInitOTA
 *
 * DESCRIPTION:
 * Initialises application OTA server related data structures and calls
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppInitOTA ( void )
{
#if (defined JENNIC_CHIP_FAMILY)
    tsNvmDefs       sNvmDefs;
#elif (defined FRDM_KW41Z)
  tsNvmDefs       sNvmDefs;
#else
    //TODO
#endif
    teZCL_Status    eZCL_Status;
    uint8           au8CAPublicKey [ 22 ];

    /* Set Up OTA server related stuff */
    vInitAndDisplayKeys ( );

    #if ( OTA_MAX_IMAGES_PER_ENDPOINT ==  3 )
        uint8 u8StartSector [ 3 ] =  { 0, 2, 4 };
    #else
        #ifdef JENNIC_CHIP_FAMILY_JN516x
            uint8 u8StartSector [ 1 ] =  { 0 };
      #else
            uint8 u8StartSector[2] = {0,3};
        #endif
    #endif

#if (defined JENNIC_CHIP_FAMILY)
    sNvmDefs.u32SectorSize     =  64*1024;
    sNvmDefs.u8FlashDeviceType =  E_FL_CHIP_AUTO;
    vOTA_FlashInit ( NULL, &sNvmDefs );
#elif (defined FRDM_KW41Z)
    sNvmDefs.u32SectorSize     =  64*1024;
    //sNvmDefs.u8FlashDeviceType =  E_FL_CHIP_AUTO;
    vOTA_FlashInit ( NULL, &sNvmDefs );
#else
// TODO
#endif
    eZCL_Status =  eOTA_NewImageLoaded ( ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT, TRUE, NULL );
    DBG_vPrintf ( TRACE_APP_OTA, "\neOTA_NewImageLoaded status = %d\n", eZCL_Status );


    eZCL_Status =  eOTA_AllocateEndpointOTASpace ( ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT, u8StartSector, OTA_MAX_IMAGES_PER_ENDPOINT, 4, TRUE, au8CAPublicKey );
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_APP_OTA, "eAllocateEndpointOTASpace returned error 0x%x\n", eZCL_Status);
    }

    eZCL_Status = eOTA_SetServerAuthorisation ( ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT, E_CLD_OTA_STATE_ALLOW_ALL, NULL, 0 );
    if ( eZCL_Status !=  E_ZCL_SUCCESS )
    {
        DBG_vPrintf ( TRACE_APP_OTA, "eSetServerAuthorisation returned error 0x%x\n", eZCL_Status );
    }
}

/****************************************************************************
 *
 * NAME: vInitAndDisplayKeys
 *
 * DESCRIPTION:
 * Initialize Keys and displays the content
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vInitAndDisplayKeys ( void )
{
    #ifndef JENNIC_CHIP_FAMILY_JN514x
        /*ZPS_vSetOverrideLocalMacAddress((uint64 *)&au8MacAddress);*/
        volatile uint8    u8Index;
        for ( u8Index = 0; u8Index < 8;u8Index++ )
            au8MacAddressVolatile [ u8Index ] =  au8MacAddress [ u8Index ];
    #endif

    #if (TRACE_APP_OTA == TRUE)
    {
        uint8   i;
        DBG_vPrintf ( TRACE_APP_OTA, "MAC Address at address = %08x\n\n\n", au8MacAddress );
        for ( i  = 0; i < 8; i++ )
        {
            DBG_vPrintf ( TRACE_APP_OTA, "%02x ", au8MacAddress [ i ] );
        }

        DBG_vPrintf( TRACE_APP_OTA, "\n\n\n Link Key " );
        for ( i = 0;i < 16;i++ )
        {
            DBG_vPrintf ( TRACE_APP_OTA, "%02x ", s_au8LnkKeyArray [ i ] );
        }
        DBG_vPrintf ( TRACE_APP_OTA, "\n\n\n" );
    }
    #endif
}
#endif //CLD_OTA
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
