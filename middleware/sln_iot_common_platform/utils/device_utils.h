/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef DEVICE_UTILS_H_
#define DEVICE_UTILS_H_

#include <stdbool.h>
/*!
 * @brief Generates a unique ID for the client, based on device specific ID
 *
 * @param removeSpecialCharacters boolean declaring whether to remove the special padding character
 * @param **uniqueID Pointer to string containing the generated client ID
 *
 */
void APP_GetUniqueID(char **uniqueID, bool removeSpecialCharacters);

/*!
 * @brief Generates a unique passphrase for PKCS11 private key decryption
 *
 * @param keyPhrase Output to ascii string passphrase
 */
void PKCS11_KeyGen(char **keyPhrase);

#endif /* DEVICE_UTILS_H_ */
