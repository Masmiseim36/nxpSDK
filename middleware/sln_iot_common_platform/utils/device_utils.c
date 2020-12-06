/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <FreeRTOS.h>

#include "mbedtls/base64.h"
#include "board.h"

__attribute__((weak)) void APP_GetUniqueID(char **uniqueID, bool cloudSafe)
{
    /* Generate unique identifier for the device. */
    uint64_t u64UniqueIDRaw = (uint64_t)((uint64_t)OCOTP->CFG1 << 32ULL) | OCOTP->CFG0;

    /* Encode for string safe usage */
    uint32_t cIdLen = 0;
    mbedtls_base64_encode(NULL, 0, &cIdLen, (const unsigned char *)&u64UniqueIDRaw, sizeof(uint64_t));
    *uniqueID = (char *)pvPortMalloc(cIdLen + 1);
    memset(*uniqueID, 0, cIdLen + 1);
    uint32_t outputLen = 0;

    mbedtls_base64_encode((unsigned char *)*uniqueID, cIdLen, &outputLen, (const unsigned char *)&u64UniqueIDRaw,
                          sizeof(uint64_t));

    /* Encode for cloud safe usage, satisfying [a-zA-Z0-9:_-]+ */
    if (cloudSafe)
    {
        for (uint32_t idx = 0; idx < outputLen; idx++)
        {
            switch ((*uniqueID)[idx])
            {
                case '+':
                    (*uniqueID)[idx] = '_';
                    break;
                case '/':
                    (*uniqueID)[idx] = '-';
                    break;
                case '=':
                    (*uniqueID)[idx] = '\0';
                    break;
                default:
                    /* Any other base64 encoded character should satisfy [a-zA-Z0-9:_-]+ */
                    break;
            }
        }
    }
}

__attribute__((weak)) void PKCS11_KeyGen(char **keyPhrase)
{
    /* Enter unique pass phrase generation routine here. */
}

__attribute__((weak)) uint8_t BUTTON_MSDPressed(void)
{
    /* Check if the button that trigger the MSD is pressed */
    return 0;
}

__attribute__((weak)) uint8_t BUTTON_OTWPressed(void)
{
    /* Check if the button that trigger the FWUPDATE OTW is pressed */
    return 0;
}