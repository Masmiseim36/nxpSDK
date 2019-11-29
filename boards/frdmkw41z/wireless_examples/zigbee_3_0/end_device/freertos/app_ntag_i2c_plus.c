/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_ntag.c
\brief      APP NTAG file
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/* NTAG middleware modules */
#include "HAL_I2C_driver.h"
#include "ntag_driver.h"
#include "ntag_driver_intern.h"
#include "ntag_bridge.h"
#include "nfc_device.h"
#include "ntag_defines.h"
#include "ndef_message.h"

/* APP NTAG header file */
#include "app_ntag_i2c_plus.h"

#include "bdb_DeviceCommissioning.h"
#include "RNG_Interface.h"
#include "app_zb_utils.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#if APP_NTAG_SUPPORT_FULL_MEM_READ_d
#define NTAG_MEM_ADDR_USER_MEMORY_SIZE_c (1896)
#endif

#define NTAG_PAYLOAD_KEY_SIZE          16
#define NTAG_NDEF_MSG_LEN_c    sizeof(au8NTAG_NDEF_Msg)

#define gInvalidateTagDefaultTimeout_c    60 /* seconds */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct NtagCommissioningData_tag
{
  uint64_t u64ExtAddress; /*  Device IEEE Extended Address */
  uint8_t  au8Key[NTAG_PAYLOAD_KEY_SIZE]; /*  Installation Code */
} NtagCommissioningData_t;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

#ifdef APP_NTAG_I2C_PLUS

 /*! -------------------------------------------------------------------------
 * \brief Checks if the NDEF messages of the NTAG smart poster are compliant
 *        with the defined format
 * \param[in] ntag_handle - handle to NTAG device
 * \return TRUE if the smart poster is compliant, FALSE otherwise
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_CheckNdefMsg(NFC_HANDLE_T ntag_handle);

 /*! -------------------------------------------------------------------------
 * \brief Read the Zigbee 3.0 install code from the tag
 * \param[in] ntag_handle - handle to NTAG device
 * \return -
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_ReadCommissioningData(NFC_HANDLE_T ntag_handle);

/*! -------------------------------------------------------------------------
 * \brief NTAG Field Detect (FD) interrupt service routine
 * \param[in] -
 * \return -
 *---------------------------------------------------------------------------*/
#if APP_NTAG_FD_INTERRUPT_ENABLE_d
static void APP_NTAG_FD_ISR(void);
#endif

#if APP_NTAG_DEVICE_IS_ZED_d
/*! -------------------------------------------------------------------------
 * \brief Clear NDEF message from tag
 * \param[in] ntag_handle - handle to NTAG device
 * \return TRUE if operation was successfull, FALSE otherwise
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_ClearTag(NFC_HANDLE_T ntag_handle);

/*! -------------------------------------------------------------------------
 * \brief Updates the NTAG handle
 * \param[in] the NTAG handle
 * \return TRUE if tag was updated successfully, FALSE otherwise
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_UpdateTag(NFC_HANDLE_T ntag_handle);

 /*! -------------------------------------------------------------------------
 * \brief Write a NTAG smart poster
 * \param[in] ntag_handle - handle to NTAG device
 * \param[in] pNDEFMsg - pointer to NDEF message(s)
 * \param[in] length - lenght of the smart poster
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_WriteTag(NFC_HANDLE_T ntag_handle,
                          const uint8_t* pNDEFMsg,
                          uint16_t length);

/*! -------------------------------------------------------------------------
 * \brief take a 64-bit number and convert it to hex string
 * \param[in] num - the 64-bit number
 * \param[out] pBuff - pointer to location when the hexadecimal string is stored
 * \return the length of the converted string
 *---------------------------------------------------------------------------*/
static uint32_t U64ToHexStr(uint64_t num, char* buff);
#endif /* APP_NTAG_DEVICE_IS_ZED_d */

#if APP_NTAG_DEVICE_IS_COORD_d
 /*! -------------------------------------------------------------------------
 * \brief NTAG events processing timer callback
 * \param[in] param - timer callback param (ignored)
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_ProcessingTmrCallback(void* param);

/*! -------------------------------------------------------------------------
 * \brief Invalidate the install code. Called after a valid iCode is read and
 *        stored
 * \param[in] ntag_handle - handle to NTAG device
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_InvalidateInstallCode(NFC_HANDLE_T ntag_handle);
#endif /* APP_NTAG_DEVICE_IS_COORD_d */

#if APP_NTAG_SUPPORT_FACTORY_FORMAT_d
/*! -------------------------------------------------------------------------
 * \brief Full erase of the tag
 * \param[in] ntag_handle - handle to NTAG device
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_FactoryFormat(NFC_HANDLE_T ntag_handle);
#endif

/*! -------------------------------------------------------------------------
 * \brief take a hex string and convert it to a 64-bit number
 * \param[in] hex - hexadecimal input string
 * \return the UINT64 value
 *---------------------------------------------------------------------------*/
static uint64_t hexStringToU64(char *hex);

#endif /* APP_NTAG_I2C_PLUS */

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Definitions / Functions                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

#ifdef APP_NTAG_I2C_PLUS

/* NTAG handle */
static NFC_HANDLE_T mNtagHandle;

/* NTAG commissioning data */
NtagCommissioningData_t mNtagCommissioningData;

/* Install Code identifier */
static const uint8_t au8InstallCodeIdentifier_c[] = "ZB_INSTALL_CODE";

/* IEEE 802.15.4 Extended Address identifier */
static const uint8_t au8ExtendedAddrIdentifier_c[] = "DEV_EXT_ADDR";

#if APP_NTAG_FD_INTERRUPT_ENABLE_d
/* FD interrupt pin */
static gpioInputPinConfig_t mFD_Pin = {
    .gpioPort = gpioPort_C_c,
    .gpioPin = 17,
    .pullSelect = pinPull_Up_c,
    .interruptSelect = pinInt_EitherEdge_c
};
#endif

#if APP_NTAG_DEVICE_IS_COORD_d

/* function pointer used to store the application callback */
static ptrAppCbk ptrAppNtagCallback;

/* flag used to signal the FD presence */
static volatile bool_t bIsFieldPresent;

/* flag used to signal that an NFC event is active */
static volatile bool_t bNfcEventActive;

/* NTAG processing timer ID */
static tmrTimerID_t mAppNtagTimerId;

/* Flag that controls the invalidation of the tag */
static bool_t mInvalidateTag = FALSE;

/* Tag invalidation timeout */
static uint8_t mInvalidateTagTimeout = gInvalidateTagDefaultTimeout_c;

#endif /* APP_NTAG_DEVICE_IS_COORD_d */

#if APP_NTAG_SUPPORT_FULL_MEM_READ_d

/* NTAG memory buffer used for memory full read operation */
static uint8_t au8NTAGMemory[NTAG_MEM_ADDR_USER_MEMORY_SIZE_c];

#endif /* APP_NTAG_SUPPORT_FULL_MEM_READ_d */

/* hex char array used for various data types conversions */
const char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' };

#endif /* APP_NTAG_I2C_PLUS */

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/*! -------------------------------------------------------------------------
 * \brief Initialize the NTAG
 * \param[in] -
 * \return TRUE if init was successfully, FALSE otherwise
 *---------------------------------------------------------------------------*/
