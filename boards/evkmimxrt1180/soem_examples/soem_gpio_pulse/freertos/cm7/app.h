/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
#include "fsl_rgpio.h"
#include "netc_ep/soem_netc_ep.h"
/*${header:end}*/

#define SOEM_PORT_NAME "ENET4"

#define MSGINTR       MSGINTR1

#define KNETC_EP_CONFIG_SI    kNETC_ENETC0PSI0

#define KNETC_HW_MII_MODE     kNETC_RmiiMode

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT1_IRQn
#define OSAL_TIMER            GPT1
#define OSAL_TIMER_IRQHandler GPT1_IRQHandler
#define OSAL_TIMER_CLK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1)

#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
#define EP0_PORT  0x00U

#define EP0_PHY_ADDR       0x03U
#define NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
status_t BOARD_InitHardware(void);
status_t NETC_EP_MDIO_Init(void);
status_t NETC_EP_PHY_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */