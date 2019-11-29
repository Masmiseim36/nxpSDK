/*
* Copyright 2015-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BDB_TL_INCLUDED
#define BDB_TL_INCLUDED

/*!=============================================================================
\file       bdb_tl.h
\brief      BDB Touchlink API
==============================================================================*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define ADJUST_POWER   TRUE

#ifdef CPU_MKW41Z512VHT4
#define TX_POWER_NORMAL     (22) //0Bbm (gPhyDefaultTxPowerLevel_d)
#define TX_POWER_LOW        (3) //-16dBm
#else
#if JENNIC_CHIP==JN5169
#define TX_POWER_NORMAL     ((uint32)(8))
#define TX_POWER_LOW        ((uint32)(0))
#else
#define TX_POWER_NORMAL     ((uint32)(3))
#define TX_POWER_LOW        ((uint32)(-9))
#endif
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/



/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void BDB_vTlInit(void);
PUBLIC bool_t BDB_bTlTouchLinkInProgress( void);
PUBLIC bool_t bIsTlStarted(void);
PUBLIC void BDB_vTlStateMachine( tsBDB_ZCLEvent *psEvent);

PUBLIC uint8 BDB_u8TlEncryptKey( uint8* au8InData,
                                  uint8* au8OutData,
                                  uint32 u32TransId,
                                  uint32 u32ResponseId,
                                  uint8 u8KeyIndex);
PUBLIC uint8 BDB_eTlDecryptKey( uint8* au8InData,
                                  uint8* au8OutData,
                                  uint32 u32TransId,
                                  uint32 u32ResponseId,
                                  uint8 u8KeyIndex);
PUBLIC bool_t BDB_bTlIsKeySupported(uint8 u8KeyIndex);
PUBLIC uint8 BDB_u8TlGetRandomPrimary(void);
PUBLIC uint8 BDB_u8TlNewUpdateID(uint8 u8ID1, uint8 u8ID2 );
#if (defined JENNIC_CHIP_FAMILY_JN516x) || (defined JENNIC_CHIP_FAMILY_JN517x)
PUBLIC void vECB_Decrypt(uint8* au8Key,
                         uint8* au8InData,
                         uint8* au8OutData);
#endif
PUBLIC void BDB_vTlTimerCb(void *pvParam);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern PUBLIC uint8 u8TimerBdbTl;

#if defined __cplusplus
}
#endif

#endif  /* BDB_TL_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/






