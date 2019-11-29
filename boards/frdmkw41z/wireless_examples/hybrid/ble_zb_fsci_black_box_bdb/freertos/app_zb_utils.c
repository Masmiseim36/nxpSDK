/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_zb_utils.c
\brief      This is a public source file for the zigbee utils for demo application.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include <jendefs.h>

#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "SecLib.h"
#include "fsl_common.h"
#include "Flash_Adapter.h"
#include "mac_vs_sap.h"
#include "SerialManager.h"
#include "MemManager.h"
#include "ModuleInfo.h"
#include "LED.h" 
#include "app_zb_utils.h"
#include "fsl_os_abstraction.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#define BUFFER_SIZE 24

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/* None */

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
uint8     APP_SerialId = gSerialMgrInvalidIdx_c;

/* The following definitions are required by the VERSION_TAGS. DO NOT MODIFY or REMOVE */
extern const moduleInfo_t ZPS_APL_version;
extern const moduleInfo_t ZPS_NWK_version;
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma required=ZPS_APL_version /* force the linker to keep the symbol in the current compilation unit */
uint8_t zps_apl_dummy; /* symbol suppressed by the linker as it is unused in the compilation unit, but necessary because 
                             to avoid warnings related to #pragma required */
#pragma required=ZPS_NWK_version /* force the linker to keep the symbol in the current compilation unit */
uint8_t zps_nwk_dummy; /* symbol suppressed by the linker as it is unused in the compilation unit, but necessary because 
                             to avoid warnings related to #pragma required */
#elif defined(__GNUC__)
static const moduleInfo_t *const zps_apl_dummy __attribute__((__used__)) = &ZPS_APL_version;
static const moduleInfo_t *const zps_nwk_dummy __attribute__((__used__)) = &ZPS_NWK_version;
#endif /* __IAR_SYSTEMS_ICC__ */


/*==================================================================================================
Private prototypes
==================================================================================================*/
static void vWriteUIntToBuffer(unsigned long long ullInteger,
                               int iBase,
                               char acOutBuffer[BUFFER_SIZE],
                               int *piOutBufferStart,
                               int *piOutBufferSize);


/*==================================================================================================
Public functions
==================================================================================================*/

/****************************************************************************
 *
 * NAME: APP_vSetMacAddr
 *
 * DESCRIPTION:
 * Set extendded mac address
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void APP_vSetMacAddr(void)
{
    uint32_t gaUniqueId[4];
    uint32_t macPrefix, macSuffix;
    uint64_t u64IeeeAddr = gMacAddress_c;
    
    if(FLib_MemCmpToVal(&u64IeeeAddr, 0xFF, sizeof(u64IeeeAddr)))
    {
#if defined(SIM_UIDH)
        gaUniqueId[0] = SIM->UIDH;
#else
        gaUniqueId[0] = 0xFFFFFFFFU;
#endif
        gaUniqueId[1] = SIM->UIDMH;
        gaUniqueId[2] = SIM->UIDML;
        gaUniqueId[3] = SIM->UIDL;
        
        if ((gaUniqueId[3] == 0xFFFFFFFFU) && (gaUniqueId[2] == 0xFFFFFFFFU))
        {
            /* Generate random data */
            RNG_GetRandomNo(&macPrefix);
            RNG_GetRandomNo(&macSuffix);
        }
        else
        {
            sha256Context_t mCtx;
            SHA256_Hash(&mCtx, (uint8_t*)gaUniqueId, sizeof(gaUniqueId));
            FLib_MemCpy(&macSuffix, (uint8_t*)(&mCtx.hash), sizeof(macSuffix));
            FLib_MemCpy(&macPrefix, ((uint8_t*)(&mCtx.hash))+4, sizeof(macPrefix));
        }
        u64IeeeAddr = (((uint64_t)macPrefix) << 32U) | macSuffix;
    }
    
	vAppApiSetMacAddrLocation(&u64IeeeAddr);
}

/****************************************************************************
 *
 * NAME: bGetInstallCode
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
bool bGetInstallCode(uint8_t* pInstallCode)
{
    uint32_t status;
    uint64_t ieeeAddr;
    bool_t result = FALSE;
    
    /* Read the install code from flash. */
    status = NV_ReadHWParameters(&gHardwareParameters);
    
    if(status == 0)
    {
        /* Use the install code from flash if set. */
        if(!FLib_MemCmpToVal(gHardwareParameters.zbInstallCode, 0xFF, sizeof(gHardwareParameters.zbInstallCode)))
        {
            FLib_MemCpy(pInstallCode, gHardwareParameters.zbInstallCode, 16);
            result = TRUE;
        }
    }
    
    if(result == FALSE)
    {
        /* Install code could not be obtained from flash. Generate install code */
        ieeeAddr =  MAC_u64GetExtAddr();
        
        /* Generate an install code that is the MAC address repetead once. */
        FLib_MemCpyReverseOrder(pInstallCode, &ieeeAddr, sizeof(uint64_t));
        FLib_MemCpyReverseOrder(pInstallCode + sizeof(uint64_t), &ieeeAddr, sizeof(uint64_t));
        result = TRUE;

    }
    
    return result;
}
#if !cPWR_UsePowerDownMode 
/****************************************************************************
*
* NAME: APP_vSetLedState
*
* DESCRIPTION:
* Device state
*
* RETURNS:
* void
*
****************************************************************************/
void APP_vSetLedState(bool_t bDeviceOnTheNwk)
{
    static uint8_t ledState = 3;
    if(ledState != (uint8_t)bDeviceOnTheNwk)
    {
        if(TRUE == bDeviceOnTheNwk)
        {

            StopLed1Flashing();         
            Led1On();
        }
        else
        {

            Led1Flashing();
        }
        ledState = (uint8_t)bDeviceOnTheNwk;
    }    
}
#endif
/****************************************************************************
*
* NAME: vWriteUIntToBuffer
*
* DESCRIPTION:
* Converts a number of the specified base to a string
*
* RETURNS:
* void
*
****************************************************************************/
static void vWriteUIntToBuffer(unsigned long long ullInteger,
                               int iBase,
                               char acOutBuffer[BUFFER_SIZE],
                               int *piOutBufferStart,
                               int *piOutBufferSize)
{
    int i = BUFFER_SIZE;
    
    do
    {
        /* cheap way to get a mod and a divide together -
        * multiply is way cheaper! */
        unsigned long long ullIntegerTmp = ullInteger / iBase;
        unsigned int uCurrentDigit = ullInteger - ullIntegerTmp * iBase;
        ullInteger = ullIntegerTmp;
        
        i--;
        if(uCurrentDigit < 0xA)
        {
            acOutBuffer[i] = '0' + uCurrentDigit;
        }
        else
        {
            acOutBuffer[i] = 'a' + uCurrentDigit - 0xA;
        }
        
    } while(ullInteger);
    
    *piOutBufferStart = i;
    *piOutBufferSize = BUFFER_SIZE - i;
}

/****************************************************************************
*
* NAME: pccHandlePlaceHolder
*
* DESCRIPTION:
* Handles format specifiers
*
* RETURNS:
* char *
*
****************************************************************************/
char *pccHandlePlaceHolder(char *pcFormat, va_list *pap, char * pOut, int* n)
{
    char acBuffer[BUFFER_SIZE];
    int i = 0;
    
    bool_t bLeftAlign           = FALSE;
    bool_t bPadWith0NotSpace    = FALSE;
    bool_t bSignWithPlus        = FALSE;
    bool_t bSignWithSpace       = FALSE;
    
    int iMinWidth = 0;
    
    bool_t bIsShort    = FALSE;
    bool_t bIsLong     = FALSE;
    bool_t bIsLongLong = FALSE;
    
    unsigned long long ullArg = 0;
    
    
    /* flags */
    while(1)
    {
        bool_t bQuit = FALSE;
        
        switch(*pcFormat)
        {
        case '-':
            bLeftAlign = TRUE;
            break;
        case '0':
            bPadWith0NotSpace = TRUE;
            break;
        case '+':
            bSignWithPlus = TRUE;
            break;
        case ' ':
            bSignWithSpace = TRUE;
            break;
        default:
            bQuit = TRUE;
            break;
        }
        
        if(bQuit)
        {
            break;
        }
        pcFormat++;
    }
    
    /* min field width */
    if (*pcFormat >= '1' && *pcFormat <= '9')
    {
        while(1)
        {
            int iThisDigit = *pcFormat - '0';
            
            if(iThisDigit < 0 || iThisDigit > 9)
            {
                break;
            }
            
            pcFormat++;
            
            iMinWidth *= 10;
            iMinWidth += iThisDigit;
        }
    }
    
    /* size modifier */
    if (*pcFormat == 'l')
    {
        /* long */
        pcFormat++;
        bIsLong = TRUE;
        
        if(*pcFormat == 'l')
        {
            /* long long */
            pcFormat++;
            bIsLongLong = TRUE;
        }
    }
    else if (*pcFormat == 'h')
    {
        /* short */
        bIsShort = TRUE;
        pcFormat++;
    }
    
    if(*pcFormat == 'i' ||
       *pcFormat == 'd' ||
       *pcFormat == 'u' ||
       *pcFormat == 'x' ||
       *pcFormat == 'p')
    {
        int iStart = 0;
        int iSize = 0;
        
        bool_t bIsSigned = FALSE;
        bool_t bIsHex = FALSE;
        bool_t bIsNegative = FALSE;
        bool_t bIsPtr = FALSE;
        
        char cSign = '\0';
        
        if(*pcFormat == 'i' ||
           *pcFormat == 'd')
        {
            bIsSigned = TRUE;
        }
        else if(*pcFormat == 'x')
        {
            bIsHex = TRUE;
        }
        else if(*pcFormat == 'p')
        {
            bIsPtr = TRUE;
            
            bIsLongLong = (sizeof(void *) >= sizeof(long long));
            bIsLong     = (sizeof(void *) >= sizeof(long));
        }
        
        if(!bIsSigned)
        {
            bSignWithPlus = FALSE;
            bSignWithSpace = FALSE;
        }
        
        if(bIsLongLong)
        {
            if(bIsSigned)
            {
                ullArg = va_arg(*pap, long long);
            }
            else
            {
                ullArg = va_arg(*pap, unsigned long long);
            }
        }
        else if(bIsLong)
        {
            if(bIsSigned)
            {
                ullArg = va_arg(*pap, long);
            }
            else
            {
                ullArg = va_arg(*pap, unsigned long);
            }
        }
        else if(bIsShort)
        {
            /* take account of lower limits */
            if(bIsSigned)
            {
                short s = va_arg(*pap, int);
                ullArg = s;
            }
            else
            {
                unsigned short us = va_arg(*pap, unsigned int);
                ullArg = us;
            }
        }
        else
        {
            if(bIsSigned)
            {
                ullArg = va_arg(*pap, int);
            }
            else
            {
                ullArg = va_arg(*pap, unsigned int);
            }
        }
        
        if(bIsSigned)
        {
            long long llArg = ullArg;
            if(llArg < 0)
            {
                bIsNegative = TRUE;
                
                /* scalar value */
                ullArg = 0 - ullArg;
            }
        }
        
        vWriteUIntToBuffer(ullArg,
                           (bIsHex || bIsPtr) ? 16 : 10,
                           acBuffer,
                           &iStart,
                           &iSize);
        
        if ((iSize > *n) || (iMinWidth > *n))
        {
            /* Not enough space */
            *n = 0;
            return pcFormat;
        }
         
        if(bIsNegative)
        {
            cSign = '-';
        }
        else
        {
            if(bSignWithPlus)
            {
                cSign = '+';
            }
            else if(bSignWithSpace)
            {
                cSign = ' ';
            }
        }
        
        iMinWidth -= iSize;
        
        if(cSign)
        {
            iMinWidth--;
        }
        
        if(bIsPtr)
        {
            iMinWidth -= 2;
        }
        
        if(bLeftAlign)
        {
            if(cSign)
            {
                /* write the sign */
                pOut[i++] = (cSign);
            }
            
            if(bIsPtr)
            {
                pOut[i++] = ('0');
                pOut[i++] = ('x');
            }
            
            /* write the digits */
            for(;iSize > 0; iSize--)
            {
                pOut[i++] = (acBuffer[iStart++]);
            }
            
            /* write the pad char */
            for(; iMinWidth > 0; iMinWidth--)
            {
                /* note, '0' is ignored when the '-' flag is set */
                pOut[i++] = (' ');
            }
        }
        else
        {
            if(bPadWith0NotSpace)
            {
                if(cSign)
                {
                    pOut[i++] = (cSign);
                }
                
                if(bIsPtr)
                {
                    pOut[i++] = ('0');
                    pOut[i++] = ('x');
                }
                
                /* write the pad char */
                for(; iMinWidth > 0; iMinWidth--)
                {
                    pOut[i++] = ('0');
                }
                
                /* write the digits */
                for(;iSize > 0; iSize--)
                {
                    pOut[i++] = (acBuffer[iStart++]);
                }
            }
            else
            {
                /* write the pad char */
                for(; iMinWidth > 0; iMinWidth--)
                {
                    pOut[i++] = (' ');
                }
                
                if(cSign)
                {
                    pOut[i++] = (cSign);
                }
                
                if(bIsPtr)
                {
                    pOut[i++] = ('0');
                    pOut[i++] = ('x');
                }
                
                /* write the digits */
                for(;iSize > 0; iSize--)
                {
                    pOut[i++] = (acBuffer[iStart++]);
                }
            }
        }
    }
    else
    {
        char c = '\0';
        const char *pc = NULL;
        
        switch(*pcFormat)
        {
        case 'c':
            c = va_arg(*pap, int);
            if (c == '\n')
            {
                pOut[i++] = ('\r');
                pOut[i++] = ('\n');
            }
            else /* PR #21 http://trac/Cortex/ticket/21 - output character if it's not a new line */
            {
                pOut[i++] = (c);
            }
            break;
            
        case 's':
            {
                const char *pcc;
                pc = va_arg(*pap, char *);
                pcc = pc;
                for(;*pcc != '\0';pcc++)
                {
                    if(*pcc == '\n')
                    {
                        pOut[i++] = ('\r');
                        pOut[i++] = ('\n');
                    }
                    else
                    {
                        pOut[i++] = (*pcc);
                    }
                }
            }
            break;
            
        case '%':
            pOut[i++] = (*pcFormat);
            break;
        default:
            /* error */
            *n=0;
            return pcFormat - 1;
        }
    }
    
    *n = i;
    return pcFormat;
}

