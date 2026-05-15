/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"
#include "netc_swt/soem_netc_swt.h"
/*${header:end}*/

#define SOEM_PORT_NAME "ENET0"
#define EXAMPLE_NETC_HAS_NO_SWITCH 0U
#define EXAMPLE_EP_NUM    0U
#define MSGINTR       MSGINTR1
#define EXAMPLE_SWT_PORT0 0x01U
#define EXAMPLE_SWT_PORT2 0x03U

#define KNETC_EP_CONFIG_SI    kNETC_ENETC1PSI0

#define NETC_FREQ             CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#define KNETC_HW_MII_MODE     kNETC_RgmiiMode

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT1_IRQn
#define OSAL_TIMER            GPT1
#define OSAL_TIMER_IRQHandler GPT1_IRQHandler
#define OSAL_TIMER_CLK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1)

#define EP_RING_NUM          3U
#define EP_RXBD_NUM          8U
#define EP_TXBD_NUM          8U
#define EP_BD_ALIGN          128U
#define EP_BUFF_SIZE_ALIGN   64U
#define EP_RXBUFF_SIZE       1518U
#define EP_TXBUFF_SIZE       1518U
#define EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_RXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)

#define EP_TXFRAME_NUM 20U
#define TX_INTR_MSG_DATA  1U
#define RX_INTR_MSG_DATA  2U
#define TX_MSIX_ENTRY_IDX 0U
#define RX_MSIX_ENTRY_IDX 1U
#define FRAME_FID         1U
#define EXAMPLE_SWT_MAX_PORT_NUM   4U

/*! Note: Be careful that some ports are multiplexed with SEMC. */
#if !defined(EXAMPLE_SWT_USED_PORT_BITMAP)
#define EXAMPLE_SWT_USED_PORT_BITMAP 0x5U /*! Enabled Switch port bit map, bit n represents port n. */
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
status_t BOARD_InitHardware(void);
status_t NETC_MDIO_Init(void);
status_t NETC_PHY_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */