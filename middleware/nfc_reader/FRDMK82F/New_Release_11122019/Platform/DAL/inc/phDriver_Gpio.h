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


#ifndef PHDRIVER_GPIO_H
#define PHDRIVER_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/** \defgroup phDriver Driver Abstraction Layer (DAL)
*
* \brief This component implements hardware drivers that are necessary for RdLib software modules
* @{
*/

/**
* \brief Port/GPIO PIN functionality.
*/
typedef enum{
    PH_DRIVER_PINFUNC_INPUT = 0x01,        /**< Input pinpad. */
    PH_DRIVER_PINFUNC_OUTPUT,              /**< Output pinpad. */
    PH_DRIVER_PINFUNC_BIDIR,               /**< Bidirectional pinpad. */
    PH_DRIVER_PINFUNC_INTERRUPT            /**< Interrupt pin. */
} phDriver_Pin_Func_t;

/**
* \brief PIN Interrupt configuration.
*/
typedef enum{
    PH_DRIVER_INTERRUPT_LEVELZERO = 0x01,   /**< Interrupt when level zero. */
    PH_DRIVER_INTERRUPT_LEVELONE,           /**< Interrupt when level one. */
    PH_DRIVER_INTERRUPT_RISINGEDGE,         /**< Interrupt on rising edge. */
    PH_DRIVER_INTERRUPT_FALLINGEDGE,        /**< Interrupt on falling edge. */
    PH_DRIVER_INTERRUPT_EITHEREDGE,         /**< Interrupt on either edge. */
} phDriver_Interrupt_Config_t;

/**
*
* \brief Interrupt callback interface which will be called when interrupt is triggered on IRQ Pin.
* \retval  None
*/
typedef void (*pphDriver_InterruptCallBck_t)(void);


/*!
  \brief Pin pull-up and pull-down settings
*/
#define PH_DRIVER_PULL_DOWN                   0x00U /**< Pull-down selected. */
#define PH_DRIVER_PULL_UP                     0x01U /**< Pull-up selected. */

/**
* \brief Port/GPIO PIN configuration.
*/
typedef struct{
    uint8_t bPullSelect; /**< PH_DRIVER_PULL_DOWN for pull-down, PH_DRIVER_PULL_UP for pull-up selection. */
    uint8_t bOutputLogic; /**< Set default output logic either 1 or 0, used in case of output. */
    phDriver_Interrupt_Config_t eInterruptConfig;    /**< Interrupt Pin configuration. */
} phDriver_Pin_Config_t;


/********************************************************************************
 * PORT/GPIO PIN API's
 *******************************************************************************/

/**
 * \brief Configures the Gpio/Port pin for the specified configuration.
 *
 * @param[in] dwPinNumber  Gpio/Port pin number that needs to be configured.
 * @param[in] ePinFunc     Whether Gpio/Port Pin needs to be configured as Input/Output/Interrupt pin.
 * @param[in] pPinConfig   Pin configurations.
 *
 * @return Status of the API
 * @retval #PH_DRIVER_SUCCESS Operation successful.
 */
phStatus_t phDriver_PinConfig(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc, phDriver_Pin_Config_t *pPinConfig);

/**
 * \brief Read the state of the Gpio pin, in case of interrupt pin it will return interrupt status.
 *
 * @param[in] dwPinNumber  Gpio/Port pin number to read.
 * @param[in] ePinFunc     Whether Gpio/Port Pin that needs to be read is Output or Interrupt pin.
 *
 * @return Status of the Gpio/Interrupt status either 1 or 0.
 */
uint8_t phDriver_PinRead(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc);

/**
 * \brief Write to the Gpio/Port pin.
 *
 * @param[in] dwPinNumber  Gpio/Port pin number to write.
 * @param[in] bValue       Either 1 or 0 to write to Gpio/Port pin .
 *
 * @return Nothing
 */
void phDriver_PinWrite(uint32_t dwPinNumber, uint8_t bValue);


/**
 * \brief Clear interrupt status.
 * @param[in] dwPinNumber  Interrupt Pin number to clear interrupt status.
 *
 * @return  Nothing
 */
void phDriver_PinClearIntStatus(uint32_t dwPinNumber);

/** @}
* end of phDriver Driver Abstraction Layer (DAL)
*/

#ifdef __cplusplus
}/*Extern C*/
#endif

#endif /* PHDRIVER_GPIO_H */
