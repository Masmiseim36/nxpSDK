/**
 * @file gcpJWTDemo.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Demo for GCP JWT
 */


#ifndef SOURCES_GAEJWTDEMO_H_
#define SOURCES_GAEJWTDEMO_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

#include "mbedtls/pk.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* base64_encodeded {"alg":"ES256","typ":"JWT"}"*/
#define JWT_HEADER_BASE64 "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9"

#define JWT_PASSWORLD_BUFFER_LEN ( 0 \
    + sizeof (JWT_HEADER_BASE64) \
    + 1 /* . */ \
    + \
    ((sizeof("{'aud':'','iat':,'exp':}") \
    + sizeof(GCP_PROJECT_NAME) /* AUD */\
    + (2*14) /* iat and exp */ \
    + 1 /* . */ \
    + (2*32) /* sign */ \
    ) * 4 / 3) /* overhead */ \
    + 10 /* some extra buffer */ \
    )

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
void BOARD_InitNetwork(const unsigned char ax_uid[18]);
unsigned long gcpNTPGetEpoch(void);

void gcpPubSub( const char * gcpJWTToken);

void gcpCreateJWTTokenES256(
    unsigned char * b64token,
    size_t * b64tokenLen,
    const char * aud,
    const unsigned long iat,
    const int delta,
    mbedtls_pk_context * pKey);

#endif /* SOURCES_GAEJWTDEMO_H_ */