/****************************************************************************
*
* NAME: APP_vPrintfNullImpl
*
* DESCRIPTION:
* ARGS still get evaluated
*
* RETURNS:
* void
*
****************************************************************************/
void APP_vPrintfNullImpl(bool_t stream, ...)
{
     (void)stream;
     __asm("NOP");
}

/*! *********************************************************************************
* \brief  This function will output a formated string onto the serial interface
*
* \param[in]  fmt string defining the output
* \param[in]  ... variable number of parameters
*
* \remarks
*
********************************************************************************** */
void APP_vPrintf(char * fmt, ...)
{
    va_list ap;
    uint16_t n;
    uint8_t placeholders = 0;
    char *pStr;
    char *pCr = "\r";

    if ((APP_SerialId != gSerialMgrInvalidIdx_c) && (fmt != NULL))
    {
        /* Get size of string and check for placeholders */
        for (n=0; fmt[n] != '\0'; n++)
        {
            if (fmt[n] == '%')
            {
                placeholders++;
            }
        }

        if (placeholders == 0)
        {
            /* If there are no placeholders, just print the string */
            (void)Serial_AsyncWrite (APP_SerialId, (uint8_t*)fmt, n, NULL, NULL);
            /* Add CR. */
            if(fmt[n-1] == '\n')
            {
                (void)Serial_AsyncWrite (APP_SerialId, (uint8_t*)pCr, 1, NULL, NULL);
            }
        }
        else
        {
            /* Allocate a memory buffer to populate placeholders. Account for placeholders length */
            pStr = MEM_BufferAlloc(n + MAX_PARAM_LEN);
            
            if (pStr)
            {
                uint32_t i = 0;
                int len;
                
                n = MEM_BufferGetSize(pStr);
                
                va_start(ap, fmt);

                for ( ;*fmt != '\0'; fmt++)
                {
                    if (*fmt == '%')
                    {
                        len = n-i; /* remaining space */
                        fmt = pccHandlePlaceHolder (fmt + 1, &ap, &pStr[i], &len);
                        i += len;
                    }
                    else if ((*fmt == '\n') && (i < (n - 1)))
                    {
                        pStr[i++] = '\r';
                        pStr[i++] = '\n';
                    }
                    else if (i < n)
                    {
                        pStr[i++] = *fmt;
                    }
                }
                
                n = i;
                
                va_end(ap);

                if (gSerial_Success_c != Serial_AsyncWrite (APP_SerialId, (uint8_t*)pStr, n, (pSerialCallBack_t)MEM_BufferFree, pStr))
                {
                    MEM_BufferFree(pStr);
                }
            }
        }
    }
}

