/*
*                     Copyright 2015-2019, NXP
*
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
* Internal Wait Routines for Pn5180 specific HAL-Component of Reader Library Framework.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5180_WAIT_H
#define PHHALHW_PN5180_WAIT_H

/**
* Waits for the interrupts to occur, this is a blocking call.
* The function does not return back until any of the interrupt does not occurred
* as specified in the \a dwIrqWaitFor register and the recorded status register
* value is stored in \a dwIrqReg.
*/

phStatus_t phhalHw_Pn5180_WaitIrq(
    phhalHw_Pn5180_DataParams_t * pDataParams,                         /**<[In] DataParameter representing this layer. */
    uint8_t bEnableIrq,                                               /**<[In] Specifies whether the interrupts are to be enabled or not. */
    uint8_t bWaitUntilPowerUp,                                        /**<[In] Specifies whether we have to wait until the power is up. */
    uint32_t dwIrqWaitFor,                                            /**<[In] IRQ line(s) on which we need to wait \note setting of any interrupt bit causes the function to exit. */
    uint32_t * dwIrqReg                                               /**<[Out] The state of the status register loaded here. */
    );

#endif /* PHHALHW_PN5180_WAIT_H */
