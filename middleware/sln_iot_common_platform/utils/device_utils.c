/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <FreeRTOS.h>

#include "mbedtls/base64.h"
#include "board.h"

void APP_GetUniqueID(char **uniqueID, bool removeSpecialCharacters)
{
    uint64_t u64UniqueIDRaw = (uint64_t)((uint64_t)OCOTP->CFG1 << 32ULL) | OCOTP->CFG0;

    uint32_t cIdLen = 0;
    mbedtls_base64_encode(NULL, 0, &cIdLen, (const unsigned char *)&u64UniqueIDRaw, sizeof(uint64_t));
    *uniqueID = (char *)pvPortMalloc(cIdLen + 1);
    memset(*uniqueID, 0, cIdLen + 1);
    uint32_t outputLen = 0;

    mbedtls_base64_encode((unsigned char *)*uniqueID, cIdLen, &outputLen, (const unsigned char *)&u64UniqueIDRaw,
                          sizeof(uint64_t));

    if (removeSpecialCharacters)
    {
        if ('=' == (*uniqueID)[outputLen - 2])
        {
            (*uniqueID)[outputLen - 2] = '\0';
            (*uniqueID)[outputLen - 1] = '\0';
        }
        else if ('=' == (*uniqueID)[outputLen - 1])
        {
            (*uniqueID)[outputLen - 1] = '\0';
        }
    }
}

__attribute__((weak)) void PKCS11_KeyGen(char **keyPhrase)
{
    /* Enter unique pass phrase generation routine here. */
}
