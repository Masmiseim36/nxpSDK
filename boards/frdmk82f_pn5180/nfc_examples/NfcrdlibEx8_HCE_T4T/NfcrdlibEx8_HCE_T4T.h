/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NFC Type 4A Card Emulation application.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 6326 $ (v06.01.00)
* $Date: 2016-10-07 16:28:33 +0530 (Fri, 07 Oct 2016) $
*/

#ifndef EX8_HCE_T4T_H
#define EX8_HCE_T4T_H

/* Reader library layers required for this example */
#define NXPBUILD_EX8_MANDATORY_LAYERS                               \
            (defined(NXPBUILD__PHHAL_HW_PN5180)                     \
            || defined(NXPBUILD__PHHAL_HW_PN7462AU))                \
            && defined(NXPBUILD__PHHAL_HW)                          \
            && defined(NXPBUILD__PHPAL_I14443P4MC_SW)               \
            && defined(NXPBUILD__PHAC_DISCLOOP_SW)                  \
            && defined(NXPBUILD__PHAC_DISCLOOP_TARGET)              \
            && (defined(PH_OSAL_FREERTOS)                 \
            || defined (PH_OSAL_LINUX))                   \
            && defined(NXPBUILD__PHCE_T4T_SW)

/* Check if all required reader library layers are enabled. */
#if NXPBUILD_EX8_MANDATORY_LAYERS

/*******************************************************************************
**  Macros
*******************************************************************************/
/* Set this to 1 for running ISO 10373-6 PICC test bench. When set to 1, code
 * for creating WTX (for TEST_COMMAND3) in ISO test bench will be enabled. */
#define ISO_10373_6_PICC_TEST_BENCH 0

/* Use application callback for processing UPDATE BINARY and proprietary
 * commands */
#define USE_APP_CALLBACK 1

/* Get content length from CC file. Initial content length is 0. */
#define GET_CCFILE_CONTENT_LEN(x) ((((uint16_t)(x[0]) << 8) & 0xFF00) | (x[1]))

/* Get content length specified in file (first 2 bytes of file) for NDEF and
 * proprietary files */
#define GET_FILE_CONTENT_LEN(x) ((((((uint16_t)(x[0]) << 8) & 0xFF00) | (x[1])) > 0)?   \
                                 (((((uint16_t)(x[0]) << 8) & 0xFF00) | (x[1])) + 2) : 0)

/* Enable(1) / Disable(0) printing error/info */
#define DETECT_ERROR 0

#if DETECT_ERROR
    #define DEBUG_ERROR_PRINT CHECK_STATUS
    #define PRINT_INFO(...) DEBUG_PRINTF(__VA_ARGS__)
#else /* DETECT_ERROR */
    #define DEBUG_ERROR_PRINT(x)
    #define PRINT_INFO(...)
#endif /* DETECT_ERROR */

/* Print system critical errors (with which execution can't proceed further)
 * and halt the execution. */
#define ERROR_CRITICAL(...) DEBUG_PRINTF(__VA_ARGS__); while(1) { /* Nothing to do */ }

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define RDLIB_TASK_STACK				(1600/4)
#define APPLICATION_TASK_STACK			(600/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define RDLIB_TASK_STACK				(1600/4)
#else /*  __PN74XXXX__*/
#define RDLIB_TASK_STACK				(1600)
#endif /*  __PN74XXXX__*/
#ifdef __PN74XXXX__
#define APPLICATION_TASK_STACK			(600/4)
#else /*  __PN74XXXX__*/
#define APPLICATION_TASK_STACK			(600)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

#define RDLIB_TASK_PRIO                 2
#define APPLICATION_TASK_PRIO           1

#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define RDLIB_TASK_STACK				0x20000
#define APPLICATION_TASK_STACK			0x20000
#define RDLIB_TASK_PRIO                 0
#define APPLICATION_TASK_PRIO           0

#endif /* PH_OSAL_LINUX */


/*******************************************************************************
**  Forward declarations
*******************************************************************************/
/*
 * WTX Callback called from WTX timer of 14443p3mC PAL.
 */
void pWtoxCallBck(void);

#if USE_APP_CALLBACK
/**
 * Application callback to handle UPDATE BINARY and proprietary commands.
 * Refer phceT4T_AppCallback_t in phceT4T.h for more info.
 * */
phStatus_t AppProcessCmdCallback(
    uint8_t bState,        /* [in] Tag State indicating received command type */
    uint16_t wRxOption,    /* [in] Indicating received data is partial or not */
    uint8_t *pRxData,      /* [in] Received Data */
    uint16_t wRxDataLen,   /* [in] Length of received data */
    uint16_t *pStatusWord, /* [out] Status Word to be sent (part of R-APDU) */
    uint8_t **ppTxData,    /* [out] Data to be transmitted (part of R-APDU) */
    uint16_t *pTxDataLen   /* [out] Length of data to be transmitted */
    );
#endif /* USE_APP_CALLBACK */

/**
 * Application thread. This calls phceT4T_AppProcessCmd() to handle update
 * binary and proprietary commands.
 * Refer phceT4T_AppProcessCmd in phceT4T.h for more info.
 * APIs which does PAL or HAL exchange should not be called from application
 * thread.
 * */
void TApplication(
    void *pParams);        /* [in] Input parameter to thread (not used here) */

/**
 * Do card emulation by starting discovery loop in listen mode and activating
 * ISO 14443p4mC.
 * Refer phceT4T_Activate() in phceT4T.h for info.
 * */
void T4TCardEmulation(void);

/**
 * Reader library thread. This is the main thread which starts discovery loop
 * in listen mode.
 * */
void TReaderLibrary(
    void *pParams);        /* [in] Input parameter to thread (not used here) */

/**
 * Initialize/Configure components required by this Example.
 * */
void phApp_Initialize(void);

/**
 * Configure discovery loop parameters.
 * */
phStatus_t ConfigureCardEmulation(void);

#endif /* NXPBUILD_EX8_MANDATORY_LAYERS */

#endif /* EX8_HCE_T4T_H */
