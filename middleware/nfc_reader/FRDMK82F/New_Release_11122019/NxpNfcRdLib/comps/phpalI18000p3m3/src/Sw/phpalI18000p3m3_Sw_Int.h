/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Software ISO18000-3 Mode3 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 23 February 2010
*
*/

#ifndef PHPALI18000P3M3_SW_INT_H
#define PHPALI18000P3M3_SW_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name Command Codes
*/
/*@{*/
#define PHPAL_I18000P3M3_CMD_SELECT         0x0AU    /**< Select command code. */
#define PHPAL_I18000P3M3_CMD_BEGIN_ROUND    0x08U     /**< BeginRound command code. */
#define PHPAL_I18000P3M3_CMD_RESIZE_ROUND   0x09U    /**< ResizeRound command code. */
#define PHPAL_I18000P3M3_CMD_NEXT_SLOT      0x00U    /**< NextSlot command code. */
#define PHPAL_I18000P3M3_CMD_ACK            0x01U    /**< ACK command code. */
#define PHPAL_I18000P3M3_CMD_NAK            0xC0U    /**< NAK command code. */
/** @} */

/**
* \name Timeout constants
*/
/*@{*/
#define PHPAL_I18000P3M3_SW_T1_MAX_US   78U
#define PHPAL_I18000P3M3_SW_T2_MIN_US   151U
#define PHPAL_I18000P3M3_SW_T3_MIN_US   25U
#define PHPAL_I18000P3M3_SW_T4_MIN_US   100U
#define PHPAL_I18000P3M3_SW_DELTA_MIN_US   10U
/** @} */

/** @}
* end of ph_Private
*/

#endif /* PHPALI18000P3M3_SW_INT_H_ */
