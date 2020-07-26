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
* Internal definitions for Pn5180 specific HAL-Component of Reader Library Framework.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHHALHW_PN5180_INT_H
#define PHHALHW_PN5180_INT_H

#include <ph_Status.h>
#include <phhalHw.h>

/** \defgroup phhalHw_PN5180_Int Internal
* \brief Pn5180 Int functions
* @{
*/


/* Timer configuration bits */


/* Timer config */
#define TIMER_FREQ                              13.56               /**< Pn5180 clk frequency.*/
#define TMR_RELOAD_VALUE_MASK                   (0x000FFFFFUL)      /**< The reload mask for the timer, also the maximum value that can be loaded into the timer reload register.*/
#define PHHAL_HW_PN5180_MAX_FREQ                13560000U            /**< Pn5180 clk Maximum frequency = 13.56 MHz.*/
#define PHHAL_HW_PN5180_MIN_FREQ                53000U               /**< Pn5180 clk Minimum frequency = 53 KHz.*/
#define PHHAL_HW_PN5180_MAX_TIME_DELAY_MS       19784U               /**< Pn5180 Maximum Time Delay in milli second = 19.78443396226.*/
#define PHHAL_HW_PN5180_MIN_TIME_DELAY_US       1U                   /**< Pn5180 Minimum Time Delay in micro second = 0.00001886792.*/
#define PHHAL_HW_PN5180_CONVERSION_MS_SEC       1000U                /**< MilliSec to Sec (In denominator for calculation purpose) --> 1000.*/
#define PHHAL_HW_PN5180_CONVERSION_US_SEC       1000000U             /**< MicroSec to Sec (In denominator for calculation purpose) --> 1000000.*/


#define PHHAL_HW_PN5180_I14443_ADD_DELAY_US     15U                  /**< Additional digital timeout delay for ISO14443. */
#define PHHAL_HW_PN5180_I15693_ADD_DELAY_US     120U                 /**< Additional digital timeout delay for ISO15693. */
/**
* \name Rf Datarate Values
*/
/*@{*/
#define PHHAL_HW_RF_DATARATE_NO_CHANGE          0x00FFU             /**< Data rate no change. */
/*@}*/


#define PHHAL_HW_CHECK_IRQ_PIN_MASK             0x01U               /**< Mask to Check for the IRQ PIN instead of polling for Status */
#define PHHAL_HW_DISABLE_IRQ_CLEAR_MASK         0x80U               /**< Mask to Disable the Clear of IRQ During the Wait for IRQ */


/**
* \name RF_STATUS_RF_ACTIVE_ERROR_CAUSE Values
*/
/*@{*/
#define RF_ACTIVE_ERROR_NO_EXT_FIELD_DETECTED   3U
/*@}*/

/**
* \name FSD Values
*/
/*@{*/
#define  PHHAL_HW_PN5180_MAX_FSD     256U
/*@}*/


/**
* \brief Returns the RxBuffer pointer, length and size.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Pn5180_GetRxBuffer(
    phhalHw_Pn5180_DataParams_t * pDataParams,          /**<[In] DatParams to this layer.*/
    uint8_t bIsExchange,                               /**<[In] Exchange option specifier.*/
    uint8_t ** pRxBuffer,                              /**<[Out] Pointer to HAL RX buffer.*/
    uint16_t * pRxBufferLen,                           /**<[Out] Length of the buffer.*/
    uint16_t * pRxBufferSize                           /**<[Out] Size of the buffer.*/
    );

/**
* \brief Returns the TxBuffer pointer, length and size.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Pn5180_GetTxBuffer(
    phhalHw_Pn5180_DataParams_t * pDataParams,          /**<[In] DatParams to this layer.*/
    uint8_t bIsExchange,                               /**<[In] Exchange option specifier.*/
    uint8_t ** pTxBuffer,                              /**<[Out] Pointer to HAL TX buffer.*/
    uint16_t * pTxBufferLen,                           /**<[Out] Length of the buffer.*/
    uint16_t * pTxBufferSize                           /**<[Out] Size of the buffer.*/
    );


/**
* \brief Internal set config function.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_PARAMETER_OVERFLOW Setting the parameter value would lead to an overflow.
*/

phStatus_t phhalHw_Pn5180_SetConfig_Int(
    phhalHw_Pn5180_DataParams_t * pDataParams,       /**<[In] DatParams to this layer.*/
    uint16_t wConfig,                               /**<[In] SetConfig option.*/
    uint16_t wValue                                 /**<[In] SetConfig value.*/
    );


/**
* \brief Internal set config function to configure FeliCa EMD register.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_Pn5180_SetConfig_FelicaEmdReg(
    phhalHw_Pn5180_DataParams_t * pDataParams          /**<[In] DatParams to this layer.*/
    );


/**
* \brief Internal set config function to configure FeliCa EMD register bit.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phhalHw_Pn5180_SetConfig_FelicaEmdRegBit(
    phhalHw_Pn5180_DataParams_t * pDataParams,          /**<[In] DatParams to this layer.*/
    uint16_t wValue,                                    /**<[In] SetConfig value.*/
    uint32_t dwMaskValue                                /**<[In] Mask value.*/
    );


