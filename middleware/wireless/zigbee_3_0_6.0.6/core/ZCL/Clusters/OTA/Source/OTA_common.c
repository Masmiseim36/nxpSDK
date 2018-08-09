/*
* The Clear BSD License
* Copyright 2012-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
   (void)EEPROM_WriteData(u16Len, u32FlashByteLocation, pu8Data);
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
    EEPROM_ReadData(u16Len, u32FlashByteLocation, pu8Data);


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