/*! *********************************************************************************
* \brief  Converts a hex character string into an unsigned integer (64-bit).
*
* \param[in]   str   Pointer to the hex character string
*
* \return  uint64_t the converted number
*
********************************************************************************** */
uint64_t hex_str_to_uint(char * str)
{
    uint64_t n = 0;

    if (str)
    {
        while (((*str >= '0') && (*str <= '9')) ||
               ((*str >= 'a') && (*str <= 'f')) ||
               ((*str >= 'A') && (*str <= 'F')))
        {
            n <<= 4;
            if ((*str >= '0') && (*str <= '9'))
            {
                n += (*str - '0');
            }
            else if ((*str >= 'a') && (*str <= 'f'))
            {
                n += (*str - 'a' + 10);
            }
            else
            {
                n += (*str - 'A' + 10);
            }
            str++;
        }
    }
    
    return n;
}

/*! *********************************************************************************
* \brief  Converts a decimal character string into an unsigned integer (64-bit).
*
* \param[in]   str   Pointer to the decimal character string
*
* \return  uint64_t the converted number
*
********************************************************************************** */
uint64_t dec_str_to_uint(char * str)
{
    uint64_t n = 0;
    
    if (str)
    {
        while ((*str >= '0') && (*str<='9'))
        {
            n = n * 10 + (*str - '0');
            str++;
        }
    }
    
    return n;
}

/*! *********************************************************************************
* \brief  Converts a hex character string into an array of bytes.
*
* \param[in]   str   Pointer to the hex character string
* \param[out]  pOut  Pointer to the output array
* \param[in]   max_len  Maximum number of elements in the output array
*
* \return  int16_t  -1 in case of invalid parameters
*                   -2 in case of array overflow
*                   >0 number of bytes converted
*
********************************************************************************** */
int16_t hex_str_to_array(char * str, uint8_t * pOut, uint16_t max_len)
{
    int16_t len = 0;
    uint8_t n = 0;

    if (!str || !pOut)
    {
        len = -2;
    }
    else
    {
        while (*str != '\0')
        {
            n <<= 4;
            
            if ((*str >= '0') && (*str <= '9'))
            {
                n += (*str - '0');
            }
            else if ((*str >= 'a') && (*str <= 'f'))
            {
                n += (*str - 'a' + 10);
            }
            else if ((*str >= 'A') && (*str <= 'F'))
            {
                n += (*str - 'A' + 10);
            }
            else
            {
                break;
            }
            
            str++;
            len++;
            
            if (!(len & 1))
            {
                if ((len >> 1) > max_len)
                {
                    len = -4;
                    break;
                }
                else
                {
                    *pOut++ = n;
                }
            }
        }
    }
    
    return len >> 1;
}
  
/*! *********************************************************************************
* \brief  HardFault_Handler
*
* \param[in]   none
*
* \return      none
*
********************************************************************************** */
void HardFault_Handler
(
    void
)
{
#if DEBUG
    while(1);
#else    
    __asm("BKPT #0\n") ; /* cause the debugger to stop */ 
#endif    
}
/*==================================================================================================
Private debug functions
==================================================================================================*/

