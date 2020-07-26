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
* Internal definitions for Software MIFARE product Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#ifndef PHPALMIFARE_INT_H
#define PHPALMIFARE_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

#define PHPAL_MIFARE_RESP_ACK    0x0AU    /**< MIFARE product ACK response code */
#define PHPAL_MIFARE_RESP_NAK0   0x00U    /**< MIFARE product NAK0 response code */
#define PHPAL_MIFARE_RESP_NAK1   0x01U    /**< MIFARE product NAK1 response code */
#define PHPAL_MIFARE_RESP_NAK4   0x04U    /**< MIFARE product NAK4 response code */
#define PHPAL_MIFARE_RESP_NAK5   0x05U    /**< MIFARE product NAK5 response code */
#define PHPAL_MIFARE_RESP_NAK6   0x06U    /**< MIFARE product NAK6 response code */
#define PHPAL_MIFARE_RESP_NAK7   0x07U    /**< MIFARE product NAK7 response code */
#define PHPAL_MIFARE_RESP_NAK9   0x09U    /**< MIFARE product NAK9 response code */

/** @}
* end of ph_Private
*/

#endif /* PHPALMIFARE_INT_H */
