/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_PHYKSZ8041_H_
#define _FSL_PHYKSZ8041_H_

#include "fsl_phy.h"

/*!
 * @addtogroup phy_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PHY driver version */
#define FSL_PHY_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*! @brief Defines the PHY registers. */
#define PHY_CONTROL1_REG 0x1EU /*!< The PHY control one register. */
#define PHY_CONTROL2_REG 0x1FU /*!< The PHY control two register. */

#define PHY_CONTROL_ID1 0x22U /*!< The PHY ID1*/

/*!@brief Defines the mask flag of operation mode in control two register*/
#define PHY_CTL1_REMOTELOOP_MASK    0x0008U /*!< The PHY remote loopback mask. */
#define PHY_CTL2_10HALFDUPLEX_MASK  0x0004U /*!< The PHY 10M half duplex mask. */
#define PHY_CTL2_100HALFDUPLEX_MASK 0x0008U /*!< The PHY 100M half duplex mask. */
#define PHY_CTL2_10FULLDUPLEX_MASK  0x0014U /*!< The PHY 10M full duplex mask. */
#define PHY_CTL2_100FULLDUPLEX_MASK 0x0018U /*!< The PHY 100M full duplex mask. */

/*! @brief ENET MDIO operations structure. */
extern const phy_operations_t phyksz8041_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name PHY Driver
 * @{
 */

/*!
 * @brief Initializes PHY.
 *
 *  This function initialize PHY.
 *
 * @param handle       PHY device handle.
 * @param config       Pointer to structure of phy_config_t.
 * @retval kStatus_Success  PHY initializes success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 * @retval kStatus_PHY_AutoNegotiateFail  PHY auto negotiate fail
 */
status_t PHY_KSZ8041_Init(phy_handle_t *handle, const phy_config_t *config);

/*!
 * @brief PHY Write function. This function write data over the SMI to
 * the specified PHY register. This function is called by all PHY interfaces.
 *
 * @param handle  PHY device handle.
 * @param phyReg  The PHY register.
 * @param data    The data written to the PHY register.
 * @retval kStatus_Success     PHY write success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 */
status_t PHY_KSZ8041_Write(phy_handle_t *handle, uint32_t phyReg, uint32_t data);

/*!
 * @brief PHY Read function. This interface read data over the SMI from the
 * specified PHY register. This function is called by all PHY interfaces.
 *
 * @param handle   PHY device handle.
 * @param phyReg   The PHY register.
 * @param dataPtr  The address to store the data read from the PHY register.
 * @retval kStatus_Success  PHY read success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 */
status_t PHY_KSZ8041_Read(phy_handle_t *handle, uint32_t phyReg, uint32_t *dataPtr);

/*!
 * @brief Enables/disables PHY loopback.
 *
 * @param handle   PHY device handle.
 * @param mode     The loopback mode to be enabled, please see "phy_loop_t".
 * All loopback modes should not be set together, when one loopback mode is set
 * another should be disabled.
 * @param speed    PHY speed for loopback mode.
 * @param enable   True to enable, false to disable.
 * @retval kStatus_Success  PHY loopback success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 */
status_t PHY_KSZ8041_EnableLoopback(phy_handle_t *handle, phy_loop_t mode, phy_speed_t speed, bool enable);

/*!
 * @brief Gets the PHY link status.
 *
 * @param handle   PHY device handle.
 * @param status   The link up or down status of the PHY.
 *         - true the link is up.
 *         - false the link is down.
 * @retval kStatus_Success   PHY gets link status success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 */
status_t PHY_KSZ8041_GetLinkStatus(phy_handle_t *handle, bool *status);

/*!
 * @brief Gets the PHY link speed and duplex.
 *
 * @param handle       PHY device handle.
 * @param speed    The address of PHY link speed.
 * @param duplex   The link duplex of PHY.
 * @retval kStatus_Success   PHY gets link speed and duplex success
 * @retval kStatus_PHY_SMIVisitTimeout  PHY SMI visit time out
 */
status_t PHY_KSZ8041_GetLinkSpeedDuplex(phy_handle_t *handle, phy_speed_t *speed, phy_duplex_t *duplex);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_PHYKSZ8041_H_ */
