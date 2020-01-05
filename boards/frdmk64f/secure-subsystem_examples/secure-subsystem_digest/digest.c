/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sss_api.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef SSS_HAVE_SOFTWARE

#include "mbedtls/sha256.h"

sss_status_t sss_open_session(sss_session_t *session,
                              sss_type_t subsystem,
                              uint32_t additionalApplicationId,
                              uint32_t connectionMethod,
                              const void *connectionData)
{
    session->subsystem = subsystem;
    return kStatus_SSS_Success;
}

void sss_close_session(sss_session_t *session)
{
    memset(session, 0, sizeof(sss_session_t));
}

sss_status_t sss_digest_context_init(sss_digest_t *context,
                                     sss_session_t *session,
                                     sss_algorithm_t algorithm,
                                     sss_mode_t mode)
{
    context->session   = session;
    context->algorithm = algorithm;
    context->mode      = mode;
    return kStatus_SSS_Success;
}

sss_status_t sss_digest_one_go(
    sss_digest_t *context, const uint8_t *message, size_t messageLen, uint8_t *digest, size_t *digestLen)
{
    if ((context->session->subsystem != kType_SSS_Software) || (context->algorithm != kAlgorithm_SSS_SHA256))
    {
        return kStatus_SSS_InvalidArgument;
    }

    uint8_t localDigest[32] = {0};

    if (0 != mbedtls_sha256_ret(message, messageLen, localDigest, 0))
    {
        return kStatus_SSS_Fail;
    }

    *digestLen = 32u;
    memcpy(digest, localDigest, 32u);
    return kStatus_SSS_Success;
}

void sss_digest_context_free(sss_digest_t *context)
{
    memset(context, 0, sizeof(sss_digest_t));
}
#endif /* SSS_HAVE_SOFTWARE */

sss_status_t test_digest(void)
{
    sss_status_t status = kStatus_SSS_Fail;

    sss_session_t mySession  = {kType_SSS_SubSystem_NONE};
    sss_digest_t myDigestCtx = {0};

    static const uint8_t message[] =
        "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
        "Get thee back into the tempest and the Nights Plutonian shore!"
        "Leave no black plume as a token of that lie thy soul hath spoken!"
        "Leave my loneliness unbroken! quit the bust above my door!"
        "Take thy beak from out my heart, and take thy form from off my door!"
        "Quoth the raven, Nevermore.  ";

    /* Expected SHA-256 for the message. */
    static const unsigned char sha256[] = {0x63, 0x76, 0xea, 0xcc, 0xc9, 0xa2, 0xc0, 0x43, 0xf4, 0xfb, 0x01,
                                           0x34, 0x69, 0xb3, 0x0c, 0xf5, 0x28, 0x63, 0x5c, 0xfa, 0xa5, 0x65,
                                           0x60, 0xef, 0x59, 0x7b, 0xd9, 0x1c, 0xac, 0xaa, 0x31, 0xf7};

    /* open session to specific security subsystem */
    status = sss_open_session(&mySession, kType_SSS_Software, 0, 0, NULL);
    if (status != kStatus_SSS_Success)
    {
        return status;
    }

    /* init digest context */
    status = sss_digest_context_init(&myDigestCtx, &mySession, kAlgorithm_SSS_SHA256, kMode_SSS_Digest);
    if (status != kStatus_SSS_Success)
    {
        return status;
    }

    /* hash */
    size_t digestLen   = 32u;
    uint8_t digest[32] = {0};
    status             = sss_digest_one_go(&myDigestCtx, message, sizeof(message) - 1u, digest, &digestLen);
    if (status != kStatus_SSS_Success)
    {
        return status;
    }

    /* clean up */
    sss_digest_context_free(&myDigestCtx);
    sss_close_session(&mySession);

    if (0 != memcmp(sha256, digest, digestLen))
    {
        do
        {
            __BKPT(0);
        } while (1);
    }

    PRINTF("SHA-256 Test pass\r\n");

    return kStatus_SSS_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("hello world.\r\n");

    test_digest();

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
