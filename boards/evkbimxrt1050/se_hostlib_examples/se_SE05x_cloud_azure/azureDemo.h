/* Copyright 2018 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#ifndef SOURCES_AZUREDEMO_H_
#define SOURCES_AZUREDEMO_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

#include "mbedtls/pk.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif
#include "sm_types.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#define IOT_UNUSED(x) (void)(x)

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
void azurePubSub(void);

#endif /* SOURCES_AZUREDEMO_H_ */
