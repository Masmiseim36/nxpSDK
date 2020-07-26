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
* Internal definitions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 5076 $ (v06.01.00)
* $Date: 2017-01-23 17:29:09 +0530 (Mon, 23 Jan 2017) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_RC663_SETTINGS_H
#define PHHALHW_RC663_SETTINGS_H

#include "ph_Status.h"

#ifdef NXPBUILD__PHHAL_HW_RC663

/**
 * \name Antenna Specific Macros.
 * Enable the antenna attached to the front end to enable the antenna settings.
 */
/*{@*/
#define NXPBUILD__PHHAL_HW_RC663_65X65      /**< Settings for 65x65 Antenna.*/
/*@}*/

#if defined (NXPBUILD__PHHAL_HW_RC663_65X65)
    #include "phhalHw_Rc663_Settings65x65.h"
#else /* NXPBUILD__PHHAL_HW_RC663_65X65 */
    #error "Enable any one antenna settings"
#endif /* NXPBUILD__PHHAL_HW_RC663_65X65 */

#endif /* NXPBUILD__PHHAL_HW_RC663 */

#endif /* PHHALHW_RC663_SETTINGS_H */
