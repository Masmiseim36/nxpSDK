/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpt.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"

#include "netc_swt/soem_netc_swt.h"
#include "netc_swt/netc_swt.h"
#include "soem_port.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
/*${header:end}*/

#define CYCLE_SHIFT_NS  440000  // 440us
#define DC_FILTER_CNT   64

#define MASTER_SLAVE_SYNC 1U
#define SOEM_PORT_NAME "ENET0"
#define EXAMPLE_NETC_HAS_NO_SWITCH 0U
#define EXAMPLE_EP_NUM    0U
#define MSGINTR       MSGINTR1
#define EXAMPLE_SWT_PORT0 0x01U
#define EXAMPLE_SWT_PORT2 0x03U

 #define CLOCK_GRANULARITY_NS 25UL
 #define CLOCK_GRANULARITY_FRE (1000000000UL/CLOCK_GRANULARITY_NS)
 #define CLOCK_INCREASE_PER_SEC 1000000000UL 
 
 #define BOARD_LED_RGPIO     BOARD_USER_LED_GPIO
 #define BOARD_LED_RGPIO_PIN BOARD_USER_LED_GPIO_PIN

#define EXAMPLE_SWT_SI       kNETC_ENETC1PSI0

#define EXAMPLE_SWT_PSEUDO_PORT 0x4U

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
void nsleep_to (uint64_t nsec_target);
void osal_gettime(struct timeval *current_time);
void osal_timer_init(uint32_t priority);
status_t APP_SWT_AddTableEntry(void);
int if_port_swt_init(void);
uint64_t system_time64_ns(void);
void update_master_clock(void);
status_t BOARD_InitHardware(void);
status_t NETC_MDIO_Init(void);
status_t NETC_PHY_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */