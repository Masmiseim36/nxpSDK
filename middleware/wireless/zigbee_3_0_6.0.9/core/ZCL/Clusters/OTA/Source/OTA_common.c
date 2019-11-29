/*
* Copyright 2012-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       ota_common.c
\brief      Over The Air Upgrade
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "OTA.h"
#include "dbg.h"
#include "OtaSupport.h"
#include "fsl_os_abstraction.h"

#ifdef OTA_PC_BUILD
extern PUBLIC bool_t bAHI_FlashInit(
                             uint8    flashType,
                             void *pCustomFncTable);
extern PUBLIC bool_t bAHI_FlashEraseSector(
                             uint8              u8Sector);
extern PUBLIC bool_t bAHI_FullFlashProgram(
                             uint32             u32Addr,
                             uint16             u16Len,
                             uint8             *pu8Data);
extern PUBLIC bool_t bAHI_FullFlashRead(
                             uint32             u32Addr,
                             uint16             u16Len,
                             uint8             *pu8Data);
extern PUBLIC void vAHI_SwReset(void);
#endif

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
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/****************************************************************************
 **
 ** NAME:       vOtaFlashInitHw
 **
 ** DESCRIPTION:
 ** Initialise the external Flash
 **
 ** PARAMETERS:               Name                           Usage
 ** uint8                    u8FlashType                   Flash type
 ** void                    *pvFlashTable                  Flash table
 **
 ** RETURN:
 ** None
 ****************************************************************************/
PUBLIC  void vOtaFlashInitHw ( uint8    u8FlashType,
                               void    *pvFlashTable)
{
    OTA_InitExternalMemory();
}
/****************************************************************************
 **
 ** NAME:       vOtaFlashErase
 **
 ** DESCRIPTION:
 ** Erase the Flash
 **
 ** PARAMETERS:               Name                           Usage
 ** uint8                    u8Sector                      sector number to erase
 **
 ** RETURN:
 ** None
 ****************************************************************************/
PUBLIC  void vOtaFlashErase(uint8 u8Sector)
{
    //OTA_EraseBlock();
    // To Do
}
/****************************************************************************
 **
 ** NAME:       vOtaFlashWrite
 **
 ** DESCRIPTION:
 ** Write data in the Flash
 **
 ** PARAMETERS:               Name                           Usage
 ** uint32                 u32FlashByteLocation           Flash byte location
 ** uint16                 u16Len                         Length of data
 ** uint8                 *pu8Data                        data bytes
 **
 ** RETURN:
 ** None
 ****************************************************************************/

PUBLIC  void vOtaFlashWrite(
                                  uint32     u32FlashByteLocation,
                                  uint16     u16Len,
                                  uint8     *pu8Data)
{
   (void)OTA_WriteExternalMemory(pu8Data, u16Len, u32FlashByteLocation);
}
/****************************************************************************
 **
 ** NAME:       vOtaFlashRead
 **
 ** DESCRIPTION:
 ** Read data from the Flash
 **
 ** PARAMETERS:               Name                           Usage
 ** uint32                 u32FlashByteLocation           Flash byte location
 ** uint16                 u16Len                         Length of data
 ** uint8                 *pu8Data                        data bytes
 **
 ** RETURN:
 ** None
 ****************************************************************************/
PUBLIC  void vOtaFlashRead(
                                  uint32     u32FlashByteLocation,
                                  uint16     u16Len,
                                  uint8     *pu8Data)
{
    OTA_ReadExternalMemory(pu8Data, u16Len, u32FlashByteLocation);


}
/****************************************************************************
 **
 ** NAME:       vOtaSwitchLoads
 **
 ** DESCRIPTION:
 ** Software reset
 **
 ** PARAMETERS:               Name                           Usage
 **
 ** RETURN:
 ** None
 ****************************************************************************/

PUBLIC  void vOtaSwitchLoads(void)
{
    ResetMCU();
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
