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


#ifndef PHDRIVER_TIMER_H
#define PHDRIVER_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/** \defgroup phDriver Driver Abstraction Layer (DAL)
*
* \brief This component implements hardware drivers that are necessary for RdLib software modules
* @{
*/

/**
* \brief Timer units.
*/
typedef enum{
    PH_DRIVER_TIMER_SECS       = 1,          /**< Seconds timer. */
    PH_DRIVER_TIMER_MILLI_SECS = 1000,          /**< Milliseconds timer. */
    PH_DRIVER_TIMER_MICRO_SECS = 1000000        /**< Microseconds timer. */
} phDriver_Timer_Unit_t;

/**
*
* \brief Timer callback interface which will be called when timer expires.
* \retval  None
*/
typedef void (*pphDriver_TimerCallBck_t)(void);


/********************************************************************************
 * TIMER API's
 *******************************************************************************/

/**
 * \brief Creates and triggers a single shot Timer for the specified time.
 *
 * @param[in] eTimerUnit      Represents timer unit i.e. seconds, milliseconds or microseconds..
 * @param[in] dwTimePeriod    Timer Timeout in units
 * @param[in] pTimerCallBack  Timer callback function in case of timeout. If pTimerCallBack is NULL then phDriver_TimerStart
 *                            will be a blocking call and will return upon Timer Timeout.
 *
 * @return Status of the API
 * @retval #PH_DRIVER_SUCCESS Operation successful.
 */
phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack);

/**
 * \brief Stops the running Timer.
 *
 * @return Status of the API
 * @retval #PH_DRIVER_SUCCESS Operation successful.
 */
phStatus_t phDriver_TimerStop(void);



/** @}
* end of phDriver Driver Abstraction Layer (DAL)
*/

#ifdef __cplusplus
}/*Extern C*/
#endif

#endif /* PHDRIVER_TIMER_H */