bool_t APP_NTAG_Init(void)
{
#ifdef APP_NTAG_I2C_PLUS

#if APP_NTAG_DEVICE_IS_ZED_d
        uint64 u64ExtAddress;
        uint8_t au8InstallCode[NTAG_PAYLOAD_KEY_SIZE];
        bool_t retVal;
#endif

/* initialize module local variables */
#if APP_NTAG_DEVICE_IS_COORD_d
        bIsFieldPresent = FALSE;
        ptrAppNtagCallback = NULL;
#endif

        /* Initialize I2C bus */
        BOARD_InitI2C();

        /* Initialize I2C */
        HAL_I2C_InitDevice(HAL_I2C_INIT_DEFAULT, I2C_MASTER_CLK_SRC, NTAG_I2C_MASTER_BASEADDR);

        /* Update system core clock */
        SystemCoreClockUpdate();

        /* Initialize the NTAG I2C components */
        mNtagHandle = NFC_InitDevice((NTAG_ID_T)0, NTAG_I2C_MASTER_BASEADDR);

        if(NTAG_OK != mNtagHandle->status)
        {
          return FALSE;
        }

#if APP_NTAG_FD_INTERRUPT_ENABLE_d
        if(gpio_success != GpioInstallIsr(APP_NTAG_FD_ISR, gGpioIsrPrioLow_c, gGpioDefaultNvicPrio_c >> 1, &mFD_Pin))
        {
          return FALSE;
        }
        GpioInputPinInit(&mFD_Pin, 1);
#endif

#if APP_NTAG_FD_INTERRUPT_ENABLE_d
        /* Setup FD ON/OFF interrupt behavior */
        NFC_SetFDOffFunction(mNtagHandle, LAST_NDEF_BLOCK_READ_OR_RF_SWITCHED_OFF_10b);
        NFC_SetFDOnFunction(mNtagHandle, FIRST_VALID_SoF_01b);
#endif

#if APP_NTAG_DEVICE_IS_ZED_d

        /* Get device extended address */
        u64ExtAddress = ZPS_u64AplZdoGetIeeeAddr();

        if(FALSE == (retVal = bGetInstallCode(au8InstallCode)))
        {
          return retVal;
        }

        if(FALSE == APP_NTAG_CheckNdefMsg(mNtagHandle))
        {
#if APP_NTAG_SUPPORT_FACTORY_FORMAT_d
          APP_NTAG_FactoryFormat(mNtagHandle);
#else
          APP_NTAG_ClearTag(mNtagHandle);
#endif
          /* update the commissioning data */
          mNtagCommissioningData.u64ExtAddress = u64ExtAddress;
          FLib_MemCpy((uint8_t*)mNtagCommissioningData.au8Key, (uint8_t*)au8InstallCode, NTAG_PAYLOAD_KEY_SIZE);

          /* update the tag */
          APP_NTAG_UpdateTag(mNtagHandle);
        }
        else
        {
          /* read the commissioning data */
          APP_NTAG_ReadCommissioningData(mNtagHandle);

          /* check if same as the ones from HW params */
          if((u64ExtAddress == mNtagCommissioningData.u64ExtAddress) &&
             FLib_MemCmp((uint8_t*)au8InstallCode, (uint8_t*)mNtagCommissioningData.au8Key, NTAG_PAYLOAD_KEY_SIZE))
          {
            /* same, no need to update the tag */
          }
          else
          {
            /* update the commissioning data */
            mNtagCommissioningData.u64ExtAddress = u64ExtAddress;
            FLib_MemCpy((uint8_t*)mNtagCommissioningData.au8Key, (uint8_t*)au8InstallCode, NTAG_PAYLOAD_KEY_SIZE);

            /* update the tag */
            APP_NTAG_UpdateTag(mNtagHandle);
          }
        }
#endif /* APP_NTAG_DEVICE_IS_ZED_d */

#if APP_NTAG_DEVICE_IS_COORD_d
        /* Allocate a timer for NTAG processing */
        mAppNtagTimerId = TMR_AllocateTimer();

        if(gTmrInvalidTimerID_c == mAppNtagTimerId)
        {
          return FALSE;
        }

        /* Enable SRAM buffer mapping with Memory Mirror */
        NTAG_SetSRAMMirrorBlock(mNtagHandle, (uint8_t)(gZB_INSTALL_CODE_PAGE_c / NTAG_I2C_BLOCK_SIZE));

        /* start NTAG processing timer */
        TMR_StartIntervalTimer(mAppNtagTimerId, 1000, APP_NTAG_ProcessingTmrCallback, NULL);
#endif

        return TRUE;
#else
        return FALSE;
#endif /* APP_NTAG_I2C_PLUS */
}

/*! -------------------------------------------------------------------------
 * \brief Retrieves the NTAG handle
 * \param[in] -
 * \return the NTAG handle
 *---------------------------------------------------------------------------*/
NFC_HANDLE_T APP_NTAG_GetHandle(void)
{
#ifdef APP_NTAG_I2C_PLUS
  return mNtagHandle;
#else
  return NULL;
#endif
}

/*! -------------------------------------------------------------------------
 * \brief Registers a user callback
 * \param[in] the callback funtion to be registered and called
 * \return -
 *---------------------------------------------------------------------------*/
