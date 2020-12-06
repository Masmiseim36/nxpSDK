/*
 * Amazon FreeRTOS OTA PAL V1.0.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/*
 * Copyright 2018-2020 NXP.
 */

/* C Runtime includes. */
#include <stdio.h>
#include <stdlib.h>

/* Amazon FreeRTOS include. */
#include "FreeRTOS.h"
#include "aws_iot_ota_pal.h"

/* Board specific includes */
#include "board.h"
#include "fica_definition.h"
#include "flash_ica_driver.h"
#include "sln_ota.h"
#include "sln_RT10xx_RGB_LED_driver.h"

/* Definitions */
#define IMG_TYPE_BOOT_PATH  "Boot"
#define IMG_TYPE_APP_A_PATH "AppA"
#define IMG_TYPE_APP_B_PATH "AppB"

static OTA_PAL_ImageState_t platformState = eOTA_PAL_ImageState_Unknown;
static TaskHandle_t s_otaDoneTaskHandle   = NULL;

/* Specify the OTA signature algorithm we support on this platform. */
const char cOTA_JSON_FileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-rsa";

OTA_Err_t prvPAL_CreateFileForRx(OTA_FileContext_t *const C)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_CreateFileForRx");

    OTA_Err_t ret    = kOTA_Err_None;
    int32_t img_type = FICA_IMG_TYPE_NONE;
    int32_t fica_ret = SLN_FLASH_NO_ERROR;

    /* Decide what image type we have based on the file path */
    if (!strcmp((const char *)C->pucFilePath, IMG_TYPE_APP_A_PATH))
    {
        img_type = FICA_IMG_TYPE_APP_A;
    }
    else if (!strcmp((const char *)C->pucFilePath, IMG_TYPE_APP_B_PATH))
    {
        img_type = FICA_IMG_TYPE_APP_B;
    }
    else
    {
        OTA_LOG_L1("[%s] Invalid file path received: '%s'.\r\n", OTA_METHOD_NAME, C->pucFilePath);
        ret = kOTA_Err_RxFileCreateFailed;
    }

    /* Init FICA to be ready for the new application */
    if (kOTA_Err_None == ret)
    {
        fica_ret = FICA_app_program_ext_init(img_type);

        if (SLN_FLASH_NO_ERROR != fica_ret)
        {
            OTA_LOG_L1("[%s] FICA_app_program_ext_init failed, error %d.\r\n", OTA_METHOD_NAME, fica_ret);
            ret = kOTA_Err_RxFileCreateFailed;
        }
        else
        {
            /* Set LED Blue to indicate update is in progress */
            RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_BLUE);
        }
    }

    if (kOTA_Err_None == ret)
    {
        /* We don't need yet anything here, but this field
         * must not remain NULL after this function returns */
        C->pucFile = (void *)0x1;
        OTA_LOG_L1("[%s] OK.\r\n", OTA_METHOD_NAME);
    }
    else
    {
        OTA_LOG_L1("[%s] Failed.\r\n", OTA_METHOD_NAME);
    }

    return ret;
}
/*-----------------------------------------------------------*/

OTA_Err_t prvPAL_Abort(OTA_FileContext_t *const C)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_Abort");

    OTA_Err_t eResult = kOTA_Err_None;

    if ((OTA_GetAgentState() ==
             eOTA_AgentState_Ready && /* When abort called in this state, we may have an invalid job */
         !C->xIsInSelfTest &&         /* Do not reset when in self test */
         eOTA_PAL_ImageState_PendingCommit != platformState) /* Do not reset when pending commit */
        ||
        (C->ulBlocksRemaining != 0 && /* Incomplete transfer? */
         OTA_GetAgentState() !=
             eOTA_AgentState_ShuttingDown)) /* Abort is also called when wifi link is lost, we shouldn't reset then */
    {
        /* Turn on awesome red led show here */
        ErrorBlinkLED();

        if (OTA_GetAgentState() == eOTA_AgentState_Ready)
        {
            OTA_LOG_L1("[%s] Aborting OTA, invalid job or failed to initiate download.\r\n", OTA_METHOD_NAME);
        }
        else
        {
            OTA_LOG_L1("[%s] Aborting OTA, did not receive all file blocks.\r\n", OTA_METHOD_NAME);
        }

        /* Reset NAP bit; restore PRDB context;
         * OTA bit is already set at OTA task initialization phase */
        selfTestCleanup();

        prvPAL_ResetDevice();
    }
    else
    {
        OTA_LOG_L1("[%s] Abort.\r\n", OTA_METHOD_NAME);
    }

    return eResult;
}
/*-----------------------------------------------------------*/

/* Write a block of data to the specified file. */
int16_t prvPAL_WriteBlock(OTA_FileContext_t *const C, uint32_t ulOffset, uint8_t *const pacData, uint32_t ulBlockSize)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_WriteBlock");

    int32_t fica_ret = FICA_app_program_ext_abs(ulOffset, (void *)pacData, ulBlockSize);
    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        OTA_LOG_L1("[%s] FICA_app_program_ext_abs failed, error %d.\r\n", OTA_METHOD_NAME, fica_ret);
        return -1;
    }

    return ulBlockSize;
}
/*-----------------------------------------------------------*/

OTA_Err_t prvPAL_CloseFile(OTA_FileContext_t *const C)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_CloseFile");

    if (NULL == C->pxSignature->ucData)
    {
        OTA_LOG_L1("[%s] Received NULL verification signature for the update binary.\r\n", OTA_METHOD_NAME);
        return kOTA_Err_FileClose;
    }

    int32_t fica_ret = FICA_Save_Signature(C->pxSignature->ucData);
    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        OTA_LOG_L1("[%s] FICA_Save_Signature failed, error %d.\r\n", OTA_METHOD_NAME, fica_ret);
        return kOTA_Err_FileClose;
    }

    fica_ret = FICA_app_program_ext_finalize();
    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        OTA_LOG_L1("[%s] FICA_app_program_ext_finalize failed, error %d.\r\n", OTA_METHOD_NAME, fica_ret);
        return kOTA_Err_FileClose;
    }

    OTA_LOG_L1("[%s] OK.\r\n", OTA_METHOD_NAME);
    C->pucFile = NULL;

    return kOTA_Err_None;
}
/*-----------------------------------------------------------*/

OTA_Err_t prvPAL_ResetDevice(void)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_ResetDevice");

    OTA_LOG_L1("[%s] Resetting the device.\r\n", OTA_METHOD_NAME);

    /* give some time for last logs printing */
    vTaskDelay(2000);

    /* good bye, cruel world! */
    NVIC_SystemReset();

    return kOTA_Err_None;
}
/*-----------------------------------------------------------*/

OTA_Err_t prvPAL_ActivateNewImage(void)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_ActivateNewImage");

    int32_t fica_ret = FICA_Clear_OTA_FlashBit();
    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        OTA_LOG_L1("[%s] FICA_Clear_OTA_FlashBit failed, error %d.\r\n", OTA_METHOD_NAME, fica_ret);
    }

    OTA_LOG_L1("[%s] Activating the new MCU image.\r\n", OTA_METHOD_NAME);
    return prvPAL_ResetDevice();
}
/*-----------------------------------------------------------*/

OTA_Err_t prvPAL_SetPlatformImageState(OTA_ImageState_t eState)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_SetPlatformImageState");
    OTA_Err_t eResult = kOTA_Err_Uninitialized;
    int32_t fica_ret  = 0;

    if (eOTA_ImageState_Accepted == eState)
    {
        OTA_LOG_L1("[%s] Accepted image.\r\n", OTA_METHOD_NAME);

        /* If previous state was PendingCommit, we are now in Self Test mode,
         * so we should commit the image, then signal the ota done task */
        if (eOTA_PAL_ImageState_PendingCommit == platformState)
        {
            /* NAP bit cleared inside the below function */
            fica_ret = FICA_app_program_ext_set_reset_vector();
            if (SLN_FLASH_NO_ERROR != fica_ret)
            {
                OTA_LOG_L1("[%s] Failed to commit image, error %d\r\n", OTA_METHOD_NAME, fica_ret);
            }
            else
            {
                OTA_LOG_L1("[%s] Image successfully committed, using updated app after reset\r\n", OTA_METHOD_NAME);

                /* Set LED to green to indicate a successful transfer */
                RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_GREEN);

#if ENABLE_UNSIGNED_USB_MSD
                bool isUSBMode = false;
                fica_ret       = FICA_get_usb_mode(&isUSBMode);

                /* Clear USB mode here. Need to do this when switching from an MSD written firmware
                 * to a successfully OTA-ed firmware */
                if (fica_ret || isUSBMode)
                {
                    fica_ret = FICA_clr_usb_mode();

                    if (SLN_FLASH_NO_ERROR != fica_ret)
                    {
                        OTA_LOG_L1("[%s] Failed to clear usb mode flag, error %d\r\n", OTA_METHOD_NAME, fica_ret);
                    }
                }
#endif /* ENABLE_UNSIGNED_USB_MSD */
            }

            /* signal ota done task, it'll reset in order to not get stuck in the bootloader  */
            xTaskNotifyGive(s_otaDoneTaskHandle);
        }

        eResult = kOTA_Err_None;
    }
    else if (eOTA_ImageState_Rejected == eState)
    {
        OTA_LOG_L1("[%s] Rejected image.\r\n", OTA_METHOD_NAME);
        eResult = kOTA_Err_None;
    }
    else if (eOTA_ImageState_Aborted == eState)
    {
        OTA_LOG_L1("[%s] Aborted image.\r\n", OTA_METHOD_NAME);
        eResult = kOTA_Err_None;
    }
    else if (eOTA_ImageState_Testing == eState)
    {
        OTA_LOG_L1("[%s] Image state testing.\r\n", OTA_METHOD_NAME);
        platformState = eOTA_PAL_ImageState_PendingCommit;
        eResult       = kOTA_Err_None;
    }
    else
    {
        eResult = kOTA_Err_BadImageState;
    }

    if ((eOTA_ImageState_Rejected == eState) || (eOTA_ImageState_Aborted == eState))
    {
        /* Turn on awesome red led show here */
        ErrorBlinkLED();

        /* Reset NAP bit; restore PRDB context;
         * OTA bit is already set at OTA task initialization phase */
        selfTestCleanup();

        /* signal ota done task, it'll reset in order to not get stuck in the bootloader  */
        xTaskNotifyGive(s_otaDoneTaskHandle);
    }

    return eResult;
}
/*-----------------------------------------------------------*/

OTA_PAL_ImageState_t prvPAL_GetPlatformImageState(void)
{
    DEFINE_OTA_METHOD_NAME("prvPAL_GetPlatformImageState");

    /* we are not really using self test mode for other than checking version got upgraded,
     * no need for extra code here */
    return platformState;
}
/*-----------------------------------------------------------*/

void otaPalOtaDoneTaskSet(TaskHandle_t handle)
{
    s_otaDoneTaskHandle = handle;
}

/* Provide access to private members for testing. */
#ifdef AMAZON_FREERTOS_ENABLE_UNIT_TESTS
#include "aws_ota_pal_test_access_define.h"
#endif