/**
* \brief Internal set config function to configure FeliCa EMD register byte(8bit value).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phhalHw_Pn5180_SetConfig_FelicaEmdRegByte(
    phhalHw_Pn5180_DataParams_t * pDataParams,          /**<[In] DatParams to this layer.*/
    uint16_t wValue,                                    /**<[In] SetConfig value.*/
    uint8_t bBytePos,                                   /**<[In] Byte Position value.*/
    uint32_t dwMaskValue                                /**<[In] Mask value.*/
    );


/**
* Reads the data from the h/w buffer
*/

phStatus_t phhalHw_Pn5180_ReadData(
    phhalHw_Pn5180_DataParams_t * pDataParams,            /**<[In] DatParams to this layer. */
    uint8_t ** pData,                                     /**<[Out] Data read from the h/w FIFO. */
    uint16_t * pBytesRead                                 /**<[Out] Number of bytes read. */
    );

/**
* Generic timer routine.
*
*/

phStatus_t phhalHw_Pn5180_TimerStart(
    phhalHw_Pn5180_DataParams_t * pDataParams,        /**<[In] DatParams to this layer.*/
    uint8_t bTimer,                                  /**<[In] Timer1/2/3 to be used.*/
    uint32_t dwStartCond,                            /**<[In] Stop condition(s) for timer. */
    uint32_t dwStopCond,                             /**<[In] Start condition(s) for timer. */
    uint32_t wPrescaler,                             /**<[In] Prescalar to be used. */
    uint32_t dwLoadValue                             /**<[In] Value to be loaded to the timer count. */
    );


/**
* \brief Apply card mode register settings (Tx and Rx Data Rate) according to given parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_SetCardMode(
    phhalHw_Pn5180_DataParams_t * pDataParams,         /**<[In] DatParams to this layer. */
    uint16_t wTxDataRate,                             /**<[In] TX data rate to be used. */
    uint16_t wRxDataRate,                             /**<[In] RX data rate to be used. */
    uint16_t wSubcarrier                              /**<[In] Sub carrier to be used. */
    );


/**
* \brief Starts the timer as part of the time out behaviour.
* \note uses timer 1
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phhalHw_Pn5180_SetTmo(
    phhalHw_Pn5180_DataParams_t *pDataParams,               /**<[In] DatParams to this layer.  */
    uint16_t wTimeout,                                     /**<[In] Time out period. */
    uint8_t  bUnit                                         /**<[In] The unit unit used for specifying the delay. */
    );


/**
* \brief Internal wait function.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phhalHw_Pn5180_Int_Wait(
    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DatParams to this layer.  */
    uint32_t dwLoadValue,                       /**<[In] the load count. */
    uint32_t wPrescaler                         /**<[In] Prescalar for the timer. */
    );

/**
* \brief Internal Get Digital Delay time function.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Int_GetDigiDelay(
    phhalHw_Pn5180_DataParams_t * pDataParams,   /**<[In] DatParams to this layer. */
    uint8_t bIsTimeout,                          /**<[In] Timeout status. */
    uint16_t * pDelayUs                          /**<[Out] Delay time. */
    );

/**
* \brief Internal Get Frame Delay time function.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Int_GetFdt(
    phhalHw_Pn5180_DataParams_t * pDataParams,   /**<[In] DatParams to this layer. */
    phStatus_t wExchangeStatus,                  /**<[In] Exchange status. */
    uint32_t * pTime                             /**<[Out] Timeout value. */
    );

/**
* \brief PN5180 implementation of common receive functionality.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Receive_Int(
    phhalHw_Pn5180_DataParams_t * pDataParams,  /**<[In] DatParams to this layer. */
    uint32_t dwIrqWaitFor,                      /**<[In] Irqs to wait for.*/
    uint8_t ** ppRxBuffer,                      /**<[out] Rx buffer.*/
    uint16_t * pRxLength,                       /**<[out] received length.*/
    uint8_t bIrqEnable                          /**<[In] enable IRQs.*/
    );

/**
* \brief PN5180 implementation of loading of idle command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Int_IdleCommand(
    phhalHw_Pn5180_DataParams_t * pDataParams /**<[In] DatParams to this layer. */
    );

/**
* \brief PN5180 implementation of loading a command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phhalHw_Pn5180_Int_LoadCommand(
    phhalHw_Pn5180_DataParams_t * pDataParams, /**<[In] DatParams to this layer. */
    uint8_t bCmd            /**<[IN] CMD to load*/
    );


/**
* \brief PN5180 implementation of handling the F0 Sync byte automatically for P2P data exchange.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phhalHw_Pn5180_AutoSyncByte(phhalHw_Pn5180_DataParams_t * pDataParams);


/** @}
* end of phhalHw_PN5180_Int group
*/
#endif  /* PHHALHW_PN5180_INT_H */