void APP_NTAG_RegisterCallback(ptrAppCbk cbk)
{
#ifdef APP_NTAG_I2C_PLUS
  #if APP_NTAG_DEVICE_IS_COORD_d
  if(cbk)
  {
    ptrAppNtagCallback = cbk;
  }
  #endif
#endif
}

/*! -------------------------------------------------------------------------
 * \brief Retrieves the Install Code (iCode)
 * \param[in] -
 * \return pointer to iCode
 *---------------------------------------------------------------------------*/
uint8_t* APP_NTAG_GetInstallCode(void)
{
#ifdef APP_NTAG_I2C_PLUS
  return (uint8_t*)mNtagCommissioningData.au8Key;
#else
  return NULL;
#endif
}

/*! -------------------------------------------------------------------------
 * \brief Retrieves the device Extended Address
 * \param[in] -
 * \return the Extended Address
 *---------------------------------------------------------------------------*/
uint64_t APP_NTAG_GetDeviceExtAddress(void)
{
#ifdef APP_NTAG_I2C_PLUS
    return mNtagCommissioningData.u64ExtAddress;
#else
  return 0;
#endif
}

/*! -------------------------------------------------------------------------
 * \brief NTAG full memory read (for debugging purposes)
 * \param[in] ntag_handle - handle to NTAG
 * \param[in] pData - pointer where the
 * \return -
 *---------------------------------------------------------------------------*/
void APP_NTAG_ReadMemoryFull(NFC_HANDLE_T ntag_handle, uint8_t* pData)
{
#ifdef APP_NTAG_I2C_PLUS
  #if APP_NTAG_SUPPORT_FULL_MEM_READ_d
  NFC_ReadBytes(ntag_handle,
                NTAG_MEM_ADDR_START_USER_MEMORY,
                (uint8_t*)pData,
                NTAG_MEM_ADDR_USER_MEMORY_SIZE_c-1);
  #endif /* APP_NTAG_SUPPORT_FULL_MEM_READ_d */
#endif /* APP_NTAG_I2C_PLUS */
}


/****************************************************************************/
/***                  Local Function                                      ***/
/****************************************************************************/

#ifdef APP_NTAG_I2C_PLUS

/*! -------------------------------------------------------------------------
 * \brief Full erase of the tag
 * \param[in] ntag_handle - handle to NTAG device
 * \return -
 *---------------------------------------------------------------------------*/
#if APP_NTAG_SUPPORT_FACTORY_FORMAT_d
static void APP_NTAG_FactoryFormat(NFC_HANDLE_T ntag_handle)
{
    uint8_t page = 1;

    while (page < 56)
    {
        NFC_WriteBlock(ntag_handle, page, Null_Block, NTAG_I2C_BLOCK_SIZE);
        page++;
    }

    /* reset pages 56,57,58 */
    NFC_WriteBlock(ntag_handle, 56, Default_Page_56, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 57, Default_Page_57, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 58, Default_Page_58, NTAG_I2C_BLOCK_SIZE);
}
#endif

/*! -------------------------------------------------------------------------
 * \brief Read the Zigbee 3.0 install code from the tag
 * \param[in] ntag_handle - handle to NTAG device
 * \param[out] ptrInstallCode - pointer to a memory location where the iCode
 *                             will be stored
 * \return -
 *---------------------------------------------------------------------------*/
#if (APP_NTAG_DEVICE_IS_COORD_d || APP_NTAG_DEVICE_IS_ZED_d)
static bool_t APP_NTAG_ReadCommissioningData(NFC_HANDLE_T ntag_handle)
{
  uint8 val;
  uint8_t loopCnt;
  uint8_t installCodeHexStr[gZB_INSTALL_CODE_LEN_c];
  uint8_t extAddrHexStr[gZB_EXT_ADDR_LEN_c + 1];
  uint8_t* token = installCodeHexStr;

  uint8_t* pBuff = MEM_BufferAlloc(sizeof(au8NTAG_NDEF_Msg));

  if(NULL == pBuff)
  {
    return FALSE;
  }

  NFC_ReadBytes(ntag_handle, (uint16_t)(NTAG_MEM_ADDR_START_USER_MEMORY), pBuff, NTAG_NDEF_MSG_LEN_c);

  FLib_MemCpy(installCodeHexStr, (uint8_t*)(&pBuff[gZB_INSTALL_CODE_IDX_c]), gZB_INSTALL_CODE_LEN_c);
  FLib_MemCpy(extAddrHexStr, (uint8_t*)(&pBuff[gZB_EXT_ADDR_IDX_c]), gZB_EXT_ADDR_LEN_c);

  MEM_BufferFree(pBuff);

  extAddrHexStr[gZB_EXT_ADDR_LEN_c] = '\0';
  mNtagCommissioningData.u64ExtAddress = hexStringToU64((char *)extAddrHexStr);

  for (loopCnt=0; loopCnt<NTAG_PAYLOAD_KEY_SIZE; loopCnt++)
  {
      val = (*token >= 'A')? *token - 'A' + 10: *token - '0';
      mNtagCommissioningData.au8Key[loopCnt] = 16  * val;
      token++;
      val = (*token >= 'A')? *token - 'A' + 10: *token - '0';
      mNtagCommissioningData.au8Key[loopCnt] += val;
      token++;
  }

  return TRUE;
}
#endif

/*! -------------------------------------------------------------------------
 * \brief Checks if the NDEF messages of the NTAG smart poster are compliant
 *        with the defined format
 * \param[in] ntag_handle - handle to NTAG device
 * \return TRUE if the smart poster is compliant, FALSE otherwise
 *---------------------------------------------------------------------------*/
#if (APP_NTAG_DEVICE_IS_COORD_d || APP_NTAG_DEVICE_IS_ZED_d)
static bool_t APP_NTAG_CheckNdefMsg(NFC_HANDLE_T ntag_handle)
{
  uint8_t* pBuff = MEM_BufferAlloc(sizeof(au8NTAG_NDEF_Msg));

  if(NULL == pBuff)
  {
    return FALSE;
  }

  NFC_ReadBytes(ntag_handle, (uint16_t)(NTAG_MEM_ADDR_START_USER_MEMORY), pBuff, NTAG_NDEF_MSG_LEN_c);

  if(!FLib_MemCmp((uint8_t *)(&pBuff[gZB_INSTALL_CODE_IDENTIFIER_IDX_c]), (uint8_t *)au8InstallCodeIdentifier_c, gZB_INSTALL_CODE_IDENTIFIER_LEN_c))
  {
    return FALSE;
  }

  if(!FLib_MemCmp((uint8_t *)(&pBuff[gZB_EXT_ADDR_IDENTIFIER_IDX_c]), (uint8_t *)au8ExtendedAddrIdentifier_c, gZB_EXT_ADDR_IDENTIFIER_LEN_c))
  {
    return FALSE;
  }

  MEM_BufferFree(pBuff);

  return TRUE;
}
#endif

#if APP_NTAG_DEVICE_IS_ZED_d
/*! -------------------------------------------------------------------------
 * \brief Clear NDEF message from tag
 * \param[in] ntag_handle - handle to NTAG device
 * \return TRUE if operation was successfull, FALSE otherwise
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_ClearTag(NFC_HANDLE_T ntag_handle)
{
  uint8_t* pBuff = MEM_BufferAlloc(sizeof(au8NTAG_NDEF_Msg));

  if(NULL == pBuff)
  {
    return FALSE;
  }

  FLib_MemSet(pBuff, 0, sizeof(au8NTAG_NDEF_Msg));

  /* Write empty NDEF */
  NFC_WriteBytes(ntag_handle, NTAG_MEM_ADDR_START_USER_MEMORY, pBuff, sizeof(au8NTAG_NDEF_Msg));

  MEM_BufferFree(pBuff);

  return TRUE;
}

/*! -------------------------------------------------------------------------
 * \brief Updates the NTAG handle
 * \param[in] the NTAG handle
 * \return TRUE if tag was updated successfully, FALSE otherwise
 *---------------------------------------------------------------------------*/
