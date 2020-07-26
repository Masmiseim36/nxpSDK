/**************************************************************************
MODULE:    MCOP_XOD_INC.h
CONTAINS:  MicroCANopen Plus Extended PDO, all includes
           Version for use with MicroCANopen library.
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2019.
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.01, EmSA 02-APR-20
           $LastChangedDate: 2020-04-02 17:30:41 +0200 (Thu, 02 Apr 2020) $
           $LastChangedRevision: 4909 $
***************************************************************************/

#ifndef _MCOP_XOD_H
#define _MCOP_XOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "mco_types.h"

#include "mco.h"

#if (USE_REMOTE_ACCESS == 1)
#include "mcohw_com.h"
#include "raccess.h"
#include "raserial.h"
#include "racrc.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // _MCOP_XOD_H
/**************************************************************************
END OF FILE
**************************************************************************/
