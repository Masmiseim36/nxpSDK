/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/

#ifndef BOARDSELECTION_H
#define BOARDSELECTION_H

/* Select one of the boards through defined symbols (-D).
 * Board Header files. */

#ifdef PHDRIVER_LPC1769PN5180_BOARD
#    include <Board_Lpc1769Pn5180.h>
#endif

#ifdef PHDRIVER_PIPN5180_BOARD
#    include <Board_PiPn5180.h>
#endif

#ifdef PHDRIVER_FRDM_K82FPN5180_BOARD
#    include <Board_FRDM_K82FPn5180.h>
#endif

#ifdef PHDRIVER_LPC1769RC663_BOARD
#    include <Board_Lpc1769Rc663.h>
#endif

#ifdef PHDRIVER_FRDM_K82FRC663_BOARD
#    include <Board_FRDM_K82FRc663.h>
#endif

#ifdef PHDRIVER_PIRC663_BOARD
#    include <Board_PiRc663.h>
#endif

#ifdef PHDRIVER_LPC1769PN5190_BOARD
#    include <Board_Lpc1769Pn5190.h>
#endif

#ifdef PHDRIVER_K82F_PNEV5190B_BOARD
#    include <Board_K82F_PNEV5190B.h>
#endif
#endif /* BOARDSELECTION_H */

