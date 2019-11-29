/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __XCVR_TEST_FSK_H__
#define __XCVR_TEST_FSK_H__

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
/*! *********************************************************************************
* \brief  This function returns instant RSSI value and returns it as unsigned byte.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details Initialization of the xcvr is necessary prior to calling this function
*
***********************************************************************************/
extern uint8_t XcvrFskGetInstantRssi(void);

/*! *********************************************************************************
* \brief  This function sets the transceiver into continuous modulated transmission.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details Initialization of the xcvr and calling XcvrFskLoadPattern are necessary
*         prior to calling this function
*
***********************************************************************************/
extern void XcvrFskModTx(void);

/*! *********************************************************************************
* \brief  This function sets the transceiver into continuous unmodulated transmission.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details 
*
***********************************************************************************/
extern void XcvrFskNoModTx(void);

/*! *********************************************************************************
* \brief  This function sets the transceiver into idle.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details 
*
***********************************************************************************/
extern void XcvrFskIdle(void);

/*! *********************************************************************************
* \brief  This function sets the transceiver into continuous modulated transmission.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details The modulation used is a pseudo-random pattern generated using a LFSR.
*
***********************************************************************************/
extern void XcvrFskTxRand(void);

/*! *********************************************************************************
* \brief  This function loads a 32 bit value into the pattern register used by XcvrFskModTx.
*
* \param[in] u32Pattern The pattern to be loaded.
*
* \ingroup TestFunctions
*
* \details 
*
***********************************************************************************/
extern void XcvrFskLoadPattern(uint32_t u32Pattern);

/*! *********************************************************************************
* \brief  This function gives tx power control to xcvr and sets the power to u8TxPow.
*
* \param[in] u8TxPow Values should be between 0x00 and 0x0F.
*
* \ingroup TestFunctions
*
* \details 
*
***********************************************************************************/
extern void XcvrFskSetTxPower(uint8_t u8TxPow);

/*! *********************************************************************************
* \brief  This function gives tx channel control to xcvr and sets the channel to u8TxChan.
*
* \param[in] u8TxChan Values should be between 0 and 39.
*
* \ingroup TestFunctions
*
* \details 
*
***********************************************************************************/
extern void XcvrFskSetTxChannel(uint8_t u8TxChan);

/*! *********************************************************************************
* \brief  This function gives tx channel control and power to the upper layer.
*
* \param[in] None.
*
* \ingroup TestFunctions
*
* \details Call this function only if XcvrFskSetTxChannel or XcvrFskSetTxPower were called
*         previously.
*
***********************************************************************************/
extern void XcvrFskRestoreTXControl(void);

#endif

