/**************************************************************************
MODULE:    MCOP_MGR_INC.h
CONTAINS:  MicroCANopen Plus Manager, all includes
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _MCOP_MGR_H
#define _MCOP_MGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcop_inc.h"

#include "comgr.h"
#if USE_CANOPEN_FD
#include "usdoclnt.h"
#endif
#if !USE_CANOPEN_FD || (defined(USE_CANOPEN_DUALMODE) && (USE_CANOPEN_DUALMODE == 1))
#include "sdoclnt.h"
#endif
#include "concisedcf.h"

#if (USE_LSS_MANAGER == 1)
#include "lssmgr.h"
#endif
#if (USE_MLSS_MANAGER == 1)
#include "mlssmgr.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // _MCOP_MGR_H
/**************************************************************************
END OF FILE
**************************************************************************/