static bool_t APP_NTAG_UpdateTag(NFC_HANDLE_T ntag_handle)
{
  uint8_t extAddrHex[gZB_EXT_ADDR_LEN_c+1];
  uint8_t installKeyHex[gZB_INSTALL_CODE_LEN_c];
  uint32_t len;
  uint32_t loopCnt;
  uint8_t idx;

  uint8_t* pBuff = MEM_BufferAlloc(sizeof(au8NTAG_NDEF_Msg));

  if(NULL == pBuff)
  {
    return FALSE;
  }

  /* get the extended address into hex string format */
  len = U64ToHexStr(mNtagCommissioningData.u64ExtAddress, extAddrHex);

  if(0 == len)
  {
    return FALSE;
  }

  /* get the install code into hex string format */
  idx = 0;
  for(loopCnt = 0; loopCnt < NTAG_PAYLOAD_KEY_SIZE; loopCnt++)
  {
    uint8_t buff[2];
    uint8_t data = mNtagCommissioningData.au8Key[loopCnt];

    /* get the hex string in local buffer */
    buff[0] = hex[data & 0xF];
    data >>= 4;
    buff[1] = hex[data & 0xF];

    /* swap bytes */
    installKeyHex[idx++] = buff[1];
    installKeyHex[idx++] = buff[0];

    if(idx >= gZB_INSTALL_CODE_LEN_c)
    {
      break; /* should never get here */
    }
  }

  /* update the tag info */
  FLib_MemCpy((uint8_t*)pBuff, (uint8_t*)au8NTAG_NDEF_Msg, NTAG_NDEF_MSG_LEN_c);
  FLib_MemCpy((uint8_t*)(&pBuff[gZB_INSTALL_CODE_IDX_c]), installKeyHex, gZB_INSTALL_CODE_LEN_c);
  FLib_MemCpy((uint8_t*)(&pBuff[gZB_EXT_ADDR_IDX_c]), extAddrHex, gZB_EXT_ADDR_LEN_c);

  /* write the tag */
  APP_NTAG_WriteTag(ntag_handle, pBuff, sizeof(au8NTAG_NDEF_Msg));

  /* free buffer */
  MEM_BufferFree(pBuff);

  /* all good, return */
  return TRUE;
}

/*! -------------------------------------------------------------------------
 * \brief Write NTAG
 * \param[in] ntag_handle - handle to NTAG device
 * \param[in] pNDEFMsg - pointer to NDEF message(s)
 * \param[in] length - lenght of the smart poster
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_WriteTag(NFC_HANDLE_T ntag_handle, const uint8_t* pNDEFMsg, uint16_t length)
{
    OSA_TimeDelay(100);

    /* Write NDEF message(s) */
    NFC_WriteBytes(ntag_handle, NTAG_MEM_ADDR_START_USER_MEMORY, (uint8_t*)pNDEFMsg, length);

    /* reset all the remaining pages until page 56 */
    uint8_t page = length >> 4;
    page++;
    if(length % NTAG_I2C_BLOCK_SIZE)
    {
      page++;
    }

    while (page < 56)
    {
        NFC_WriteBlock(ntag_handle, page, Null_Block, NTAG_I2C_BLOCK_SIZE);
        page++;
    }

    /* reset pages 56,57,58 */
    NFC_WriteBlock(ntag_handle, 56, Default_Page_56, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 57, Default_Page_57, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 58, Default_Page_58, NTAG_I2C_BLOCK_SIZE);

    OSA_TimeDelay(100);
}
#endif

/*! -------------------------------------------------------------------------
 * \brief NTAG events processing timer callback
 * \param[in] param - timer callback param (ignored)
 * \return -
 *---------------------------------------------------------------------------*/
#if APP_NTAG_DEVICE_IS_COORD_d
static void APP_NTAG_ProcessingTmrCallback(void* param)
{
  (void)param;

  /* initialize with invalid iCode event  */
  APP_NTAG_Events_t event = NTAG_ICODE_INVALID_c;

  if(mInvalidateTag)
  {
    if(--mInvalidateTagTimeout == 0)
    {
      /* invalidate the install code from the tag to prevent further reads */
      APP_NTAG_InvalidateInstallCode(mNtagHandle);
      /* clear the flag */
      mInvalidateTag = FALSE;
    }
  }

  if(bNfcEventActive)
  {
    if(!bIsFieldPresent) /* NFC device removed from the field, so it's supposed that it might be written */
    {
      /* check if there is a compliant NDEF Message */
      if(APP_NTAG_CheckNdefMsg(mNtagHandle))
      {
        /* read the Zigbee Install Code */
        if(APP_NTAG_ReadCommissioningData(mNtagHandle))
        {
          /* set the valid iCode event  */
          event = NTAG_ICODE_VALID_c;
          /* request tag invalidation */
          mInvalidateTag = TRUE;
          /* set the tag invalidation timeout to default */
          mInvalidateTagTimeout = gInvalidateTagDefaultTimeout_c;
        }
      }

      /* inform the application if there is a valid callback provided */
      if(ptrAppNtagCallback)
      {
        ptrAppNtagCallback(event);
      }

      /* reset the event active flag */
      bNfcEventActive = FALSE;
    }
  }
}
#endif

