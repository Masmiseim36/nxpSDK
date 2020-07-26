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
* Internal functions and defines of the FeliCa application layer.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 01. February 2010
*
*/

#ifndef PHALFELICA_INT_H
#define PHALFELICA_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name FeliCa Command Codes
*/
/*@{*/
#define PHAL_FELICA_CMD_REQUEST_RESPONSE    0x04U   /**< Get the PICCs current mode. */
#define PHAL_FELICA_CMD_REQUEST_SERVICE     0x02U   /**< Get area key version and service key version. */
#define PHAL_FELICA_CMD_READ                0x06U   /**< Read the record value of the specified service. */
#define PHAL_FELICA_CMD_WRITE               0x08U   /**< Write records of the specified service. */
#define PHAL_FELICA_RSP_REQUEST_RESPONSE    0x05U   /**< Response code to the Request Response command. */
#define PHAL_FELICA_RSP_REQUEST_SERVICE     0x03U   /**< Response code to the Request Service command. */
#define PHAL_FELICA_RSP_READ                0x07U   /**< Response code to the Read command. */
#define PHAL_FELICA_RSP_WRITE               0x09U   /**< Response code to the Write command. */
/*@}*/

/** @}
* end of ph_Private
*/

#endif /* PHALFELICA_INT_H */