#if APP_NTAG_DEVICE_IS_COORD_d
/*! -------------------------------------------------------------------------
 * \brief Invalidate the install code. Called after a valid iCode is read and
 *        stored
 * \param[in] ntag_handle - handle to NTAG device
 * \return -
 *---------------------------------------------------------------------------*/
static void APP_NTAG_InvalidateInstallCode(NFC_HANDLE_T ntag_handle)
{
  const uint8_t buff[MAX(gZB_INSTALL_CODE_LEN_c, gZB_EXT_ADDR_LEN_c)] = {0};

  NFC_WriteBytes(ntag_handle,
                 (uint16_t)(NTAG_MEM_ADDR_START_USER_MEMORY + gZB_INSTALL_CODE_IDX_c),
                 (uint8_t*)buff,
                  gZB_INSTALL_CODE_LEN_c);

  NFC_WriteBytes(ntag_handle,
               (uint16_t)(NTAG_MEM_ADDR_START_USER_MEMORY + gZB_EXT_ADDR_IDX_c),
               (uint8_t*)buff,
               gZB_EXT_ADDR_LEN_c);
}
#endif

/*! -------------------------------------------------------------------------
 * \brief NTAG Field Detect (FD) interrupt service routine
 * \param[in] -
 * \return -
 *---------------------------------------------------------------------------*/
#if APP_NTAG_FD_INTERRUPT_ENABLE_d
static void APP_NTAG_FD_ISR(void)
{
    if(GpioIsPinIntPending(&mFD_Pin))
    {
      /* clear the interrupt status flag */
      GpioClearPinIntFlag(&mFD_Pin);

      /* check if RF field present */
      bIsFieldPresent = !GpioReadPinInput(&mFD_Pin);
      bNfcEventActive = TRUE;
    }
}
#endif

/*! -------------------------------------------------------------------------
 * \brief take a hex string and convert it to a 64-bit number
 * \param[in] hex - hexadecimal input string
 * \return the UINT64 value
 *---------------------------------------------------------------------------*/
static uint64_t hexStringToU64(char *hex)
{
    uint64_t val = 0;
    uint8_t byte;

    while (*hex)
    {
        byte = *hex++;
        if (byte >= '0' && byte <= '9')
        {
          byte = byte - '0';
        }
        else if (byte >= 'a' && byte <='f')
        {
          byte = byte - 'a' + 10;
        }
        else if (byte >= 'A' && byte <='F')
        {
          byte = byte - 'A' + 10;
        }

        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

#if APP_NTAG_DEVICE_IS_ZED_d
/*! -------------------------------------------------------------------------
 * \brief take a 64-bit number and convert it to hex string
 * \param[in] num - the 64-bit number
 * \param[out] pBuff - pointer to location when the hexadecimal string is stored
 * \return the length of the converted string
 *---------------------------------------------------------------------------*/
static uint32_t U64ToHexStr(uint64_t num, char* pBuff)
{
    uint32_t len = 0;
    uint32_t loopCnt = 0;

    /* get the hex string */
    do
    {
        pBuff[len] = hex[num & 0xF];
        len++;
        num >>= 4;
    } while(num!=0);

    /* reverse the order */
    for(; loopCnt<len/2; loopCnt++)
    {
        pBuff[loopCnt] ^= pBuff[len-loopCnt-1];
        pBuff[len-loopCnt-1] ^= pBuff[loopCnt];
        pBuff[loopCnt] ^= pBuff[len-loopCnt-1];
    }
    pBuff[len]='\0';
    return len;
}
#endif

#endif /* APP_NTAG_I2C_PLUS */